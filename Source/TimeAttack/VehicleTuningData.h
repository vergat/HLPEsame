#pragma once

#include "Engine/DataAsset.h"
#include "VehicleTuningData.generated.h"

UCLASS(BlueprintType)
class TIMEATTACK_API UVehicleTuningData : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = Tuning)
	float AerodynamicDrag = 0.f;
};