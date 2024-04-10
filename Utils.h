#include "HEAD.h"

#define ST_0 "0"
#define C_0 "0";
constexpr auto LAMBDA = 126;
using namespace std;
class Utils {
public:
    static string K_for_H;
    static unordered_map<string, string> PRP;
public:
	static string num2str(int num) {
		string str;
		stringstream ss;
		ss << num;
		ss >> str;
		return str;
	}

    static string sha256(const string str)
    {
        char buf[10000];
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, str.c_str(), str.size());
        SHA256_Final(hash, &sha256);
        std::string NewString = "";
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        {
            sprintf(buf, "%02x", hash[i]);
            NewString = NewString + buf;
        }
        return NewString;
    }

    static string sha512(const string str)
    {
        char buf[10000];
        unsigned char hash[SHA512_DIGEST_LENGTH];
        SHA512_CTX sha512;
        SHA512_Init(&sha512);
        SHA512_Update(&sha512, str.c_str(), str.size());
        SHA512_Final(hash, &sha512);
        std::string NewString = "";
        for (int i = 0; i < SHA512_DIGEST_LENGTH; i++)
        {
            sprintf(buf, "%02x", hash[i]);
            NewString = NewString + buf;
        }
        return NewString;
    }
    static std::string newxor(const std::string& s1, const std::string& s2) {
        // 将输入的十六进制字符串转换为十进制整数
        unsigned long long int n1, n2;
        std::stringstream ss1(s1), ss2(s2);
        ss1 >> std::hex >> n1;
        ss2 >> std::hex >> n2;

        // 执行异或操作
        unsigned long long int result = n1 ^ n2;

        // 将结果转换为十六进制字符串，并补零至固定长度
        std::stringstream ss;
        ss << std::hex << result;
        std::string fr = ss.str();
        while (fr.length() < 32) {
            fr = "0" + fr;
        }

        return fr;
    }

    /*static string H1(string key, int ct) {
        string fin = key + num_str(ct);
        string out = "";
        out = sha256(fin);
        return out;
    }*/


    static string hmac256(string key, string msg)
    {
        stringstream ss;
        HMAC_CTX* ctx;
        ctx = HMAC_CTX_new();
        unsigned int  len;
        unsigned char out[EVP_MAX_MD_SIZE];
        HMAC_Init_ex(ctx, key.c_str(), key.length(), EVP_sha256(), NULL);
        HMAC_Update(ctx, (unsigned char*)msg.c_str(), msg.length());
        HMAC_Final(ctx, out, &len);
        HMAC_CTX_free(ctx);
        for (unsigned int i = 0; i < len; i++)
        {
            ss << setw(2) << setfill('0') << hex << static_cast<int> (out[i]);
        }
        return ss.str();
    }

    static vector<string> split(string str, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(str);
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    static string hmac512(string key, string msg)
    {
        stringstream ss;
        HMAC_CTX* ctx;
        ctx = HMAC_CTX_new();
        unsigned int  len;
        unsigned char out[EVP_MAX_MD_SIZE];
        HMAC_Init_ex(ctx, key.c_str(), key.length(), EVP_sha512(), NULL);
        HMAC_Update(ctx, (unsigned char*)msg.c_str(), msg.length());
        HMAC_Final(ctx, out, &len);
        HMAC_CTX_free(ctx);
        for (unsigned int i = 0; i < len; i++)
        {
            ss << setw(2) << setfill('0') << hex << static_cast<int> (out[i]);
        }
        return ss.str();
    }

    static string generateRandomKey(int length) {
        unsigned char buffer[1024];
        RAND_bytes(buffer, length);

        // 将随机生成的字节序列转换为十六进制字符串
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for (int i = 0; i < length; ++i) {
            ss << std::setw(2) << static_cast<int>(buffer[i]);
        }
        return ss.str();
    }

    static string F(string key, string msg)
    {
        stringstream ss;
        HMAC_CTX* ctx;
        ctx = HMAC_CTX_new();
        unsigned int  len;
        unsigned char out[EVP_MAX_MD_SIZE];
        HMAC_Init_ex(ctx, key.c_str(), key.length(), EVP_md5(), NULL);
        HMAC_Update(ctx, (unsigned char*)msg.c_str(), msg.length());
        HMAC_Final(ctx, out, &len);
        HMAC_CTX_free(ctx);
        for (unsigned int i = 0; i < len; i++)
        {
            ss << setw(2) << setfill('0') << hex << static_cast<int> (out[i]);
        }
        return ss.str();
    }

    static string H(string msg)
    {
        return hmac256(K_for_H, msg);
    }

    static string rand_str(const int len) {
        // 使用随机设备种子初始化随机数生成器
        boost::random::random_device rng;
        
        // 初始化 Mersenne Twister 伪随机数生成器
        boost::random::mt19937 gen(rng());

        // 定义字符集，这里使用所有数字字符和小写字母
        const std::string charset = "0123456789abcdefghijklmnopqrstuvwxyz";

        // 使用随机数生成器生成指定长度的随机字符串
        std::string result;
        for (int i = 0; i < len; ++i) {
            result.push_back(charset[gen() % charset.length()]);
        }

        return result;
    }

    static string P(string k, string st) {
        string out = H(st);
        PRP[st] = out;
        PRP[out] = st;
        return out;
    }
    static string P_(string k, string st) {
        string out = PRP[st];
        return out;
    }
};

string Utils::K_for_H = "18601d80f3e9858da48c871e3be7773d";