/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "SmartCanRequest"
 * 	found in "SmartCanRequest.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_EXTERIORLAMPSWITCH_H_
#define	_EXTERIORLAMPSWITCH_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum EXTERIORLAMPSWITCH {
	EXTERIORLAMPSWITCH_off	= 0,
	EXTERIORLAMPSWITCH_auto	= 1,
	EXTERIORLAMPSWITCH_park	= 2,
	EXTERIORLAMPSWITCH_lowbeam	= 3
} e_EXTERIORLAMPSWITCH;

/* EXTERIORLAMPSWITCH */
typedef long	 EXTERIORLAMPSWITCH_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_EXTERIORLAMPSWITCH;
asn_struct_free_f EXTERIORLAMPSWITCH_free;
asn_struct_print_f EXTERIORLAMPSWITCH_print;
asn_constr_check_f EXTERIORLAMPSWITCH_constraint;
ber_type_decoder_f EXTERIORLAMPSWITCH_decode_ber;
der_type_encoder_f EXTERIORLAMPSWITCH_encode_der;
xer_type_decoder_f EXTERIORLAMPSWITCH_decode_xer;
xer_type_encoder_f EXTERIORLAMPSWITCH_encode_xer;
per_type_decoder_f EXTERIORLAMPSWITCH_decode_uper;
per_type_encoder_f EXTERIORLAMPSWITCH_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _EXTERIORLAMPSWITCH_H_ */
#include <asn_internal.h>
