// Copyright 2023 Teyon. All Rights Reserved.


#include "Vehicle/Components/VehicleSkeletalMesh.h"
#include "Vehicle/VehicleDataAsset.h"

UVehicleSkeletalMesh::UVehicleSkeletalMesh()
{
	if(GetOwner() == nullptr || GetOwner()->GetName().Contains("Default"))
		return;
	
	VehicleDataAsset = ConstructorHelpers::FObjectFinder<UVehicleDataAsset>(TEXT("/Script/Praktyki.VehicleDataAsset'/Game/Vehicles/Porsche_911_GT3_R/DA_Porshe.DA_Porshe'")).Object;
	
	if(VehicleDataAsset == nullptr)
		return;

	VehicleDataAsset->FillArray();
	
	if(VehicleDataAsset->VehicleParts.IsEmpty())
		return;
	
	VehicleDataAsset->Interior;
	UStaticMeshComponent* SMInteriorComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Interior"));
	SMInteriorComponent->SetupAttachment(this, VehicleDataAsset->Interior.BoneName);
	SMInteriorComponent->SetStaticMesh(VehicleDataAsset->Interior.StaticMesh);

	int Iterator = 0;
	for (const FVehiclePart Part : VehicleDataAsset->VehicleParts)
	{
		Iterator++;
		if(Part.StaticMesh == nullptr || Part.BoneName == "None")
			continue;

		UStaticMeshComponent* SMComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName(Part.StaticMesh->GetName(), Iterator));
		SMComponent->SetStaticMesh(Part.StaticMesh);

		if(Part.bRotate)
			SMComponent->SetRelativeRotation(FRotator(0, 180, 0));

		if(Part.IsInterior)
			SMComponent->SetupAttachment(SMInteriorComponent, Part.BoneName);
		else
			SMComponent->SetupAttachment(this, Part.BoneName);
	}
}

void UVehicleSkeletalMesh::BeginPlay()
{
	Super::BeginPlay();

	FrontLeftWheelMeshes.Reserve(4);
	FrontRightWheelMeshes.Reserve(4);
	RearLeftWheelMeshes.Reserve(4);
	RearRightWheelMeshes.Reserve(4);

	const UStaticMeshComponent* Interior = nullptr;
	
	for (USceneComponent* Child : GetAttachChildren())
	{
		FString BoneName = Child->GetAttachSocketName().ToString();
		FString ChildName = Child->GetName();

		if(!Child->IsA(UStaticMeshComponent::StaticClass()))
			continue;
		
		if(Interior == nullptr && ChildName.Contains("Interior"))
			Interior = Cast<UStaticMeshComponent>(Child);
		
		if(!BoneName.Contains("suspension"))
			continue;

		if(BoneName == "suspension_front_left")
		{
			FrontLeftWheelMeshes.Add(Cast<UStaticMeshComponent>(Child));
			if (ChildName.Contains("Brake_Disc") || ChildName.Contains("Rear_Wheel") || ChildName.Contains("Front_Wheel"))
				DriveFrontLeftWheelMeshes.Add(Cast<UStaticMeshComponent>(Child));
		}
		else if(BoneName == "suspension_front_right")
		{
			FrontRightWheelMeshes.Add(Cast<UStaticMeshComponent>(Child));
			if (ChildName.Contains("Brake_Disc") || ChildName.Contains("Rear_Wheel") || ChildName.Contains("Front_Wheel"))
				DriveFrontRightWheelMeshes.Add(Cast<UStaticMeshComponent>(Child));
		}
		else if(BoneName == "suspension_back_left")
		{
			RearLeftWheelMeshes.Add(Cast<UStaticMeshComponent>(Child));
			if (ChildName.Contains("Brake_Disc") || ChildName.Contains("Rear_Wheel") || ChildName.Contains("Front_Wheel"))
				DriveRearLeftWheelMeshes.Add(Cast<UStaticMeshComponent>(Child));
		}
		else if(BoneName == "suspension_back_right")
		{
			RearRightWheelMeshes.Add(Cast<UStaticMeshComponent>(Child));
			if (ChildName.Contains("Brake_Disc") || ChildName.Contains("Rear_Wheel") || ChildName.Contains("Front_Wheel"))
				DriveRearRightWheelMeshes.Add(Cast<UStaticMeshComponent>(Child));
		}
	}

	for (USceneComponent* Child : Interior->GetAttachChildren())
	{
		FString BoneName = Child->GetName();

		if(!BoneName.Contains("Steering_Wheel") || !Child->IsA(UStaticMeshComponent::StaticClass()))
			continue;

		SteeringWheel = Cast<UStaticMeshComponent>(Child);
		break;
	}
}
