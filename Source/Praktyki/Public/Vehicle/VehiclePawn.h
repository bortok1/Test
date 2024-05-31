// Copyright 2023 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VehiclePawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;
class UInputAction;
class USteeringManager;
class UWheelComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInputForwardChange, float)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInputRightChange, float)
DECLARE_MULTICAST_DELEGATE(FOnInputGearUp)
DECLARE_MULTICAST_DELEGATE(FOnInputGearDown)

UCLASS(Abstract)
class PRAKTYKI_API AVehiclePawn : public APawn
{
	GENERATED_BODY()
public:
	AVehiclePawn();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	USteeringManager* SteeringManager = nullptr;

	UPROPERTY()
	TArray<UWheelComponent*> WheelComponents;
	
private:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void InputRight(const FInputActionValue& InputActionValue);
	void InputForward(const FInputActionValue& InputActionValue);
	void InputGearUp();
	void InputGearDown();
	void InputCameraMove(const FInputActionValue& InputActionValue);
	
public:
	FOnInputForwardChange OnInputForwardChange;
	FOnInputForwardChange OnInputRightChange;
	FOnInputGearUp OnGearUp;
	FOnInputGearDown OnGearDown;

protected:
	UPROPERTY(EditAnywhere)
	UInputAction* SteeringAction = nullptr;

	UPROPERTY(EditAnywhere)
	UInputAction* DrivingAction = nullptr;
	
	UPROPERTY(EditAnywhere)
	UInputAction* ChangeGearUpAction = nullptr;
	
	UPROPERTY(EditAnywhere)
	UInputAction* ChangeGearDownAction = nullptr;
	
	UPROPERTY(EditAnywhere)
	UInputAction* MoveCameraAction = nullptr;

public:
	virtual void Tick(float DeltaSeconds) override;

#pragma region Camera rotation
protected:
	void SetupCamera();

private:
	void RotateYawSpringArmToDefault(float DeltaTime, float RotationDifference);
	static float GetRotationDifference(float A, float B, float ErrorMargin);
	
	UPROPERTY()
	float TimeSinceLastMoved = 0.f;
	UPROPERTY()
	float TimeUntilFullSpeed = 5.f;
	UPROPERTY()
	float AngleDiffFullSpeed = 5.f;
	
public:
	UPROPERTY(EditAnywhere, Category="CameraMovement")
	float CameraAngleErrorMargin = 1.f;
	UPROPERTY(EditAnywhere, Category="CameraMovement")
	float TimeAfterMove = 1.f;
	UPROPERTY(EditAnywhere, Category="CameraMovement")
	float AutoRotationSpeed = 10.f;
	UPROPERTY(EditAnywhere, Category="CameraMovement")
	float HandRotationSpeed = 1.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArm = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* Camera = nullptr;
#pragma endregion 
};
