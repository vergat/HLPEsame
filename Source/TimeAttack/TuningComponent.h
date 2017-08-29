#pragma once

#include "Components/ActorComponent.h"
#include "TuningComponent.generated.h"

/**
 * Tuning modifier: modifies a specific property.
 */
USTRUCT(Blueprintable, BlueprintType)
struct TIMEATTACK_API FTuningModifier
{
	GENERATED_USTRUCT_BODY()

	FTuningModifier()
		: MinMultiplier(1.f)
		, MaxMultiplier(1.f)
		, InitialValue(0.f)
		, InitialValueValid(false)
	{

	}

	/** Member path (i.e. [PawnMemberName] or [ComponentName].[ComponentMemberName] */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tuning)
	FString	Member;

	/** Multiplier applied to original value for modifier = 0 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tuning)
	float	MinMultiplier;

	/** Multiplier applied to original value for modifier = 1 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tuning)
	float	MaxMultiplier;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Tuning)
	float	InitialValue;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Tuning)
	bool	InitialValueValid;
};

/**
 * Tuning modifier: modifies a specific property.
 */
USTRUCT(Blueprintable, BlueprintType)
struct TIMEATTACK_API FTuningEntry
{
	GENERATED_USTRUCT_BODY()

	FTuningEntry()
		: Value(0.f)
	{

	}

	// Entry description

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tuning)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, AdvancedDisplay, Category = Tuning)
	TArray<FTuningModifier>	Modifiers;

	// Entry value

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tuning)
	float Value;

	// BUSINESS LOGIC

	void Apply(AActor* Target);
};

UCLASS(Blueprintable, BlueprintType, ClassGroup=(VehicleTuning), meta=(BlueprintSpawnableComponent))
class TIMEATTACK_API UTuningComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPROPERTY(EditAnywhere, Category = Tuning)
	TArray<FTuningEntry>	Entries;

	// CTOR

	UTuningComponent();

	// BUSINESS LOGIC

	/** Assign settings from an blueprint object. Property names are used to match corresponding settings. */
	UFUNCTION(BlueprintCallable, Category = Tuning)
	void AssignSettings(UObject* Settings);

	/** Apply settings to the vehicle. */
	UFUNCTION(BlueprintCallable, Category = Tuning)
	void ApplySettings();

	UFUNCTION(BlueprintCallable, Category = Tuning)
	void SetValue(FName Name, float NewValue);
};