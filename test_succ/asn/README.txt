$ cd asn
$ asn1c -EF *.asn
$ asn1c -gen-PER *.asn
$ cp Makefile.am.sample makefile
$ make
$ ar -r libconnect_client_asn1c.a FLOAT.o DOUBLE.o MsgCount.o DYear.o DMonth.o DDay.o DHour.o DMinute.o DSecond.o DTimeOffset.o BasicVehicleClass.o Latitude.o Longitude.o Elevation.o Acceleration.o VerticalAcceleration.o YawRate.o TimeOffset.o CoarseHeading.o ResponsErrodCode.o TIMESTAMP.o AlertType.o Priority.o Radius.o Speed.o Heading.o SteeringWheelAngle.o Confidence.o ExteriorLights.o ThrottlePadel.o HornStatus.o PositionOffsetLLV.o PathPointList.o PositionOffsetLL.o VerticalOffset.o VertOffset-B07.o VertOffset-B08.o VertOffset-B09.o VertOffset-B10.o VertOffset-B11.o VertOffset-B12.o Position-LL-24B.o Position-LL-28B.o Position-LL-32B.o Position-LL-36B.o Position-LL-44B.o Position-LL-48B.o Position-LLmD-64b.o OffsetLL-B12.o OffsetLL-B14.o OffsetLL-B16.o OffsetLL-B18.o OffsetLL-B22.o OffsetLL-B24.o Position3D.o DemoRequestBody.o TransmissionState.o BrakePedalStatus.o TractionControlStatus.o AntiLockBrakeStatus.o StabilityControlStatus.o BrakeBoostApplied.o AuxiliaryBrakeStatus.o SpeedConfidence.o HeadingConfidence.o SteeringWheelAngleConfidence.o PositionConfidence.o ElevationConfidence.o TimeConfidence.o LoginRequestBody.o LoginResponseBody.o LogoutRequestBody.o LogoutResponseBody.o SmartCanRequestBody.o WiperList.o Wiper.o FRONTWIPERSWITCH.o FRONTWIPERINTERMSWITCH.o REARWIPER.o FRONTWASHINGSWITCH.o LightList.o Light.o TURNINGLAMPSWITCH.o EXTERIORLAMPSWITCH.o FOGLAMPSWITCH.o TELLTALEFRONTFOGLIGHT.o TELLTALEREARFOGLIGHT.o LOWBEAMINDICATOR.o HIGHBEAMINDICATOR.o BRAKELIGHTRIGHTACTIVE.o BRAKELIGHTLEFTACTIVE.o PARKLIGHTONWARNING.o RunningList.o Running.o BreakPadelSwtichSts.o VALIDACCELERATOR.o TotalODO.o VALIDWHEELSPEEDFL.o VALIDWHEELSPEEDFR.o VALIDWHEELSPEEDRL.o VALIDWHEELSPEEDRR.o VALIDVEHICLESPEED.o SafetyList.o Safety.o ABSOPERATING.o ESPAEBactive.o ESPOPERATING.o TCSOPERATING.o EPSSTA.o TCUTGSMode.o TCUCurrentGearPosition.o OthersList.o Other.o HANDBRAKE.o AutoHorn.o SmartCanResponseBody.o SmartControlFeedbackRequestBody.o SmartControlFeedbackResponseBody.o SmartControlRequestBody.o SmartControlResponseBody.o SmartFaultRequestBody.o FaultType.o FaultCode.o InnerNo.o SmartFaultResponseBody.o SmartRecognizeRequestBody.o XTypeEnum.o SmartRecognizeResponseBody.o SmartStrategyRequestBody.o SmartStrategyResponseBody.o SyncRequestBody.o SyncResponseBody.o V2xBsmRequestBody.o V2xBsmRequestList.o V2xBsmRequestElement.o BrakeAppliedStatus.o PathHistory.o FullPositionVector.o PathHistoryPoint.o DDateTime.o GNSSstatus.o PathHistoryPointList.o VehicleClassification.o PositionConfidenceSet.o MotionConfidenceSet.o AccelerationSet4Way.o BrakeSystemStatus.o VehicleSize.o VehicleWidth.o VehicleLength.o VehicleHeight.o VehicleSafetyExtensions.o VehicleEventFlags.o PathPrediction.o RadiusOfCurvature.o V2xBsmResponseBody.o V2xEventRequestBody.o V2xEventEnum.o V2xEventResponseBody.o V2xRsiRequestBody.o V2xRsiResponseBody.o V2xSpatRequestBody.o IntersectionStateList.o MinuteOfTheYear.o DescriptiveName.o IntersectionState.o NodeReferenceID.o IntersectionStatusObject.o PhaseList.o RoadRegulatorID.o NodeID.o Phase.o PhaseID.o PhaseStateList.o PhaseState.o LightState.o TimeChangeDetails.o TimeMark.o V2xSpatResponseBody.o BOOLEAN.o IA5String.o INTEGER.o NativeEnumerated.o NativeInteger.o NativeReal.o REAL.o VisibleString.o asn_SEQUENCE_OF.o asn_SET_OF.o constr_CHOICE.o constr_SEQUENCE.o constr_SEQUENCE_OF.o constr_SET_OF.o OCTET_STRING.o BIT_STRING.o asn_codecs_prim.o ber_tlv_length.o ber_tlv_tag.o ber_decoder.o der_encoder.o constr_TYPE.o constraints.o xer_support.o xer_decoder.o xer_encoder.o per_support.o per_decoder.o per_encoder.o per_opentype.o
$ cp *.h ../connect_client/src/include/connect_client_asn1c/
$ cp libconnect_client_asn1c.a ../connect_client/src/lib/
$ g++ -W -Wall -I. TestDemoRequest.cc CommonUtility.cc libconnect_client_asn1c.a -o TestDemoRequest
$ ./TestDemoRequest
uper encode
<DemoRequestBody>
    <height>777</height>
    <width>888</width>
</DemoRequestBody>
DemoRequestBody, 7, 0x7 byte, 0x0203090370e1c0
uper decode
<DemoRequestBody>
    <height>777</height>
    <width>888</width>
</DemoRequestBody>



// asnlab java serial number
4B3A04B196C8CA9E9EA15C9D6BA6AF98A2DEB6C20AD8E55DA8F1DAC95B9A477B165483A1E952F762AD3B52737B14BF7B

// asnlab java
itu.baic.asn.v2xbsmrequest.PositionOffsetLL增加如下构造方法：
public PositionOffsetLL() {
    this.choiceID = 0;
    this.position_LL1 = null;
    this.position_LL2 = null;
    this.position_LL3 = null;
    this.position_LL4 = null;
    this.position_LL5 = null;
    this.position_LL6 = null;
    this.position_LatLon = null;
}

itu.baic.asn.v2xbsmrequest.VerticalOffset增加如下构造方法：
private VerticalOffset() {
    this.choiceID = 0;
    this.offset1 = null;
    this.offset2 = null;
    this.offset3 = null;
    this.offset4 = null;
    this.offset5 = null;
    this.offset6 = null;
    this.elevation = null;
}