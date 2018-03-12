#ifndef CONNECT_CLIENT_COMMON_UTILITY_H
#define CONNECT_CLIENT_COMMON_UTILITY_H

#include "Client.h"

/**
 * @author guanghui.liu@youbiai.com
 */
class CommonUtility {
public:
	CommonUtility();
	~CommonUtility();
	
public:
    static std::string byteArrayToHexString(const char* data, int length);    
	static void Hex2Char(const unsigned char *szHex, unsigned char *rch);
	static void HexStr2CharStr(const unsigned char *pszHexStr, int iSize, unsigned char *pucCharStr);	
	// get random integer: [a, b].
	static int getRandomInteger(int a, int b);
};

#endif