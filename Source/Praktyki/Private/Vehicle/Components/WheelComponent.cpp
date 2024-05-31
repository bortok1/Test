// Copyright 2023 Teyon. All Rights Reserved.


#include "Vehicle/Components/WheelComponent.h"

#include "RaceState.h"
#include "VectorTypes.h"
#include "Vehicle/Components/GearboxComponent.h"
#include "Vehicle/Components/SteeringManager.h"

#define CONVERT_CMS_TO_KMH 0.036f

UWheelComponent::UWheelComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWheelComponent::BeginPlay()
{
	Super::BeginPlay();
	
	RaceState = Cast<ARaceState>(GetWorld()->GetGameState());
	if(!RaceState)
		UE_LOG(LogTemp, Warning, TEXT("GameState should be ARaceState!"));
	
	if(CanSteer)
	{
		SteeringManager->OnSteeringChanged.AddDynamic(this, &UWheelComponent::OnSteeringChangedHandler);
	}
}

void UWheelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	TireWorldVel = SteeringManager->Body->GetPhysicsLinearVelocityAtPoint(GetComponentLocation()) * CONVERT_CMS_TO_KMH;
	
	WheelDeltaTime = DeltaTime;
	
	if(IsTouchingGround())
	{
		const FVector FrictionBreaking = CalculateBreakInVector(GetForwardVector(), SteeringManager->FrictionBreakingForce, SteeringManager->FrictionBreakingEfficacyCurve);
		const FVector Force = CalculateHoverVector() + CalculateForwardVector() + CalculateSideSlide() + FrictionBreaking;
		SteeringManager->Body->AddForceAtLocation(Force, this->GetComponentLocation());
	}
}

void UWheelComponent::SetSteeringManager(USteeringManager* Manager)
{
	SteeringManager = Manager;
}

float UWheelComponent::GetWheelForwardVelocity() const
{
	return TireWorldVel.Dot(GetForwardVector());
}

FVector UWheelComponent::CalculateHoverVector()
{
	// Compression <0, 1> = <No compression, Full compression>
	const float CompressionNow = (SteeringManager->HoverDistance - GroundHitResult.Distance) / SteeringManager->HoverDistance; 
	const float HoverForce = SteeringManager->Stiffness * CompressionNow + SteeringManager->Damping * (CompressionNow - PreviousCompression) / WheelDeltaTime;
	PreviousCompression = CompressionNow;
	return GetUpVector() * HoverForce;
}

FVector UWheelComponent::CalculateForwardVector() const
{
	if(SteeringManager->ForwardInputValue == 0) return FVector::ZeroVector;	// No input

	const FVector ForwardVector = GetForwardVector();
	const FVector Velocity = SteeringManager->Body->GetPhysicsLinearVelocityAtPoint(GetComponentLocation());
	const float VelocityForward = UE::Geometry::Dot(ForwardVector, Velocity) * CONVERT_CMS_TO_KMH;

	if(FMath::Sign(VelocityForward) == FMath::Sign(SteeringManager->ForwardInputValue) ||
		abs(VelocityForward) < 1.f)
	{
		// Player wants to move
		const float Force = SteeringManager->GearBox->CalculateTorque(VelocityForward)
			* SteeringManager->AccelerationForce * SteeringManager->ForwardInputValue * WheelDeltaTime;
		return ForwardVector * Force;
	}
	
	// Player wants to stop
	return CalculateBreakInVector(ForwardVector, SteeringManager->BreakingForce, SteeringManager->BreakingEfficacyCurve);
}

void UWheelComponent::OnSteeringChangedHandler(const float SteeringPercent)
{
	SetRelativeRotation(FRotator(0, SteeringPercent * SteeringManager->MaxSteeringRotation * (-2 * !IsFrontWheel + 1), 0));
}

FVector UWheelComponent::CalculateSideSlide() const
{
	const FVector BreakingVector = GetRightVector();
	const float Force = SteeringManager->CounterSlideForce;

	UCurveFloat* SlideCurve;
	if(CanSteer)
		SlideCurve = SteeringManager->SteeringGripCurve;
	else
		SlideCurve = SteeringManager->NonSteeringGripCurve;
	
	return CalculateBreakInVector(BreakingVector, Force, SlideCurve);
}

FVector UWheelComponent::CalculateBreakInVector(const FVector& BreakingVector, const float BreakForce, UCurveFloat* BreakingCurve) const
{
	const float SteeringVel = UE::Geometry::Dot(BreakingVector, TireWorldVel);
	const float PartVelInBreakingDir = abs(SteeringVel) / TireWorldVel.Size();
	const float PartVelToMaxVel =  SteeringVel / SteeringManager->MaxVelocity;

	const float BreakingEfficacy = BreakingCurve->GetFloatValue(PartVelToMaxVel * PartVelInBreakingDir);
	
	const float DesiredVelChange = -SteeringVel * BreakingEfficacy;
	const float DesiredAccel = DesiredVelChange / WheelDeltaTime;

	return BreakingVector * BreakForce * DesiredAccel;
}

bool UWheelComponent::IsTouchingGround()
{
	const FVector Start = GetComponentLocation();
	const FVector End = Start - GetUpVector() * SteeringManager->HoverDistance;

	const bool bGround = GetWorld()->LineTraceSingleByChannel(GroundHitResult, Start, End, ECC_Pawn, SteeringManager->GroundCollisionParams);
	if(!bGround)
		GroundHitResult.Reset();
	else if(GroundHitResult.GetComponent()->ComponentHasTag("Invalid ground"))
		RaceState->InvalidateRace();
	
	return bGround;
}
