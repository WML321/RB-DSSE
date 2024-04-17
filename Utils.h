#include "HEAD.h"

#ifndef HEAD
#define HEAD
#define ST_0 "0"
#define C_0 "0";
static int LAMBDA = 126;

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
        EVP_MD_CTX* ctx = EVP_MD_CTX_new();
        if (!ctx) {
            // �����ڴ����ʧ�ܵ����
            return "";
        }

        if (!EVP_DigestInit_ex(ctx, EVP_sha256(), NULL)) {
            // �����ʼ��ʧ�ܵ����
            EVP_MD_CTX_free(ctx);
            return "";
        }

        if (!EVP_DigestUpdate(ctx, str.c_str(), str.size())) {
            // �������ʧ�ܵ����
            EVP_MD_CTX_free(ctx);
            return "";
        }

        unsigned char hash[EVP_MAX_MD_SIZE];
        unsigned int hash_len;
        if (!EVP_DigestFinal_ex(ctx, hash, &hash_len)) {
            // �����ȡ��ϣ���ʧ�ܵ����
            EVP_MD_CTX_free(ctx);
            return "";
        }

        EVP_MD_CTX_free(ctx);

        // ����ϣֵת��Ϊʮ�������ַ���
        std::stringstream ss;
        for (unsigned int i = 0; i < hash_len; ++i) {
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
        }
        return ss.str();
    }

    static string sha512(const string str)
    {
        unsigned char hash[SHA512_DIGEST_LENGTH];
        EVP_MD_CTX* ctx = EVP_MD_CTX_new();
        EVP_DigestInit(ctx, EVP_sha512());
        EVP_DigestUpdate(ctx, str.c_str(), str.size());
        EVP_DigestFinal(ctx, hash, nullptr);
        EVP_MD_CTX_free(ctx);

        std::stringstream ss;
        for (int i = 0; i < SHA512_DIGEST_LENGTH; i++) {
            ss << std::setw(2) << std::setfill('0') << std::hex << static_cast<unsigned int>(hash[i]);
        }
        return ss.str();
    }
    static std::string newxor(const std::string& s1, const std::string& s2) {
        // �������ʮ�������ַ���ת��Ϊʮ��������
        unsigned long long int n1, n2;
        std::stringstream ss1(s1), ss2(s2);
        ss1 >> std::hex >> n1;
        ss2 >> std::hex >> n2;

        // ִ��������
        unsigned long long int result = n1 ^ n2;

        // �����ת��Ϊʮ�������ַ��������������̶�����
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
        //ctx = HMAC_CTX_new();
        unsigned int  len;
        unsigned char out[EVP_MAX_MD_SIZE];
        /*HMAC_Init_ex(ctx, key.c_str(), key.length(), EVP_sha256(), NULL);
        HMAC_Update(ctx, (unsigned char*)msg.c_str(), msg.length());
        HMAC_Final(ctx, out, &len);
        HMAC_CTX_free(ctx);*/
        len = 64;
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
        std::stringstream ss;
        unsigned int len;
        unsigned char out[EVP_MAX_MD_SIZE];

        /*HMAC_CTX* ctx = HMAC_CTX_new();
        HMAC_Init_ex(ctx, key.c_str(), key.length(), EVP_sha512(), nullptr);
        HMAC_Update(ctx, reinterpret_cast<const unsigned char*>(msg.c_str()), msg.length());
        HMAC_Final(ctx, out, &len);
        HMAC_CTX_free(ctx);*/

        for (unsigned int i = 0; i < len; i++) {
            ss << std::setw(2) << std::setfill('0') << std::hex << static_cast<unsigned int>(out[i]);
        }

        return ss.str();
    }

    static string generateRandomKey(int length) {
        unsigned char buffer[1024];
        RAND_bytes(buffer, length);

        // ��������ɵ��ֽ�����ת��Ϊʮ�������ַ���
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
       // ctx = HMAC_CTX_new();
        unsigned int  len;
        unsigned char out[EVP_MAX_MD_SIZE];
        /*HMAC_Init_ex(ctx, key.c_str(), key.length(), EVP_md5(), NULL);
        HMAC_Update(ctx, (unsigned char*)msg.c_str(), msg.length());
        HMAC_Final(ctx, out, &len);
        HMAC_CTX_free(ctx);*/
        len = 64;
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
        // ʹ������豸���ӳ�ʼ�������������
        boost::random::random_device rng;
        
        // ��ʼ�� Mersenne Twister α�����������
        boost::random::mt19937 gen(rng());

        // �����ַ���������ʹ�����������ַ���Сд��ĸ
        const std::string charset = "0123456789abcdefghijklmnopqrstuvwxyz";

        // ʹ�����������������ָ�����ȵ�����ַ���
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
std::unordered_map<std::string, std::string> Utils::PRP;
#endif