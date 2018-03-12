/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "SmartCanRequest"
 * 	found in "SmartCanRequest.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_EPSSTA_H_
#define	_EPSSTA_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum EPSSTA {
	EPSSTA_readiness	= 0,
	EPSSTA_normal	= 1,
	EPSSTA_fault	= 2,
	EPSSTA_void	= 3
} e_EPSSTA;

/* EPSSTA */
typedef long	 EPSSTA_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_EPSSTA;
asn_struct_free_f EPSSTA_free;
asn_struct_print_f EPSSTA_print;
asn_constr_check_f EPSSTA_constraint;
ber_type_decoder_f EPSSTA_decode_ber;
der_type_encoder_f EPSSTA_encode_der;
xer_type_decoder_f EPSSTA_decode_xer;
xer_type_encoder_f EPSSTA_encode_xer;
per_type_decoder_f EPSSTA_decode_uper;
per_type_encoder_f EPSSTA_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _EPSSTA_H_ */
#include <asn_internal.h>
