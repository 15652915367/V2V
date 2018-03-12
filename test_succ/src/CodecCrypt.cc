#include "CodecCrypt.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <V2VBsmJamMsg.h>


using namespace std;
#define AES_SECRET_KEY_LENGTH SECRET_KEY_IV_LENGTH
#define BUFFER_SIZE     1450     

int CodecCrypt::encode_encrypt(Pkt* originData, int originDataLength, void* encryptedData, int& encryptedDataLength){
    V2VBsmJamMsg_t*  encodeRequest = new V2VBsmJamMsg();
    if (originData->pkt_type == JAM_MSG) {
        encodeRequest->vid.size = RSUID_SIZE;
        encodeRequest->vid.buf = new uint8_t[RSUID_SIZE];
        memcpy(encodeRequest->vid.buf, originData->payload.jamData.rsuid, RSUID_SIZE);
        encodeRequest->bsmOrJam = BsmOrJam_jam;
        encodeRequest->jamLevel = new JamLevel_t(originData->payload.jamData.jam_level);
        encodeRequest->pos.Long = originData->payload.jamData.lng*10000000;
        encodeRequest->pos.lat = originData->payload.jamData.lat*10000000;
        encodeRequest->timestamp = originData->payload.jamData.tv.tv_sec*1000 + originData->payload.jamData.tv.tv_usec/1000;//两终端之间时间并不一定统一, 其实这一项没什么用
        encodeRequest->speed = 0;
        encodeRequest->heading = 0;
        encodeRequest->accelSet.Long = 0;
        
    }else if(originData->pkt_type == BSM_MSG){
        encodeRequest->vid.size = VID_SIZE;
        encodeRequest->vid.buf = new uint8_t[VID_SIZE];
        memcpy(encodeRequest->vid.buf, originData->payload.bsmData.vid, VID_SIZE);
        encodeRequest->bsmOrJam = BsmOrJam_bsm;
        encodeRequest->jamLevel = NULL;
        encodeRequest->pos.Long = originData->payload.bsmData.lng*10000000;
        encodeRequest->pos.lat = originData->payload.bsmData.lat*10000000;
        encodeRequest->timestamp = originData->payload.jamData.tv.tv_sec*1000 + originData->payload.jamData.tv.tv_usec/1000;//两终端之间时间并不一定统一, 其实这一项没什么用
        encodeRequest->speed = originData->payload.bsmData.speed*50;
        encodeRequest->heading = originData->payload.bsmData.heading*80;
        encodeRequest->accelSet.Long = originData->payload.bsmData.acce*100;
        if (originData->payload.bsmData.canData.if_can_valid == false) {
            encodeRequest->brakes.brakePadel = new BrakePedalStatus_t(BrakePedalStatus_unavailable);
        }else{
            if (originData->payload.bsmData.canData.brake_state_e == SWITCH_OFF)   encodeRequest->brakes.brakePadel = new BrakePedalStatus_t(BrakePedalStatus_off);
            else if (originData->payload.bsmData.canData.brake_state_e == SWITCH_ON)    encodeRequest->brakes.brakePadel = new BrakePedalStatus_t(BrakePedalStatus_on);
        }
        if (originData->payload.bsmData.canData.turning_light_state_e == TURN_LEFT) {
            encodeRequest->safetyExt = new VehicleSafetyExtensions_t();

        }else{
            encodeRequest->safetyExt = NULL;
        }
    }

    encodeRequest->msgCnt = 0;//该数据非关键数据, 暂不对其做处理
    encodeRequest->plateNo = NULL;   //该数据非关键数据, 暂不对其做处理
    encodeRequest->secMark = 10;    //该数据非关键数据, 暂不对其做处理
    encodeRequest->pos.elevation = NULL;    //该数据非关键数据, 暂不对其做处理
    encodeRequest->accuracy.pos = 8;    //该数据非关键数据, 暂不对其做处理   0~15  8: 表示误差范围2m
    encodeRequest->accuracy.elevation = NULL;       //该数据非关键数据, 暂不对其做处理   0~15  8: 表示误差范围2m
    encodeRequest->transmission = 0;    //该数据非关键数据, 暂不对其做处理   0~7 档位
    encodeRequest->angle = NULL;    //该数据非关键数据, 暂不对其做处理
    encodeRequest->motionCfd = NULL;    //该数据非关键数据, 暂不对其做处理
    encodeRequest->accelSet.lat = 0;    //该数据非关键数据, 暂不对其做处理
    encodeRequest->accelSet.vert = 0;   //该数据非关键数据, 暂不对其做处理
    encodeRequest->accelSet.yaw = 0;    //该数据非关键数据, 暂不对其做处理
    encodeRequest->size.length = 410;   //该数据非关键数据, 暂不对其做处理
    encodeRequest->size.width = 190;    //该数据非关键数据, 暂不对其做处理
    encodeRequest->vehicleClass.classification = 1;    //该数据非关键数据, 暂不对其做处理
    //赋值结束
    //打印结构体
    xer_fprint(stdout, &asn_DEF_V2VBsmJamMsg, encodeRequest);
    // uper encode
	char buffer[BUFFER_SIZE] = "";
    cout << "uper encode" << endl;
	asn_enc_rval_t encodeResult;
    encodeResult = uper_encode_to_buffer(
        &asn_DEF_V2VBsmJamMsg, 
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
        ASN_STRUCT_FREE(asn_DEF_V2VBsmJamMsg, encodeRequest);
        return encodeResult.encoded;
    }
    size_t bufferLength = (encodeResult.encoded + 7) / 8;
    stringstream ss;
    ss
        << "Encoded length, " 
        << bufferLength
        << ", 0x"
        << hex
        << bufferLength;
    cout << ss.str() << endl;

    // encrypt
	const char aesSecretKey[AES_SECRET_KEY_LENGTH] = AES_SECRET_KEY;
	int ciphertextLength = 0;
	char* ciphertext = NULL;
    cout << "encrypt" << endl;
	AesUtility::encrypt(
        aesSecretKey,
        AES_SECRET_KEY_LENGTH,
        buffer,
        bufferLength,
        ciphertext,
        ciphertextLength);
    cout << "encrypted" << endl;
    cout << "encoded ciphertextLength : " <<ciphertextLength<< endl;
    encryptedDataLength = ciphertextLength;
    memcpy(encryptedData, ciphertext, ciphertextLength);

    free(ciphertext);
    ASN_STRUCT_FREE(asn_DEF_V2VBsmJamMsg, encodeRequest);
    return 1;

}






int CodecCrypt::decrypt_decode(const char* recvedBuf, int recvedBufLength, Pkt& pkt){
    // decrypt
    const char aesSecretKey[AES_SECRET_KEY_LENGTH] = AES_SECRET_KEY;
	char* plainBuffer = NULL;
	cout << "decrypt" << endl;
	int plaintextLength = 0;
	AesUtility::decrypt(
        aesSecretKey,
        AES_SECRET_KEY_LENGTH,
        recvedBuf,
        recvedBufLength,
        plainBuffer,
        plaintextLength);

    // uper decode
    cout << "uper decode" << endl;
	asn_codec_ctx_t* asnCodecCtx = NULL;
    void* decodedBuf = NULL;
    
    asn_dec_rval_t decodeResult;
	decodeResult = uper_decode_complete(
        asnCodecCtx, 
        &asn_DEF_V2VBsmJamMsg, 
        &decodedBuf,        
        plainBuffer,
        plaintextLength);
    if (decodeResult.code != RC_OK) {   
        cout << "uper decode failed, " << decodeResult.code << endl;
        ASN_STRUCT_FREE(asn_DEF_V2VBsmJamMsg, decodedBuf);
        if (asnCodecCtx != NULL)    free(asnCodecCtx);
        if (plainBuffer != NULL)    free(plainBuffer);

        cout << "free succ "<<endl;
        return decodeResult.code;        
    }
    V2VBsmJamMsg_t* decodeRequest = (V2VBsmJamMsg_t*)decodedBuf;
    xer_fprint(stdout, &asn_DEF_V2VBsmJamMsg, decodeRequest); 

    if (decodeRequest->bsmOrJam == BsmOrJam_jam) {
        pkt.pkt_type = JAM_MSG;
        memcpy(pkt.payload.jamData.rsuid, decodeRequest->vid.buf, RSUID_SIZE);
        pkt.payload.jamData.lng = decodeRequest->pos.Long / 10000000.0;
        pkt.payload.jamData.lat = decodeRequest->pos.lat / 10000000.0;
        pkt.payload.jamData.jam_level = *(decodeRequest->jamLevel);

    }else if(decodeRequest->bsmOrJam == BsmOrJam_bsm){
        pkt.pkt_type = BSM_MSG;
        memcpy(pkt.payload.bsmData.vid, decodeRequest->vid.buf, VID_SIZE);
        pkt.payload.bsmData.lng = decodeRequest->pos.Long / 10000000.0;
        pkt.payload.bsmData.lat = decodeRequest->pos.lat / 10000000.0;
        pkt.payload.bsmData.heading = decodeRequest->heading / 80.0;
        pkt.payload.bsmData.speed = decodeRequest->speed / 50.0;
        pkt.payload.bsmData.acce = decodeRequest->accelSet.Long /100.0;
        if (*(decodeRequest->brakes.brakePadel) == BrakePedalStatus_unavailable) {
            pkt.payload.bsmData.canData.if_can_valid = false;
            pkt.payload.bsmData.canData.brake_state_e = SWITCH_OFF;
            pkt.payload.bsmData.canData.turning_light_state_e = INACTIVE;
        } else{
            pkt.payload.bsmData.canData.if_can_valid = true;
            if (*(decodeRequest->brakes.brakePadel) == BrakePedalStatus_off)    pkt.payload.bsmData.canData.brake_state_e = SWITCH_OFF;
            else if (*(decodeRequest->brakes.brakePadel) == BrakePedalStatus_on)    pkt.payload.bsmData.canData.brake_state_e = SWITCH_ON;
            //现在的做法是 有有效的左转向灯数据时, safetyExt项不为空(但也没有别的值). 没有有效的左转向灯数据时, safetyExt项为空。相当粗略的临时做法
            //如有需要可以把safetyExt充分利用起来, 不过那样的话, 还请自行换用别的方法来表示左转灯状态
            if (decodeRequest->safetyExt == NULL)   pkt.payload.bsmData.canData.turning_light_state_e = INACTIVE;
            else    pkt.payload.bsmData.canData.turning_light_state_e = TURN_LEFT;
        }


    }

    ASN_STRUCT_FREE(asn_DEF_V2VBsmJamMsg, decodeRequest);
    free(asnCodecCtx);
    free(plainBuffer);
    return 0;//succ
}
