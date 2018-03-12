#include "AesUtility.h"

AesUtility::AesUtility() {
}

AesUtility::~AesUtility() {
}

void AesUtility::encrypt(
		const char* secretKey, 
		int secretKeyLength, 
		const char* plaintext, 
		int plaintextLength,
		char*& ciphertext,
		int& ciphertextLength) {
    AES_KEY aesKey;
    AES_set_encrypt_key((unsigned char*)secretKey, secretKeyLength * 8, &aesKey);   

    int plaintextPaddingLength = (plaintextLength / 16 + 1) * 16;
    char plaintextPadding[plaintextPaddingLength];
    int paddingChar = plaintextPaddingLength - plaintextLength;
    // PKCS5Padding
    memset(plaintextPadding, paddingChar, sizeof(plaintextPadding));
    memcpy(plaintextPadding, plaintext, plaintextLength);

    ciphertextLength = plaintextPaddingLength;
    ciphertext = new char[ciphertextLength];
    memset(ciphertext, 0, ciphertextLength);

    char iv[SECRET_KEY_IV_LENGTH];
    memset(iv, 0, sizeof(iv));

    AES_cbc_encrypt((unsigned char*)plaintextPadding,
                    (unsigned char*)ciphertext,
                    plaintextPaddingLength,
                    &aesKey,
                    (unsigned char*)iv,
                    AES_ENCRYPT);
}

void AesUtility::decrypt(
		const char* secretKey,
		int secretKeyLength,
		const char* ciphertext,
		int ciphertextLength,
		char*& plaintext,
		int& plaintextLength) {
    AES_KEY aesKey;
    AES_set_decrypt_key((unsigned char*)secretKey, secretKeyLength * 8, &aesKey);   		

    char iv[SECRET_KEY_IV_LENGTH];
    memset(iv, 0, sizeof(iv));

    plaintext = new char[ciphertextLength];
    memset(plaintext, 0, ciphertextLength); 
    
    AES_cbc_encrypt((unsigned char*)ciphertext,
                    (unsigned char*)plaintext,
                    ciphertextLength,
                    &aesKey, 
                    (unsigned char*)iv,
                    AES_DECRYPT);    

    // PKCS5Padding
    int paddingChar = plaintext[ciphertextLength - 1];    
    memset(plaintext + ciphertextLength - paddingChar, 0, paddingChar); 
    plaintextLength = ciphertextLength - paddingChar;
}

