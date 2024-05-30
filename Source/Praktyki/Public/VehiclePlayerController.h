// Copyright 2023 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VehiclePlayerController.generated.h"

class UInputMappingContext;

UCLASS()
class PRAKTYKI_API AVehiclePlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* InputMappingContext;
	
public:
	virtual void SetupInputComponent() override;
};
