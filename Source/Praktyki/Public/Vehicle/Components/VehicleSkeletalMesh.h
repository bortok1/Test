// Copyright 2023 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "VehicleSkeletalMesh.generated.h"

class UVehicleDataAsset;

UCLASS()
class PRAKTYKI_API UVehicleSkeletalMesh : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	UVehicleSkeletalMesh();
	virtual void BeginPlay() override;
	
	UPROPERTY()
	UVehicleDataAsset* VehicleDataAsset = nullptr;

#pragma region Steering
	UPROPERTY()
	TArray<UStaticMeshComponent*> FrontLeftWheelMeshes;
	UPROPERTY()
	TArray<UStaticMeshComponent*> FrontRightWheelMeshes;
	UPROPERTY()
	TArray<UStaticMeshComponent*> RearLeftWheelMeshes;
	UPROPERTY()
	TArray<UStaticMeshComponent*> RearRightWheelMeshes;

	UPROPERTY()
	UStaticMeshComponent* SteeringWheel;
#pragma endregion 

#pragma region Drive
	UPROPERTY()
	TArray<UStaticMeshComponent*> DriveFrontLeftWheelMeshes;
	UPROPERTY()
	TArray<UStaticMeshComponent*> DriveFrontRightWheelMeshes;
	UPROPERTY()
	TArray<UStaticMeshComponent*> DriveRearLeftWheelMeshes;
	UPROPERTY()
	TArray<UStaticMeshComponent*> DriveRearRightWheelMeshes;
#pragma endregion
};
