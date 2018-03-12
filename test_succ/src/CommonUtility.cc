#include "CommonUtility.h"

CommonUtility::CommonUtility() {
}

CommonUtility::~CommonUtility() {
}

std::string CommonUtility::byteArrayToHexString(const char* data, int length) {
    std::stringstream ss;
    for(int i = 0; i < length; ++i) {
        ss << std::setw(2) << std::setfill('0') << std::hex << (data[i] & 0xff);
    }    
    return ss.str();
}

void CommonUtility::Hex2Char(const unsigned char *szHex, unsigned char *rch) {
    for (int i = 0; i < 2; i++) {
        if (*(szHex + i) >= '0' && *(szHex + i) <= '9') {
            *rch = (*rch << 4) + (*(szHex + i) - '0');
        } else if (*(szHex + i) >= 'A' && *(szHex + i) <= 'F') {
            *rch = (*rch << 4) + (*(szHex + i) - 'A' + 10);
        } else if (*(szHex + i) >= 'a' && *(szHex + i) <= 'f') {
            *rch = (*rch << 4) + (*(szHex + i) - 'a' + 10);
        } else {
            break;
        }
    }
}

void CommonUtility::HexStr2CharStr(const unsigned char *pszHexStr, int iSize,
                             unsigned char *pucCharStr) {
    if (iSize % 2 != 0) {
        return;
    }
    unsigned char ch;
    for (int i = 0; i < iSize / 2; i++) {
        Hex2Char(pszHexStr + 2 * i, &ch);
        pucCharStr[i] = ch;
    }
}

int CommonUtility::getRandomInteger(int a, int b) {
    return (rand() % (b - a + 1)) + a;
}

