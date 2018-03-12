#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

#include <iostream>
using namespace std;

#include "DemoRequestBody.h"
#include "CommonUtility.h"

#define BUFFER_SIZE     1048576

int main() {
    DemoRequestBody* encodeRequest = new DemoRequestBody();
    encodeRequest->height = 777;    
    int tempBufferLength = 3;
    char* tempBuffer = new char[tempBufferLength];
    memset(tempBuffer, 0, tempBufferLength);
    memcpy(tempBuffer, "888", tempBufferLength);
    encodeRequest->width.buf = (uint8_t*)tempBuffer;
    encodeRequest->width.size = tempBufferLength;
    encodeRequest->timestamp = 9999999999999;

    // uper encode
    cout << "uper encode" << endl;
    xer_fprint(stdout, &asn_DEF_DemoRequestBody, encodeRequest);            
    char* buffer = new char[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    asn_enc_rval_t encodeResult;
    encodeResult = uper_encode_to_buffer(
        &asn_DEF_DemoRequestBody, 
        encodeRequest, 
        buffer, 
        BUFFER_SIZE);
    if (-1 == encodeResult.encoded) {
        cout 
            << "uper encode failed, " 
            << encodeResult.encoded
            << ", error information="
            << encodeResult.failed_type->name
            << endl;
        return encodeResult.encoded;        
    }
    size_t bufferLength = (encodeResult.encoded + 7) / 8;
    stringstream ss;
    ss
        << "DemoRequestBody, " 
        << bufferLength
        << ", 0x"
        << hex
        << bufferLength
        << " byte, 0x"
        << CommonUtility::byteArrayToHexString(buffer, bufferLength);    
    cout << ss.str() << endl;     

    // uper decode
    cout << "uper decode" << endl;    
    asn_codec_ctx_t* asnCodecCtx = NULL;
    void* decodeRequest = NULL;
    asn_dec_rval_t decodeResult;
    decodeResult = uper_decode_complete(
        asnCodecCtx, 
        &asn_DEF_DemoRequestBody, 
        &decodeRequest,        
        buffer,
        bufferLength);   
    if (decodeResult.code != RC_OK) {   
        cout << "uper decode failed, " << decodeResult.code << endl;
        return decodeResult.code;        
    }
    xer_fprint(stdout, &asn_DEF_DemoRequestBody, decodeRequest);            
    
    ASN_STRUCT_FREE(asn_DEF_DemoRequestBody, encodeRequest);    
    ASN_STRUCT_FREE(asn_DEF_DemoRequestBody, decodeRequest);
    delete[] buffer;

    return 0;
}