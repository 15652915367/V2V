#include "AesUtility.h"
#include "Struct.h"

using namespace std;

#define AES_SECRET_KEY "buptantbqxmaaaa"

class CodecCrypt{
public:
    static int encode_encrypt(Pkt* originData, int originDataLength, void* encryptedData, int& encryptedDataLength);
    static int decrypt_decode(const char* recvedBuf, int recvedBufLength, Pkt& pkt);
};
