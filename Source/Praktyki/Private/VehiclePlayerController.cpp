// Copyright 2023 Teyon. All Rights Reserved.


#include "VehiclePlayerController.h"
#include "EnhancedInputSubsystems.h"

void AVehiclePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
    
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}
