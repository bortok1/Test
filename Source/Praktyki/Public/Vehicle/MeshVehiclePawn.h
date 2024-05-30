// Copyright 2023 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Vehicle/VehiclePawn.h"
#include "MeshVehiclePawn.generated.h"

UCLASS()
class PRAKTYKI_API AMeshVehiclePawn : public AVehiclePawn
{
	GENERATED_BODY()

public:
	AMeshVehiclePawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;
};
