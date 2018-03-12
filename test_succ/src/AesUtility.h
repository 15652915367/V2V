#ifndef CONNECT_CLIENT_AES_UTILITY_H
#define CONNECT_CLIENT_AES_UTILITY_H

#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/aes.h>


#define SECRET_KEY_IV_LENGTH		16

/**
 * AES/CBC/PKCS5Padding, aes secret key length 16/24/32 -> 128/192/256 bytes.
 *
 * @author guanghui.liu@youbiai.com
 */
class AesUtility {
public:
	AesUtility();
	~AesUtility();
	
public:
    static void encrypt(
		const char* secretKey, 
		int secretKeyLength, 
		const char* plaintext, 
		int plaintextLength,
		char*& ciphertext,
		int& ciphertextLength);

	static void decrypt(
		const char* secretKey,
		int secretKeyLength,
		const char* ciphertext,
		int ciphertextLength,
		char*& plaintext,
		int& plaintextLength);
};

#endif
