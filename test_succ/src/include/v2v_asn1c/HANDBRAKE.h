/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "SmartCanRequest"
 * 	found in "SmartCanRequest.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_HANDBRAKE_H_
#define	_HANDBRAKE_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum HANDBRAKE {
	HANDBRAKE_off	= 0,
	HANDBRAKE_on	= 1
} e_HANDBRAKE;

/* HANDBRAKE */
typedef long	 HANDBRAKE_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_HANDBRAKE;
asn_struct_free_f HANDBRAKE_free;
asn_struct_print_f HANDBRAKE_print;
asn_constr_check_f HANDBRAKE_constraint;
ber_type_decoder_f HANDBRAKE_decode_ber;
der_type_encoder_f HANDBRAKE_encode_der;
xer_type_decoder_f HANDBRAKE_decode_xer;
xer_type_encoder_f HANDBRAKE_encode_xer;
per_type_decoder_f HANDBRAKE_decode_uper;
per_type_encoder_f HANDBRAKE_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _HANDBRAKE_H_ */
#include <asn_internal.h>
