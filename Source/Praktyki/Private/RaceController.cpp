// Copyright 2023 Teyon. All Rights Reserved.


#include "RaceController.h"
#include "RaceState.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Vehicle/VehiclePawn.h"

ARaceController::ARaceController()
{
	PrimaryActorTick.bCanEverTick = false;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(RootComponent);

	FrontArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("RaceDirection"));
	FrontArrow->SetupAttachment(RootComponent);

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ARaceController::OnOverlapBegin);
}

void ARaceController::BeginPlay()
{
	Super::BeginPlay();

	GameState = Cast<ARaceState>(GetWorld()->GetGameState());

	ForwardVector = GetActorForwardVector();
	GameState->StartRaceTimer();

	LapsToDo = GameState->LapsToDoOverall;
}

void ARaceController::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                     class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && Cast<AVehiclePawn>(OtherActor) && OtherComp)
	{
		const FVector OtherActorLocation = OtherActor->GetActorLocation();
		FVector ToOtherActor = OtherActorLocation - GetActorLocation();
		ToOtherActor.Normalize();

		if (FVector::DotProduct(ForwardVector, ToOtherActor) > 0.0f)
		{
			OnEnterFromFront();
		}
		else
		{
			OnEnterFromBack();
		}
	}
}

void ARaceController::OnEnterFromFront()
{
	WrongWayCounter++; // bad
}

void ARaceController::OnEnterFromBack()
{
	WrongWayCounter--;
	if(WrongWayCounter > 0)
		return;

	LapsToDo--;
	if(LapsToDo > 0)
		GameState->Lap();
	else
		GameState->StopRaceTimer();
}
