// Copyright 2023 Teyon. All Rights Reserved.


#include "Vehicle/VehicleDataAsset.h"

void UVehicleDataAsset::FillArray()
{
	VehicleParts.Empty();
	VehicleParts.Reserve(38);
	VehicleParts.Add(CockpitConsole);
	VehicleParts.Add(DoorLeft);
	VehicleParts.Add(DoorRight);
	VehicleParts.Add(EngineComponents);
	VehicleParts.Add(Window);
	VehicleParts.Add(FenderLeft);
	VehicleParts.Add(FenderRight);
	VehicleParts.Add(FrontLeftBrakeDisc);
	VehicleParts.Add(FrontRightBrakeDisc);
	VehicleParts.Add(FrontBumper);
	VehicleParts.Add(FontHood);
	VehicleParts.Add(FrontLeftCaliper);
	VehicleParts.Add(FrontRightCaliper);
	VehicleParts.Add(FrontLeftWheel);
	VehicleParts.Add(FrontRightWheel);
	VehicleParts.Add(FrontLeftWheelBlur);
	VehicleParts.Add(FrontRightWheelBlur);
	VehicleParts.Add(MainBody);
	VehicleParts.Add(Net);
	VehicleParts.Add(PedalAcceleration);
	VehicleParts.Add(PedalBrake);
	VehicleParts.Add(RearBoot);
	VehicleParts.Add(RearLeftBrakeDisc);
	VehicleParts.Add(RearRightBrakeDisc);
	VehicleParts.Add(RearBumper);
	VehicleParts.Add(RearDiffuser);
	VehicleParts.Add(RearLeftCaliper);
	VehicleParts.Add(RearRightCaliper);
	VehicleParts.Add(RearSpoiler);
	VehicleParts.Add(RearLeftWheel);
	VehicleParts.Add(RearRightWheel);
	VehicleParts.Add(RearLeftWheelBlur);
	VehicleParts.Add(RearRightWheelBlur);
	VehicleParts.Add(SeatNetClamps);
	VehicleParts.Add(SteeringWheel);
	VehicleParts.Add(WingMirrorLeft);
	VehicleParts.Add(WingMirrorRight);
	VehicleParts.Add(Wiper);
}
