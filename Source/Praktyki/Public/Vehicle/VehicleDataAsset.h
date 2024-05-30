// Copyright 2023 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "VehicleDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FVehiclePart
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BoneName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRotate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsInterior;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh;

	FVehiclePart()
	{
		this->BoneName = "";
		this->bRotate = false;
		this->IsInterior = false;
		this->StaticMesh = nullptr;
	}
};
UCLASS()
class PRAKTYKI_API UVehicleDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	void FillArray();

	UPROPERTY()
	TArray<FVehiclePart> VehicleParts;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart CockpitConsole;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart DoorLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart DoorRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart EngineComponents;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart Window;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart FenderLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart FenderRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart FrontLeftBrakeDisc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart FrontRightBrakeDisc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart FrontBumper;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart FontHood;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart FrontLeftCaliper;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart FrontRightCaliper;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart FrontLeftWheel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart FrontRightWheel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart FrontLeftWheelBlur;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart FrontRightWheelBlur;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart Interior;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart MainBody;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart Net;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart PedalAcceleration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart PedalBrake;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart RearBoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart RearLeftBrakeDisc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart RearRightBrakeDisc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart RearBumper;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart RearDiffuser;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart RearLeftCaliper;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart RearRightCaliper;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart RearSpoiler;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart RearLeftWheel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart RearRightWheel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart RearLeftWheelBlur;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart RearRightWheelBlur;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart SeatNetClamps;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart SteeringWheel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart WingMirrorLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart WingMirrorRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePart Wiper;
};
