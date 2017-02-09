#pragma once
#include <openssl/evp.h>
#define IV_SIZE				16
#define SALT_SIZE			9
#define KEY_LEN_BYTES		32
#define KEY_LEN_BITS		KEY_LEN_BYTES * 8
#define ITER_COUNT			100

class CryptoHelper
{
public:
	static const byte pbIV[];
	static const byte pbSalt[];
	static const char  szPass[];
	static int Encryptor(const unsigned char *in, unsigned char *out, const size_t length, const bool enc);
	static size_t EncOutputSize(const size_t length);
	static int GenSHA1(unsigned char *in, unsigned char *out, size_t length);
	static char *CryptoHelper::Base64Encode(const unsigned char* buffer, size_t length);
	static size_t calcDecodeLength(const char* b64input);
	static int Base64Decode(char* b64message, unsigned char** buffer, size_t* length);
};

