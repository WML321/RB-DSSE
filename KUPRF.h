#ifndef KUPRF_H
#define KUPRF_H

#include <iostream>
#include <openssl/bn.h>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>
#include <openssl/ec.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>
#include <string>

enum OpType { op_add, op_del, op_srh, op_rol };
//key updatable PRF
//Key Size of KUPRF: 32
//Result Size of KUPRF: 33
class KUPRF
{
public:
    EC_GROUP* group_ = EC_GROUP_new_by_curve_name(NID_secp256k1);
    BN_CTX* ctx_ = BN_CTX_new();
public:
    KUPRF() = default;

    ~KUPRF() = default;

    int key_gen(unsigned char* key_out);

    int Eval(unsigned char* out, const unsigned char* key, const std::string& keyword, OpType op);

    int update_token(unsigned char* out, const unsigned char* K1, const unsigned char* K2);

    int update(unsigned char* out, const unsigned char* data, const unsigned char* key);

    int mul(unsigned char* out, const unsigned char* K1, const unsigned char* K2);

    static void init();

    static void clean();

};

#endif //SSE_2018_KUPRF_H