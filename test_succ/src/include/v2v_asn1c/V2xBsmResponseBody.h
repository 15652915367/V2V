/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "V2xBsmResponse"
 * 	found in "V2xBsmResponse.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_V2xBsmResponseBody_H_
#define	_V2xBsmResponseBody_H_


#include <asn_application.h>

/* Including external dependencies */
#include "MsgCount.h"
#include "TIMESTAMP.h"
#include <OCTET_STRING.h>
#include <BOOLEAN.h>
#include "ResponsErrodCode.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* V2xBsmResponseBody */
typedef struct V2xBsmResponseBody {
	MsgCount_t	 msgCnt;
	TIMESTAMP_t	 timestamp;
	OCTET_STRING_t	 vid;
	BOOLEAN_t	 msgStatus;
	ResponsErrodCode_t	 errorCode;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} V2xBsmResponseBody_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_V2xBsmResponseBody;

#ifdef __cplusplus
}
#endif

#endif	/* _V2xBsmResponseBody_H_ */
#include <asn_internal.h>
