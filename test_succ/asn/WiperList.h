/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "SmartCanRequest"
 * 	found in "SmartCanRequest.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_WiperList_H_
#define	_WiperList_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct Wiper;

/* WiperList */
typedef struct WiperList {
	A_SEQUENCE_OF(struct Wiper) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} WiperList_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_WiperList;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "Wiper.h"

#endif	/* _WiperList_H_ */
#include <asn_internal.h>
