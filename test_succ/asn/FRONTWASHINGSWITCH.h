/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "SmartCanRequest"
 * 	found in "SmartCanRequest.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_FRONTWASHINGSWITCH_H_
#define	_FRONTWASHINGSWITCH_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum FRONTWASHINGSWITCH {
	FRONTWASHINGSWITCH_inactive	= 0,
	FRONTWASHINGSWITCH_active	= 1
} e_FRONTWASHINGSWITCH;

/* FRONTWASHINGSWITCH */
typedef long	 FRONTWASHINGSWITCH_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_FRONTWASHINGSWITCH;
asn_struct_free_f FRONTWASHINGSWITCH_free;
asn_struct_print_f FRONTWASHINGSWITCH_print;
asn_constr_check_f FRONTWASHINGSWITCH_constraint;
ber_type_decoder_f FRONTWASHINGSWITCH_decode_ber;
der_type_encoder_f FRONTWASHINGSWITCH_encode_der;
xer_type_decoder_f FRONTWASHINGSWITCH_decode_xer;
xer_type_encoder_f FRONTWASHINGSWITCH_encode_xer;
per_type_decoder_f FRONTWASHINGSWITCH_decode_uper;
per_type_encoder_f FRONTWASHINGSWITCH_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _FRONTWASHINGSWITCH_H_ */
#include <asn_internal.h>
