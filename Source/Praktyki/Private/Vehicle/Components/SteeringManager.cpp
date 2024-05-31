// Copyright 2023 Teyon. All Rights Reserved.


#include "Vehicle/Components/SteeringManager.h"

#include "ComponentReregisterContext.h"
#include "Vehicle/MeshVehiclePawn.h"
#include "Vehicle/SkeletalVehiclePawn.h"
#include "Vehicle/VehiclePawn.h"
#include "Vehicle/Components/GearboxComponent.h"
#include "Vehicle/Components/VehicleSkeletalMesh.h"
#include "Vehicle/Components/WheelComponent.h"

#define CONVERT_CMS_TO_KMH 0.036f

USteeringManager::USteeringManager()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void USteeringManager::OnRegister()
{
	Super::OnRegister();

	Owner = Cast<AVehiclePawn>(GetOwner());

	if(Owner == nullptr)
	{
		UE_LOG(LogActorComponent, Warning, TEXT("SteeringManager couldn't find it's parent."));
		return;
	}

	if(Owner->IsA(ASkeletalVehiclePawn::StaticClass()))
	{
		UVehicleSkeletalMesh* SkeletalMesh = Cast<ASkeletalVehiclePawn>(Owner)->SkeletalComponent;
		Body = Cast<UPrimitiveComponent>(SkeletalMesh);
	}
	else if(Owner->IsA(AMeshVehiclePawn::StaticClass()))
	{
		UStaticMeshComponent* StaticMesh = Cast<AMeshVehiclePawn>(Owner)->MeshComponent;
		Body = Cast<UPrimitiveComponent>(StaticMesh);
	}
	
	if(Body == nullptr)
	{
		UE_LOG(LogActorComponent, Error, TEXT("SteeringManager couldn't find Body of it's parent."));
		return;
	}
}

void USteeringManager::BeginPlay()
{
	Super::BeginPlay();

	GroundCollisionParams.AddIgnoredActor(Owner);
	
	if (AVehiclePawn* VehiclePawn = Cast<AVehiclePawn>(Owner))
	{
		VehiclePawn->OnInputForwardChange.AddUObject(this, &USteeringManager::OnInputForwardChange);
		VehiclePawn->OnInputRightChange.AddUObject(this, &USteeringManager::OnInputRightChange);
		
		VehiclePawn->OnGearUp.AddUObject(GearBox, &UGearboxComponent::GearUp);
		VehiclePawn->OnGearDown.AddUObject(GearBox, &UGearboxComponent::GearDown);
	}

	const TArray<FName> WheelNames = { "WheelFR", "WheelFL", "WheelRR", "WheelRL" };

	for (int Iterator = 0; Iterator < WheelNames.Num(); Iterator++)
	{
		UWheelComponent* Wheel = Owner->WheelComponents[Iterator];
		Wheel->SetSteeringManager(this);

		if(Iterator < 2)
		{
			// Front wheels
			Wheel->SetRelativeLocation(FVector(FrontWheelsXOffset, WheelsYOffset, WheelsZOffset));

			if(SteerType == ESteerType::FrontS || SteerType == ESteerType::AllS)
				Wheel->CanSteer = true;
			else
				Wheel->CanSteer = false;
			
			if(DriveType == EDriveType::FrontD || DriveType == EDriveType::AllD)
				Wheel->CanAccelerate = true;
			else
				Wheel->CanAccelerate = false;

			Wheel->IsFrontWheel = true;
		}
		else
		{
			// Rear wheels
			Wheel->SetRelativeLocation(FVector(RearWheelsXOffset, WheelsYOffset, WheelsZOffset));
			
			if(SteerType == ESteerType::RearS || SteerType == ESteerType::AllS)
				Wheel->CanSteer = true;
			else
				Wheel->CanSteer = false;
			
			if(DriveType == EDriveType::RearD || DriveType == EDriveType::AllD)
				Wheel->CanAccelerate = true;
			else
				Wheel->CanAccelerate = false;

			Wheel->IsFrontWheel = false;
		}
		WheelsYOffset *= -1;
	}
	
	WheelsYOffset *= -1;
}

void USteeringManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HandleSteeringRotation(DeltaTime);
	CalculateAverageVelocity();
}

void USteeringManager::CalculateAverageVelocity()
{
	float VelocitySum = 0;
	for (const UWheelComponent* Wheel : Owner->WheelComponents)
	{
		if(Wheel->CanAccelerate)
			VelocitySum += Wheel->GetWheelForwardVelocity();
	}

	if(DriveType == EDriveType::AllD)
		Velocity = VelocitySum / 4;
	else
		Velocity = VelocitySum / 2;
}

void USteeringManager::OnInputForwardChange(const float NewForwardInput)
{
	ForwardInputValue = NewForwardInput;
}

void USteeringManager::OnInputRightChange(const float NewRightInput)
{
	RightInputValue = NewRightInput;
}

void USteeringManager::SetGearBox(UGearboxComponent* GearboxComponent)
{
	GearBox = GearboxComponent;
}

void USteeringManager::HandleSteeringRotation(const float DeltaTime)
{
	// Give keyboard player better control at high speed
	const float PercentVelocityForward = FMath::Abs(Body->GetForwardVector().Dot(Body->GetPhysicsLinearVelocity())) * CONVERT_CMS_TO_KMH / MaxVelocity;
	const float MaxSteeringPercent = 1 - FMath::Tanh(PercentVelocityForward);
	const float SteeringStepPercent = SteeringStep * MaxSteeringPercent;

	if(MaxSteeringPercent > 1 || MaxSteeringPercent < 0)
		UE_LOG(LogActorComponent, Error, TEXT("The float value is: %f"), MaxSteeringPercent);
	
	if (RightInputValue == 0)
	{
		if(SteeringPercent == 0)
			return;

		if(FMath::IsNearlyZero(SteeringPercent, SteeringStepPercent))
		{
			SteeringPercent = 0;
			SteeringChanged();
			return;
		}

		// Move towards default rotation (0)
		if(SteeringPercent > 0)
			SteeringPercent -= SteeringStepPercent * DeltaTime;
		else
			SteeringPercent += SteeringStepPercent * DeltaTime;

		SteeringChanged();
		return;
	}
	
	// Move towards desired rotation
	const float DesiredRotation = MaxSteeringPercent * RightInputValue;			// Here I want to end up in few frames
	const float RotationThisFrame = SteeringStepPercent * DeltaTime;			// This much I can rotate in this frame
	const float RotationToGetThru = FMath::Abs(DesiredRotation - SteeringPercent);	// This much rotation I need overall

	if(RotationToGetThru >= RotationThisFrame)
	{
		// I can't go to target this frame
		SteeringPercent = FMath::Clamp(SteeringPercent + RotationThisFrame * FMath::Sign(RightInputValue),
											-MaxSteeringPercent, MaxSteeringPercent);
		SteeringChanged();
		return;
	}

	// I can go to target this frame
	SteeringPercent = DesiredRotation;
	SteeringChanged();
}

void USteeringManager::SteeringChanged()
{
	OnSteeringChanged.Broadcast(SteeringPercent);
}

ESteerType USteeringManager::GetSteerType() const
{
	return SteerType;
}
