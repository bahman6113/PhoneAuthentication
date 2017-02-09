#include "CryptoHelper.h"
#include <stdio.h>
#include "openssl\aes.h"
#include "openssl\sha.h"
#include "openssl\buffer.h"
#include <openssl/evp.h>
#include <cmath>
#include <cassert>

const BYTE CryptoHelper::pbIV[IV_SIZE] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
const byte CryptoHelper::pbSalt[SALT_SIZE] = { 8, 7, 6, 5, 4, 3, 2, 1, 0 };
const char CryptoHelper::szPass[] = "ThisIsJustASample,ThisIsNotASecureWayToStorePassword";


#pragma comment(lib, "libeay32.lib")

int CryptoHelper::Encryptor(const unsigned char *in, unsigned char *out, const size_t length, const bool enc)
{
	BYTE oKey[KEY_LEN_BYTES] = { 0 };
	BYTE pbTmpIV[IV_SIZE];

	AES_KEY key;

	int iRet = 0;

	if (PKCS5_PBKDF2_HMAC_SHA1(szPass, strlen(szPass), pbSalt, sizeof(pbSalt), ITER_COUNT, KEY_LEN_BYTES, oKey) == 0)
		return 1;
	
	
	if (enc)
		iRet = AES_set_encrypt_key(oKey, 256, &key);
	else
		iRet = AES_set_decrypt_key (oKey, 256, &key);

	if(iRet != 0)
		return 2;

	memcpy(pbTmpIV, pbIV, IV_SIZE);
	AES_cbc_encrypt(in, out, length, &key, pbTmpIV, enc);
	SecureZeroMemory(oKey, sizeof(oKey));
	return 0;
}

int CryptoHelper::GenSHA1(unsigned char *in, unsigned char *out, size_t length)
{
	if (SHA1(in, length, out) == NULL)
		return 1;

	return 0;
}

size_t CryptoHelper::EncOutputSize(const size_t length)
{
	return ceil((float)length / IV_SIZE) * IV_SIZE;
}

char *CryptoHelper::Base64Encode(const unsigned char* buffer, size_t length) { //Encodes a binary base to base64 string
	BIO *bmem, *b64;
	BUF_MEM *bptr;
	b64 = BIO_new(BIO_f_base64());
	bmem = BIO_new(BIO_s_mem());
	b64 = BIO_push(b64, bmem);
	BIO_write(b64, buffer, length);
	BIO_flush(b64);
	BIO_get_mem_ptr(b64, &bptr);
	char *buff = (char *)malloc(bptr->length);
	memcpy(buff, bptr->data, bptr->length - 1);
	buff[bptr->length - 1] = 0;
	BIO_free_all(b64);
	return buff;

}

size_t CryptoHelper::calcDecodeLength(const char* b64input) { //Calculates the length of a decoded string
	size_t len = strlen(b64input),
		padding = 0;

	if (b64input[len - 1] == '=' && b64input[len - 2] == '=') //last two chars are =
		padding = 2;
	else if (b64input[len - 1] == '=') //last char is =
		padding = 1;

	return (len * 3) / 4 - padding;
}

int CryptoHelper::Base64Decode(char* b64message, unsigned char** buffer, size_t* length) { //Decodes a base64 encoded string
	BIO *bio, *b64;

	int decodeLen = calcDecodeLength(b64message);
	*buffer = (unsigned char*)malloc(decodeLen + 1);
	(*buffer)[decodeLen] = '\0';

	bio = BIO_new_mem_buf(b64message, -1);
	b64 = BIO_new(BIO_f_base64());
	bio = BIO_push(b64, bio);

	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Do not use newlines to flush buffer
	*length = BIO_read(bio, *buffer, strlen(b64message));
	assert(*length == decodeLen); //length should equal decodeLen, else something went horribly wrong
	BIO_free_all(bio);

	return (0); //success
}
