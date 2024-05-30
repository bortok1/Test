// Copyright 2023 Teyon. All Rights Reserved.


#include "Vehicle/MeshVehiclePawn.h"
#include "Vehicle/Components/GearboxComponent.h"
#include "Vehicle/Components/SteeringManager.h"
#include "Vehicle/Components/WheelComponent.h"


AMeshVehiclePawn::AMeshVehiclePawn()
{
	PrimaryActorTick.bCanEverTick = true;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VehicleSkeletalMesh"));
	MeshComponent->SetStaticMesh(Mesh);
	MeshComponent->SetCollisionProfileName("PhysicsActor");
	MeshComponent->SetSimulatePhysics(true);
	RootComponent = MeshComponent;

	SteeringManager = CreateDefaultSubobject<USteeringManager>(TEXT("SteeringManager"));
	
	TArray<FName> WheelNames = { "WheelFR", "WheelFL", "WheelRR", "WheelRL" };
	
	for (int Iterator = 0; Iterator < WheelNames.Num(); Iterator++)
	{
		UWheelComponent* Wheel = CreateDefaultSubobject<UWheelComponent>(WheelNames[Iterator]);
		WheelComponents.Add(Wheel);
		Wheel->SetupAttachment(MeshComponent);
	}
	
	UGearboxComponent* GearBox = CreateDefaultSubobject<UGearboxComponent>(FName("GearboxComponent"));
	GearBox->SetSteeringManager(SteeringManager);
	SteeringManager->SetGearBox(GearBox);
}
