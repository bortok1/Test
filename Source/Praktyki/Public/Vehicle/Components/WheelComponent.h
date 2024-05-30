// Copyright 2023 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WheelComponent.generated.h"

class ARaceState;
class USteeringManager;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRAKTYKI_API UWheelComponent : public USceneComponent
{
	GENERATED_BODY()

	UPROPERTY()
	USteeringManager* SteeringManager = nullptr;

	UPROPERTY()
	ARaceState* RaceState = nullptr;
	
public:
	UPROPERTY()
	bool CanSteer = false;

	UPROPERTY()
	bool IsFrontWheel = false;
	
	UPROPERTY()
	bool CanAccelerate = false;
	
	UWheelComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetSteeringManager(USteeringManager* Manager);

	float GetWheelForwardVelocity() const;
	
private:
	UPROPERTY()
	float WheelDeltaTime = 1.f;

	FVector CalculateHoverVector();
	FVector CalculateForwardVector() const;
	FVector CalculateSideSlide() const;
	FVector CalculateBreakInVector(const FVector& BreakingVector, float BreakForce, UCurveFloat* BreakingCurve) const;

	// Velocity of this tire
	// Updated every frame
	UPROPERTY()
	FVector TireWorldVel = FVector::ZeroVector;
	
	UPROPERTY()
	float PreviousCompression = 0;

	UFUNCTION()
	void OnSteeringChangedHandler(float SteeringPercent);

	bool IsTouchingGround();
	
	UPROPERTY()
	FHitResult GroundHitResult;
};
