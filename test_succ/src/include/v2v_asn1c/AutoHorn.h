/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "SmartCanRequest"
 * 	found in "SmartCanRequest.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_AutoHorn_H_
#define	_AutoHorn_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum AutoHorn {
	AutoHorn_off	= 0,
	AutoHorn_on	= 1
} e_AutoHorn;

/* AutoHorn */
typedef long	 AutoHorn_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_AutoHorn;
asn_struct_free_f AutoHorn_free;
asn_struct_print_f AutoHorn_print;
asn_constr_check_f AutoHorn_constraint;
ber_type_decoder_f AutoHorn_decode_ber;
der_type_encoder_f AutoHorn_encode_der;
xer_type_decoder_f AutoHorn_decode_xer;
xer_type_encoder_f AutoHorn_encode_xer;
per_type_decoder_f AutoHorn_decode_uper;
per_type_encoder_f AutoHorn_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _AutoHorn_H_ */
#include <asn_internal.h>
