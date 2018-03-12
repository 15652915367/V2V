/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "V2xBsmRequest"
 * 	found in "V2xBsmRequest.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_V2VBsmJamMsg_H_
#define	_V2VBsmJamMsg_H_


#include <asn_application.h>

/* Including external dependencies */
#include "MsgCount.h"
#include <OCTET_STRING.h>
#include "BsmOrJam.h"
#include "JamLevel.h"
#include "TIMESTAMP.h"
#include "DSecond.h"
#include "Position3D.h"
#include "PositionConfidenceSet.h"
#include "TransmissionState.h"
#include "Speed.h"
#include "Heading.h"
#include "SteeringWheelAngle.h"
#include "AccelerationSet4Way.h"
#include "BrakeSystemStatus.h"
#include "VehicleSize.h"
#include "VehicleClassification.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct MotionConfidenceSet;
struct VehicleSafetyExtensions;

/* V2VBsmJamMsg */
typedef struct V2VBsmJamMsg {
	MsgCount_t	 msgCnt;
	OCTET_STRING_t	 vid;
	BsmOrJam_t	 bsmOrJam;
	JamLevel_t	*jamLevel	/* OPTIONAL */;
	TIMESTAMP_t	 timestamp;
	OCTET_STRING_t	*plateNo	/* OPTIONAL */;
	DSecond_t	 secMark;
	Position3D_t	 pos;
	PositionConfidenceSet_t	 accuracy;
	TransmissionState_t	 transmission;
	Speed_t	 speed;
	Heading_t	 heading;
	SteeringWheelAngle_t	*angle	/* OPTIONAL */;
	struct MotionConfidenceSet	*motionCfd	/* OPTIONAL */;
	AccelerationSet4Way_t	 accelSet;
	BrakeSystemStatus_t	 brakes;
	VehicleSize_t	 size;
	VehicleClassification_t	 vehicleClass;
	struct VehicleSafetyExtensions	*safetyExt	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} V2VBsmJamMsg_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_V2VBsmJamMsg;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "MotionConfidenceSet.h"
#include "VehicleSafetyExtensions.h"

#endif	/* _V2VBsmJamMsg_H_ */
#include <asn_internal.h>
