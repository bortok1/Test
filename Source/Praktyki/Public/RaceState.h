// Copyright 2023 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RaceState.generated.h"

UCLASS()
class PRAKTYKI_API ARaceState : public AGameStateBase
{
	GENERATED_BODY()
    
public:
	ARaceState();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ElapsedRaceTime = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int AllLapsToDo = 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int LapsDone = 0;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	float BestLapTime = INT_MAX;

	void InvalidateRace();
	
	void StartRaceTimer();
	void Lap();
	void StopRaceTimer();

	UFUNCTION(BlueprintImplementableEvent)
	void EndRaceEvent();
	
	FTimerHandle RaceTimerHandle;
	
private:
	UPROPERTY()
	bool IsRaceInvalid = false;

	UPROPERTY()
	float RaceStartTime = 0;

	UPROPERTY()
	float LapStartTime = 0;
	
	void UpdateRaceTime();
};
