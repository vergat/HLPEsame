#include "TimeAttack.h"
#include "VehicleSimData.h"

#include "WheeledVehicle.h"
#include "WheeledVehicleMovementComponent.h"

UVehicleSimData::UVehicleSimData()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PostPhysics;
}

void UVehicleSimData::BeginPlay()
{
	Super::BeginPlay();

	// Init structures.

	AWheeledVehicle* WheeledVehicle = Cast<AWheeledVehicle>(GetOwner());

	UWheeledVehicleMovementComponent* MovementComp = WheeledVehicle->GetVehicleMovementComponent();
	if (MovementComp != nullptr)
	{
		int32 WheelsNum = MovementComp->Wheels.Num();
		WheelSimData.AddDefaulted(WheelsNum);
	}
}

void UVehicleSimData::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AWheeledVehicle* WheeledVehicle = Cast<AWheeledVehicle>(GetOwner());

	UWheeledVehicleMovementComponent* MovementComp = WheeledVehicle->GetVehicleMovementComponent();
	if (MovementComp != nullptr)
	{
		UPrimitiveComponent* PrimitiveComponent = MovementComp->UpdatedPrimitive;
		FBodyInstance* BodyInstance = PrimitiveComponent->GetBodyInstance();

		// Update chassis sim data.

		{
			FVector LinearyVelocity = BodyInstance->GetUnrealWorldVelocity() * 0.01f;	// m/s
			FVector AngularVelocity = BodyInstance->GetUnrealWorldAngularVelocity();	// deg/s

			float Speed = LinearyVelocity.Size();										// m/s

			ChassisSimData.LinearVelocity = LinearyVelocity;
			ChassisSimData.AngularVelocity = AngularVelocity;

			ChassisSimData.Speed = Speed;
		}

		// Update wheels sim data.

		int32 WheelsNum = MovementComp->Wheels.Num();

		for (int32 WheelIndex = 0; WheelIndex < WheelsNum; ++WheelIndex)
		{
			UVehicleWheel* Wheel = MovementComp->Wheels[WheelIndex];

			FVector Velocity = Wheel->Velocity * 0.01f;
			float Speed = Velocity.Size();

			float SteerAngle = Wheel->GetSteerAngle();
			float RotationAngle = Wheel->GetRotationAngle();

			float SuspensionOffset = Wheel->GetSuspensionOffset() * 0.01f;

			FWheelSimData& SimData = WheelSimData[WheelIndex];

			SimData.Velocity = Velocity;
			SimData.Speed = Speed;

			SimData.SteerAngle = SteerAngle;
			SimData.RotationAngle = RotationAngle;

			SimData.SuspensionOffset = SuspensionOffset;
		}
	}
}