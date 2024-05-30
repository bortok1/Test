// Copyright 2023 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SteeringManager.generated.h"

class AVehiclePawn;
class UGearboxComponent;

UENUM(BlueprintType)
enum class EDriveType : uint8
{
	FrontD	UMETA(DisplayName="Front-Wheel Drive"),
	RearD	UMETA(DisplayName="Rear-Wheel Drive"),
	AllD	UMETA(DisplayName="All-Wheel Drive")
};

UENUM(BlueprintType)
enum class ESteerType : uint8
{
	FrontS	UMETA(DisplayName="Front-Wheel Steer"),
	RearS	UMETA(DisplayName="Rear-Wheel Steer"),
	AllS	UMETA(DisplayName="All-Wheel Steer")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSteeringChanged, float, Rotation);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRAKTYKI_API USteeringManager : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	AVehiclePawn* Owner = nullptr;
	
	UPROPERTY()
	UPrimitiveComponent* Body = nullptr;
	
	USteeringManager();

	UPROPERTY(BlueprintReadOnly)
	UGearboxComponent* GearBox = nullptr;
	
protected:
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Forces|Move")
	float Velocity = 0.f;
	
private:
	void CalculateAverageVelocity();
	
#pragma region Input
private:
	void OnInputForwardChange(float NewForwardInput);
	void OnInputRightChange(float NewRightInput);
	
public:
	UPROPERTY()
	float ForwardInputValue = 0;
	
	UPROPERTY()
	float RightInputValue = 0;
#pragma endregion
	
#pragma region Suspension
public:
	UPROPERTY(EditAnywhere, Category="Forces|Suspewnsion")
	float HoverDistance = 60;
	UPROPERTY(EditAnywhere, Category="Forces|Suspewnsion")
	float Stiffness = 980000;
	UPROPERTY(EditAnywhere, Category="Forces|Suspewnsion")
	float Damping = 98000;
#pragma endregion

#pragma region MoveForwardBreak
public:
	UPROPERTY(EditAnywhere, Category="Forces|Move")
	float AccelerationForce = 10000000.0f;
	UPROPERTY(EditAnywhere, Category="Forces|Move")
	float MaxVelocity = 250.f;
	UPROPERTY(EditAnywhere, Category="Forces|Move")
	UCurveFloat* GearTorqueCurve = nullptr;
	
	void SetGearBox(UGearboxComponent* GearboxComponent);
	
	UPROPERTY(EditAnywhere, Category="Forces|Break")
	float BreakingForce = 300.f;
	UPROPERTY(EditAnywhere, Category="Forces|Break")
	UCurveFloat* BreakingEfficacyCurve = nullptr;
	UPROPERTY(EditAnywhere, Category="Forces|Break")
	float FrictionBreakingForce = 100.f;
	UPROPERTY(EditAnywhere, Category="Forces|Break")
	UCurveFloat* FrictionBreakingEfficacyCurve = nullptr;
#pragma endregion

#pragma region Steering
private:
	void HandleSteeringRotation(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Steering")
	void SteeringChanged();
	
public:
	UPROPERTY()
	float SteeringPercent = 0; // <-1; 1>

	UPROPERTY(BlueprintAssignable, Category = "Steering")
	FOnSteeringChanged OnSteeringChanged;

	UPROPERTY(EditAnywhere, Category="Forces|Steering")
	float MaxSteeringRotation = 30;
	
private:
	UPROPERTY(EditAnywhere, Category="Forces|Steering")
	float SteeringStep = 4;
#pragma endregion

#pragma region Sliding
public:
	UPROPERTY(EditAnywhere, Category="Forces|Slide")
	float CounterSlideForce = 800.f;
	UPROPERTY(EditAnywhere, Category="Forces|Slide")
	UCurveFloat* SteeringGripCurve = nullptr;
	UPROPERTY(EditAnywhere, Category="Forces|Slide")
	UCurveFloat* NonSteeringGripCurve = nullptr;
#pragma endregion
	
#pragma region TouchingGround
public:
	FCollisionQueryParams GroundCollisionParams;
#pragma endregion

#pragma region Wheels
private:
	UPROPERTY(EditAnywhere, Category="Wheels")
	EDriveType DriveType = EDriveType::AllD;
	UPROPERTY(EditAnywhere, Category="Wheels")
	ESteerType SteerType = ESteerType::FrontS;

public:
	ESteerType GetSteerType() const;

private:
	UPROPERTY(EditAnywhere, Category="Wheels|Position")
	float RearWheelsXOffset = -120.f;
	UPROPERTY(EditAnywhere, Category="Wheels|Position")
	float FrontWheelsXOffset = 120.f;
	UPROPERTY(EditAnywhere, Category="Wheels|Position")
	float WheelsYOffset = 90.f;
	UPROPERTY(EditAnywhere, Category="Wheels|Position")
	float WheelsZOffset = 10.f;
#pragma endregion
};
