// Copyright 2023 Teyon. All Rights Reserved.


#include "Vehicle/VehiclePawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

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

		EnhancedInputComponent->BindAction(MoveCameraAction, ETriggerEvent::Triggered, this, &AVehiclePawn::InputCameraMove); 
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

void AVehiclePawn::InputCameraMove(const FInputActionValue& InputActionValue)
{
	const FVector2D Vector = InputActionValue.Get<FVector2D>();
	const float DeltaTime = GetWorld()->GetDeltaSeconds();
	AddControllerYawInput(Vector.X * HandRotationSpeed * DeltaTime);
	AddControllerPitchInput(-Vector.Y * HandRotationSpeed * DeltaTime);
	TimeSinceLastMoved = 0.f;
}

void AVehiclePawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TimeSinceLastMoved += DeltaSeconds;

	if (TimeSinceLastMoved < TimeAfterMove)
		return;
	
	const float CameraRotation = Camera->GetComponentRotation().Yaw;
	const float ActorRotation = GetActorRotation().Yaw;
	
	const float RotationYawDifference = GetRotationDifference(CameraRotation, ActorRotation, CameraAngleErrorMargin);
	if(RotationYawDifference != 0)
	{
		RotateYawSpringArmToDefault(DeltaSeconds, RotationYawDifference);
	}
}

void AVehiclePawn::SetupCamera()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritRoll = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 50.f;
	SpringArm->CameraLagMaxDistance = 100.f;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
}

void AVehiclePawn::RotateYawSpringArmToDefault(const float DeltaTime, const float RotationDifference)
{
	const float AngleDiffMultiplier = FMath::Clamp(FMath::Abs(RotationDifference / AngleDiffFullSpeed), 0, 1);
	const float TimeMultiplier = FMath::Clamp(TimeSinceLastMoved / TimeUntilFullSpeed, 0, 1);
	
	if(RotationDifference > 0)
		AddControllerYawInput(-AutoRotationSpeed * DeltaTime * TimeMultiplier * AngleDiffMultiplier);
	else
		AddControllerYawInput(AutoRotationSpeed * DeltaTime * TimeMultiplier * AngleDiffMultiplier);
}

float AVehiclePawn::GetRotationDifference(const float A, const float B, const float ErrorMargin) {
	float Diff = A - B;

	// Adjust for the shortest path around the circle
	if (Diff > 180.0f) {
		Diff -= 360.0f;
	} else if (Diff < -180.0f) {
		Diff += 360.0f;
	}
	
	if(abs(Diff) < ErrorMargin)
		return 0.f;
	
	return Diff;
}