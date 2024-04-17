#include "KUPRF.h"


using namespace std;
namespace mp = boost::multiprecision;


int KUPRF::key_gen(unsigned char* out)
{
    boost::random::random_device rng;

    // 初始化 OpenSSL 的大数上下文
    BIGNUM* ord = BN_new();
    BIGNUM* bn1 = BN_new();

    // 获取当前椭圆曲线的阶，并将其存储在 ord 中
    // 这里使用 OpenSSL 库中的随机数生成器来生成一个随机数
    // 您可以根据您的需要修改生成随机数的方式和范围
    // 这里只是一个示例
    BN_rand(ord, 256, -1, 0);

    // 使用 OpenSSL 的随机数生成器生成一个随机的大数，其取值范围在 0 到 ord - 1 之间
    BN_rand_range(bn1, ord);

    // 将生成的随机大数 bn1 转换为二进制，并存储在 out 中
    int len = BN_bn2bin(bn1, out);

    // 释放 OpenSSL 的大数上下文
    BN_free(ord);
    BN_free(bn1);

    return len; // 返回生成的随机数的字节数
}

int KUPRF::Eval(unsigned char* out, const unsigned char* key, const string& keyword, OpType op)
{
    unsigned char buf[SHA512_DIGEST_LENGTH];
    unsigned int op_data;
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EC_POINT* ele;
    BIGNUM* bn1;

    ele = EC_POINT_new(group_);
    bn1 = BN_bin2bn(key, 32, nullptr);

    switch (op) {
    case op_add:
        op_data = 0x0000ffff;
        break;
    case op_del:
        op_data = 0xffff0000;
        break;
    case op_srh:
        op_data = 0xffffffff;
        break;
    default:
        op_data = 0;
    }

    // 初始化哈希上下文
    if (!EVP_DigestInit_ex(ctx, EVP_sha512(), NULL)) {
        // 处理初始化失败的情况
        return 0;
    }

    // 更新哈希上下文，使用 EVP_DigestUpdate
    if (!EVP_DigestUpdate(ctx, keyword.c_str(), keyword.size())) {
        // 处理更新失败的情况
        return 0;
    }
    if (!EVP_DigestUpdate(ctx, &op_data, sizeof(op_data))) {
        // 处理更新失败的情况
        return 0;
    }

    // 结束哈希计算并获取结果，使用 EVP_DigestFinal_ex
    unsigned int len;
    if (!EVP_DigestFinal_ex(ctx, buf, &len)) {
        // 处理获取哈希结果失败的情况
        return 0;
    }

    // 将哈希结果映射到椭圆曲线上
    EC_POINT_oct2point(group_, ele, buf, len, nullptr);

    // 执行乘法运算 ele = ele * bn1
    EC_POINT_mul(group_, ele, nullptr, ele, bn1, nullptr);

    // 将结果转换为二进制并存储到输出缓冲区中
    int out_len = EC_POINT_point2oct(group_, ele, POINT_CONVERSION_UNCOMPRESSED, out, 33, nullptr);

    // 释放资源
    EC_POINT_free(ele);
    BN_free(bn1);
    EVP_MD_CTX_free(ctx);

    return out_len;
}

int KUPRF::update_token(unsigned char* out, const unsigned char* K1, const unsigned char* K2) {
    BIGNUM* bn_K1 = BN_bin2bn(K1, 32, nullptr);
    BIGNUM* bn_K2 = BN_bin2bn(K2, 32, nullptr);
    BIGNUM* bn_out = BN_new();
    BIGNUM* b = BN_new();
    BIGNUM* ord = BN_new();

    EC_GROUP_get_order(group_, ord, ctx_);

    // 计算 bn_out = (K1^{-1} mod ord) * K2 mod ord
    BN_mod_inverse(b, bn_K1, ord, ctx_);
    BN_mod_mul(bn_out, b, bn_K2, ord, ctx_);

    // 将结果输出到 out
    BN_bn2bin(bn_out, out);

    // 释放资源
    BN_free(bn_K1);
    BN_free(bn_K2);
    BN_free(bn_out);
    BN_free(b);
    BN_free(ord);

    return 0;
}

int KUPRF::update(unsigned char* out, const unsigned char* token, const unsigned char* data)
{
    BIGNUM* bn1 = BN_bin2bn(token, 32, nullptr);

    // 使用 OpenSSL 的 EC_POINT 类型表示椭圆曲线元素
    EC_POINT* ele = EC_POINT_new(group_);

    // 使用 OpenSSL 的 EC_POINT_oct2point 函数将二进制数据转换为椭圆曲线元素
    EC_POINT_oct2point(group_, ele, data, 33, nullptr);

    // 使用 OpenSSL 的 EC_POINT_mul 函数计算 ele = ele * bn1
    EC_POINT_mul(group_, ele, nullptr, ele, bn1, nullptr);

    // 将椭圆曲线元素 ele 转换为二进制数据，并存储到 out 中
    int len = EC_POINT_point2oct(group_, ele, POINT_CONVERSION_UNCOMPRESSED, out, 33, nullptr);

    // 释放资源
    BN_free(bn1);
    EC_POINT_free(ele);

    return len;
}

int KUPRF::mul(unsigned char* out, const unsigned char* K1, const unsigned char* K2)
{
    BIGNUM* bn_K1 = BN_bin2bn(K1, 32, nullptr);
    BIGNUM* bn_K2 = BN_bin2bn(K2, 32, nullptr);
    BIGNUM* ord = BN_new();

    // 获取椭圆曲线的阶
    EC_GROUP_get_order(group_, ord, ctx_);

    // 计算 K1 * K2 mod ord
    BN_mod_mul(bn_K1, bn_K1, bn_K2, ord, ctx_);

    // 将结果输出到 out
    BN_bn2bin(bn_K1, out);

    // 释放资源
    BN_free(bn_K1);
    BN_free(bn_K2);
    BN_free(ord);

    return 0;
}

void KUPRF::clean()
{
    //core_clean();
}

void KUPRF::init()
{
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CONFIG, NULL);

    // 初始化椭圆曲线参数
    EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);

    // 设置椭圆曲线参数
    EC_GROUP_set_asn1_flag(group, OPENSSL_EC_NAMED_CURVE);

    // 释放资源
    EC_GROUP_free(group);
}