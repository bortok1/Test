// Copyright 2023 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VehiclePawn.generated.h"

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
};
