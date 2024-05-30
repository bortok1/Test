// Copyright 2023 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VehiclePawn.h"
#include "SkeletalVehiclePawn.generated.h"

class UVehicleSkeletalMesh;

UCLASS()
class PRAKTYKI_API ASkeletalVehiclePawn : public AVehiclePawn
{
	GENERATED_BODY()

public:
	ASkeletalVehiclePawn();
	
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UVehicleSkeletalMesh* SkeletalComponent = nullptr;

private:
	static void RotateDrivingWheels(TArray<UStaticMeshComponent*> MeshComponents, const UWheelComponent* WheelComponent, float DeltaTime, const bool bRight = false);
	
	UFUNCTION()
	void OnSteeringChangedHandler(float Rotation);
	void RotateSteeringWheels(TArray<UStaticMeshComponent*> MeshComponents, float Rotation, const bool bRear = false) const;

	UPROPERTY()
	float PreviousRotation = 0.f;
};
