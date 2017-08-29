#pragma once

#include "Components/ActorComponent.h"
#include "VehicleSimData.generated.h"

USTRUCT()
struct FChassisSimData
{
	GENERATED_USTRUCT_BODY()

	FChassisSimData()
		: LinearVelocity(FVector::ZeroVector)
		, AngularVelocity(FVector::ZeroVector)
		, Speed(0.f)
	{

	}

	UPROPERTY(Transient)
	FVector LinearVelocity;
	UPROPERTY(Transient)
	FVector AngularVelocity;

	UPROPERTY(Transient)
	float Speed;
};

USTRUCT()
struct FWheelSimData
{
	GENERATED_USTRUCT_BODY()

	FWheelSimData()
		: SteerAngle(0.f)
		, RotationAngle(0.f)
		, SuspensionOffset(0.f)
	{

	}

	UPROPERTY(Transient)
	FVector Velocity;
	UPROPERTY(Transient)
	float Speed;

	UPROPERTY(Transient)
	float SteerAngle;
	UPROPERTY(Transient)
	float RotationAngle;

	UPROPERTY(Transient)
	float SuspensionOffset;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TIMEATTACK_API UVehicleSimData : public UActorComponent
{
	GENERATED_BODY()

public:	

	UVehicleSimData();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	UPROPERTY(Transient)
	FChassisSimData ChassisSimData;
	UPROPERTY(Transient)
	TArray<FWheelSimData> WheelSimData;
};