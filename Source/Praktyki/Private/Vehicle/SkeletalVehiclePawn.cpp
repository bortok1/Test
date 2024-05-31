// Copyright 2023 Teyon. All Rights Reserved.


#include "Vehicle/SkeletalVehiclePawn.h"

#include "Vehicle/Components/SteeringManager.h"
#include "Vehicle/Components/VehicleSkeletalMesh.h"
#include "Vehicle/Components/WheelComponent.h"

#define CONVERT_KMH_TO_CMS 27.7778f
#define WHEEL_DISTANCE_PER_DEGREE 0.7f

ASkeletalVehiclePawn::ASkeletalVehiclePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	USkeletalMesh* SkeletalMesh = ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("/Script/Engine.SkeletalMesh'/Game/Vehicles/Porsche_911_GT3_R/SK_Porsche_911_GT3-R.SK_Porsche_911_GT3-R'")).Object;
	SkeletalComponent = CreateDefaultSubobject<UVehicleSkeletalMesh>(TEXT("VehicleSkeletalMesh"));
	RootComponent = SkeletalComponent;
	SkeletalComponent->SetSkeletalMesh(SkeletalMesh);
	SkeletalComponent->SetCollisionProfileName("PhysicsActor");
	SkeletalComponent->SetSimulatePhysics(true);
	
	TArray<FName> WheelNames = { "WheelFR", "WheelFL", "WheelRR", "WheelRL" };
	
	for (int Iterator = 0; Iterator < WheelNames.Num(); Iterator++)
	{
		UWheelComponent* Wheel = CreateDefaultSubobject<UWheelComponent>(WheelNames[Iterator]);
		WheelComponents.Add(Wheel);
		Wheel->SetupAttachment(SkeletalComponent);
	}
	
	SetupCamera();
}

void ASkeletalVehiclePawn::BeginPlay()
{
	Super::BeginPlay();

	SteeringManager->OnSteeringChanged.AddDynamic(this, &ASkeletalVehiclePawn::OnSteeringChangedHandler);
}

void ASkeletalVehiclePawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	// Info on order of Wheels in TArray<FName> WheelNames (27)
	RotateDrivingWheels(SkeletalComponent->FrontRightWheelMeshes, WheelComponents[0], DeltaSeconds, true);
	RotateDrivingWheels(SkeletalComponent->FrontLeftWheelMeshes, WheelComponents[1], DeltaSeconds);
	RotateDrivingWheels(SkeletalComponent->RearRightWheelMeshes, WheelComponents[2], DeltaSeconds, true);
	RotateDrivingWheels(SkeletalComponent->RearLeftWheelMeshes, WheelComponents[3], DeltaSeconds);
}

void ASkeletalVehiclePawn::RotateDrivingWheels(TArray<UStaticMeshComponent*> MeshComponents, const UWheelComponent* WheelComponent, const float DeltaTime, const bool bRight)
{
	for (UStaticMeshComponent* MeshComp : MeshComponents)
	{
		const float Pitch = -WheelComponent->GetWheelForwardVelocity() * CONVERT_KMH_TO_CMS * WHEEL_DISTANCE_PER_DEGREE * DeltaTime * (1 - bRight * 2);
		MeshComp->AddLocalRotation(FRotator(Pitch, 0, 0));
	}
}

void ASkeletalVehiclePawn::OnSteeringChangedHandler(const float Rotation)
{
	{	// Steering wheel
		FRotator MeshRotation = FRotator::ZeroRotator;
		MeshRotation.Roll += SteeringManager->MaxSteeringRotation * 4.f * (Rotation - PreviousRotation);
		SkeletalComponent->SteeringWheel->AddRelativeRotation(MeshRotation);
	}
	
	if(const ESteerType SteerType = SteeringManager->GetSteerType(); SteerType == ESteerType::FrontS)
	{
		RotateSteeringWheels(SkeletalComponent->FrontLeftWheelMeshes, Rotation);
		RotateSteeringWheels(SkeletalComponent->FrontRightWheelMeshes, Rotation);
	}
	else if(SteerType == ESteerType::RearS)
	{
		RotateSteeringWheels(SkeletalComponent->RearLeftWheelMeshes, Rotation, true);
		RotateSteeringWheels(SkeletalComponent->RearRightWheelMeshes, Rotation, true);
	}
	else
	{
		RotateSteeringWheels(SkeletalComponent->FrontLeftWheelMeshes, Rotation);
		RotateSteeringWheels(SkeletalComponent->FrontRightWheelMeshes, Rotation);
		RotateSteeringWheels(SkeletalComponent->RearLeftWheelMeshes, Rotation, true);
		RotateSteeringWheels(SkeletalComponent->RearRightWheelMeshes, Rotation, true);
	}
	PreviousRotation = Rotation;
}

void ASkeletalVehiclePawn::RotateSteeringWheels(TArray<UStaticMeshComponent*> MeshComponents, const float Rotation, const bool bRear) const
{
	for (UStaticMeshComponent* MeshComp : MeshComponents)
	{
		FRotator MeshRotation = MeshComp->GetRelativeRotation();
		MeshRotation.Yaw += SteeringManager->MaxSteeringRotation * (Rotation - PreviousRotation) * (1 - bRear * 2);
		MeshComp->SetRelativeRotation(MeshRotation);
	}
}
