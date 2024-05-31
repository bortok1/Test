// Copyright 2023 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GearboxComponent.generated.h"


class USteeringManager;

USTRUCT(BlueprintType)
struct FGearData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GearData")
	float MinVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GearData")
	float MaxVelocity;

	FGearData()
	{
		this->MinVelocity = 0;
		this->MaxVelocity = 0;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRAKTYKI_API UGearboxComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGearboxComponent();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int Gear = 0;

	float CalculateTorque(float Velocity);

private:
	UPROPERTY()
	USteeringManager* SteeringManager = nullptr;

	UPROPERTY(EditAnywhere, Category="GearData")
	TArray<FGearData> GearData;

public:
	void GearUp();

	void GearDown();

	UFUNCTION(BlueprintCallable)
	FGearData& GetGearData();
	
	void SetSteeringManager(USteeringManager* Manager);
};
