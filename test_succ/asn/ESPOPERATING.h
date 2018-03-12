/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "SmartCanRequest"
 * 	found in "SmartCanRequest.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_ESPOPERATING_H_
#define	_ESPOPERATING_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ESPOPERATING {
	ESPOPERATING_inactive	= 0,
	ESPOPERATING_active	= 1
} e_ESPOPERATING;

/* ESPOPERATING */
typedef long	 ESPOPERATING_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ESPOPERATING;
asn_struct_free_f ESPOPERATING_free;
asn_struct_print_f ESPOPERATING_print;
asn_constr_check_f ESPOPERATING_constraint;
ber_type_decoder_f ESPOPERATING_decode_ber;
der_type_encoder_f ESPOPERATING_encode_der;
xer_type_decoder_f ESPOPERATING_decode_xer;
xer_type_encoder_f ESPOPERATING_encode_xer;
per_type_decoder_f ESPOPERATING_decode_uper;
per_type_encoder_f ESPOPERATING_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _ESPOPERATING_H_ */
#include <asn_internal.h>
