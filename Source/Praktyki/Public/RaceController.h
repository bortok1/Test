// Copyright 2023 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RaceController.generated.h"

class ARaceState;

UCLASS()
class PRAKTYKI_API ARaceController : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	ARaceState* GameState = nullptr;
	
public:    
	ARaceController();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* BoxCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UArrowComponent* FrontArrow;

private:
	bool bWrongWay = false;
	
	UPROPERTY()
	int LapsToDo = 1;
	
	UPROPERTY()
	FVector ForwardVector = FVector::ZeroVector;
	
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
						class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void OnEnterFromFront();
	void OnEnterFromBack();
};
