// Copyright 2023 Teyon. All Rights Reserved.


#include "RaceState.h"

ARaceState::ARaceState()
{
	RaceStartTime = 0.0f;
	ElapsedRaceTime = 0.0f;
}

void ARaceState::InvalidateRace()
{
	if(IsRaceInvalid)
		return;

	IsRaceInvalid = true;
	GetWorld()->GetTimerManager().ClearTimer(RaceTimerHandle);
}

void ARaceState::StartRaceTimer()
{
	RaceStartTime = GetWorld()->GetTimeSeconds();
	LapStartTime = RaceStartTime;
	
	GetWorld()->GetTimerManager().SetTimer(RaceTimerHandle, this, &ARaceState::UpdateRaceTime, 0.01f, true);
}

void ARaceState::Lap()
{
	if(IsRaceInvalid)
		return;
	
	const float LapTime = GetWorld()->GetTimeSeconds() - LapStartTime;
	if(BestLapTime >= LapTime)
	{
		BestLapTime = LapTime;
	}
	LapsDone++;
}

void ARaceState::StopRaceTimer()
{
	if(IsRaceInvalid)
		return;
	
	GetWorld()->GetTimerManager().ClearTimer(RaceTimerHandle);

	ElapsedRaceTime = GetWorld()->GetTimeSeconds() - RaceStartTime;
	
	Lap();
	
	EndRaceEvent();
}

void ARaceState::UpdateRaceTime()
{
	ElapsedRaceTime = GetWorld()->GetTimeSeconds() - RaceStartTime;
}
