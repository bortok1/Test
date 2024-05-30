// Copyright 2023 Teyon. All Rights Reserved.


#include "Vehicle/VehiclePawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AVehiclePawn::AVehiclePawn()
{
	
}

void AVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Triggered, this, &AVehiclePawn::InputRight);
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Completed, this, &AVehiclePawn::InputRight);
		
		EnhancedInputComponent->BindAction(DrivingAction, ETriggerEvent::Triggered, this, &AVehiclePawn::InputForward);
		EnhancedInputComponent->BindAction(DrivingAction, ETriggerEvent::Completed, this, &AVehiclePawn::InputForward);
		
		EnhancedInputComponent->BindAction(ChangeGearUpAction, ETriggerEvent::Triggered, this, &AVehiclePawn::InputGearUp); 
		EnhancedInputComponent->BindAction(ChangeGearDownAction, ETriggerEvent::Triggered, this, &AVehiclePawn::InputGearDown); 
	}
}

void AVehiclePawn::InputRight(const FInputActionValue& InputActionValue)
{
	float Value = InputActionValue.Get<float>();
	if(FMath::IsNearlyZero(InputActionValue.Get<float>(), 0.1f))
		Value = 0;
	
	OnInputRightChange.Broadcast(Value);
}

void AVehiclePawn::InputForward(const FInputActionValue& InputActionValue)
{
	OnInputForwardChange.Broadcast(InputActionValue.Get<float>());
}

void AVehiclePawn::InputGearUp()
{
	OnGearUp.Broadcast();
}

void AVehiclePawn::InputGearDown()
{
	OnGearDown.Broadcast();
}

