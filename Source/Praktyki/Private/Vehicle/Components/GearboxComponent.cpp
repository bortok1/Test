// Copyright 2023 Teyon. All Rights Reserved.


#include "Vehicle/Components/GearboxComponent.h"
#include "Vehicle/Components/SteeringManager.h"

UGearboxComponent::UGearboxComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UGearboxComponent::BeginPlay()
{
	Super::BeginPlay();

	FGearData Reverse;
	Reverse.MinVelocity = 0;
	Reverse.MaxVelocity = SteeringManager->MaxVelocity / 5;
	
	FGearData Neutral;
	Neutral.MinVelocity = 0;
	Neutral.MaxVelocity = 0;
	
	GearData.Add(Reverse);
	GearData.Add(Neutral);
	
	constexpr int HighestGear = 6;
	for(int Iterator = 1; Iterator <= HighestGear; Iterator++)
	{
		const float MinVelocity = GearData[Iterator].MaxVelocity * 0.9f;
		const float MaxVelocity = SteeringManager->MaxVelocity * Iterator / HighestGear;

		FGearData Drive;
		Drive.MinVelocity = MinVelocity;
		Drive.MaxVelocity = MaxVelocity;
		
		GearData.Add(Drive);
	}
}

float UGearboxComponent::CalculateTorque(const float Velocity)
{
	if(Velocity < 0)
	{
		GearDown();
		Gear = -1;
	}
	else if(Gear < 1)
	{
		GearUp();
		Gear = 1;
	}
	const FGearData ThisGearData = GetGearData();

	const float VelocityPercent = (FMath::Abs(Velocity) - ThisGearData.MinVelocity) / (ThisGearData.MaxVelocity - ThisGearData.MinVelocity);
	
	return SteeringManager->GearTorqueCurve->GetFloatValue(VelocityPercent);
}

void UGearboxComponent::GearUp()
{
	if(Gear == 6)
		return;

	Gear++;
}

void UGearboxComponent::GearDown()
{
	if(Gear <= 0)
		return;
	
	Gear--;
}

FGearData& UGearboxComponent::GetGearData()
{
	return GearData[Gear + 1];
}

void UGearboxComponent::SetSteeringManager(USteeringManager* Manager)
{
	SteeringManager = Manager;
}

