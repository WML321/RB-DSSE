#include "KUPRF.h"


using namespace std;
namespace mp = boost::multiprecision;


int KUPRF::key_gen(unsigned char* out)
{
    boost::random::random_device rng;

    // ��ʼ�� OpenSSL �Ĵ���������
    BIGNUM* ord = BN_new();
    BIGNUM* bn1 = BN_new();

    // ��ȡ��ǰ��Բ���ߵĽף�������洢�� ord ��
    // ����ʹ�� OpenSSL ���е������������������һ�������
    // �����Ը���������Ҫ�޸�����������ķ�ʽ�ͷ�Χ
    // ����ֻ��һ��ʾ��
    BN_rand(ord, 256, -1, 0);

    // ʹ�� OpenSSL �����������������һ������Ĵ�������ȡֵ��Χ�� 0 �� ord - 1 ֮��
    BN_rand_range(bn1, ord);

    // �����ɵ�������� bn1 ת��Ϊ�����ƣ����洢�� out ��
    int len = BN_bn2bin(bn1, out);

    // �ͷ� OpenSSL �Ĵ���������
    BN_free(ord);
    BN_free(bn1);

    return len; // �������ɵ���������ֽ���
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

    // ��ʼ����ϣ������
    if (!EVP_DigestInit_ex(ctx, EVP_sha512(), NULL)) {
        // �����ʼ��ʧ�ܵ����
        return 0;
    }

    // ���¹�ϣ�����ģ�ʹ�� EVP_DigestUpdate
    if (!EVP_DigestUpdate(ctx, keyword.c_str(), keyword.size())) {
        // �������ʧ�ܵ����
        return 0;
    }
    if (!EVP_DigestUpdate(ctx, &op_data, sizeof(op_data))) {
        // �������ʧ�ܵ����
        return 0;
    }

    // ������ϣ���㲢��ȡ�����ʹ�� EVP_DigestFinal_ex
    unsigned int len;
    if (!EVP_DigestFinal_ex(ctx, buf, &len)) {
        // �����ȡ��ϣ���ʧ�ܵ����
        return 0;
    }

    // ����ϣ���ӳ�䵽��Բ������
    EC_POINT_oct2point(group_, ele, buf, len, nullptr);

    // ִ�г˷����� ele = ele * bn1
    EC_POINT_mul(group_, ele, nullptr, ele, bn1, nullptr);

    // �����ת��Ϊ�����Ʋ��洢�������������
    int out_len = EC_POINT_point2oct(group_, ele, POINT_CONVERSION_UNCOMPRESSED, out, 33, nullptr);

    // �ͷ���Դ
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

    // ���� bn_out = (K1^{-1} mod ord) * K2 mod ord
    BN_mod_inverse(b, bn_K1, ord, ctx_);
    BN_mod_mul(bn_out, b, bn_K2, ord, ctx_);

    // ���������� out
    BN_bn2bin(bn_out, out);

    // �ͷ���Դ
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

    // ʹ�� OpenSSL �� EC_POINT ���ͱ�ʾ��Բ����Ԫ��
    EC_POINT* ele = EC_POINT_new(group_);

    // ʹ�� OpenSSL �� EC_POINT_oct2point ����������������ת��Ϊ��Բ����Ԫ��
    EC_POINT_oct2point(group_, ele, data, 33, nullptr);

    // ʹ�� OpenSSL �� EC_POINT_mul �������� ele = ele * bn1
    EC_POINT_mul(group_, ele, nullptr, ele, bn1, nullptr);

    // ����Բ����Ԫ�� ele ת��Ϊ���������ݣ����洢�� out ��
    int len = EC_POINT_point2oct(group_, ele, POINT_CONVERSION_UNCOMPRESSED, out, 33, nullptr);

    // �ͷ���Դ
    BN_free(bn1);
    EC_POINT_free(ele);

    return len;
}

int KUPRF::mul(unsigned char* out, const unsigned char* K1, const unsigned char* K2)
{
    BIGNUM* bn_K1 = BN_bin2bn(K1, 32, nullptr);
    BIGNUM* bn_K2 = BN_bin2bn(K2, 32, nullptr);
    BIGNUM* ord = BN_new();

    // ��ȡ��Բ���ߵĽ�
    EC_GROUP_get_order(group_, ord, ctx_);

    // ���� K1 * K2 mod ord
    BN_mod_mul(bn_K1, bn_K1, bn_K2, ord, ctx_);

    // ���������� out
    BN_bn2bin(bn_K1, out);

    // �ͷ���Դ
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

    // ��ʼ����Բ���߲���
    EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);

    // ������Բ���߲���
    EC_GROUP_set_asn1_flag(group, OPENSSL_EC_NAMED_CURVE);

    // �ͷ���Դ
    EC_GROUP_free(group);
}