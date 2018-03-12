/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "V2xBsmRequest"
 * 	found in "V2xBsmRequest.asn"
 * 	`asn1c -gen-PER`
 */

#include "VehicleSafetyExtensions.h"

static asn_TYPE_member_t asn_MBR_VehicleSafetyExtensions_1[] = {
	{ ATF_POINTER, 4, offsetof(struct VehicleSafetyExtensions, events),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VehicleEventFlags,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"events"
		},
	{ ATF_POINTER, 3, offsetof(struct VehicleSafetyExtensions, pathHistory),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PathHistory,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"pathHistory"
		},
	{ ATF_POINTER, 2, offsetof(struct VehicleSafetyExtensions, pathPrediction),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PathPrediction,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"pathPrediction"
		},
	{ ATF_POINTER, 1, offsetof(struct VehicleSafetyExtensions, lights),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ExteriorLights,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"lights"
		},
};
static const int asn_MAP_VehicleSafetyExtensions_oms_1[] = { 0, 1, 2, 3 };
static const ber_tlv_tag_t asn_DEF_VehicleSafetyExtensions_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_VehicleSafetyExtensions_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* events */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* pathHistory */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* pathPrediction */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 } /* lights */
};
static asn_SEQUENCE_specifics_t asn_SPC_VehicleSafetyExtensions_specs_1 = {
	sizeof(struct VehicleSafetyExtensions),
	offsetof(struct VehicleSafetyExtensions, _asn_ctx),
	asn_MAP_VehicleSafetyExtensions_tag2el_1,
	4,	/* Count of tags in the map */
	asn_MAP_VehicleSafetyExtensions_oms_1,	/* Optional members */
	4, 0,	/* Root/Additions */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_VehicleSafetyExtensions = {
	"VehicleSafetyExtensions",
	"VehicleSafetyExtensions",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	SEQUENCE_decode_uper,
	SEQUENCE_encode_uper,
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_VehicleSafetyExtensions_tags_1,
	sizeof(asn_DEF_VehicleSafetyExtensions_tags_1)
		/sizeof(asn_DEF_VehicleSafetyExtensions_tags_1[0]), /* 1 */
	asn_DEF_VehicleSafetyExtensions_tags_1,	/* Same as above */
	sizeof(asn_DEF_VehicleSafetyExtensions_tags_1)
		/sizeof(asn_DEF_VehicleSafetyExtensions_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_VehicleSafetyExtensions_1,
	4,	/* Elements count */
	&asn_SPC_VehicleSafetyExtensions_specs_1	/* Additional specs */
};

