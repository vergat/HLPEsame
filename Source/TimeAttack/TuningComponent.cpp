#include "TimeAttack.h"
#include "TuningComponent.h"

#include "TimeAttackFunctionLibrary.h"

#pragma optimize ("", off)

// FTuningEntry

void FTuningEntry::Apply(AActor* Target)
{
	for (auto& Modifier : Modifiers)
	{
		UObject* Object = nullptr;
		UProperty* Property = UTimeAttackFunctionLibrary::RetrieveProperty(Target, Modifier.Member, Object);

		// If found, apply modifier.

		if (Property != nullptr)
		{
			UFloatProperty* FloatProp = Cast<UFloatProperty>(Property);
			UBoolProperty* BoolProp = FloatProp == nullptr ? Cast<UBoolProperty>(Property) : nullptr;

			if (!Modifier.InitialValueValid)
			{
				if (FloatProp != nullptr)
				{
					Modifier.InitialValue = FloatProp->GetPropertyValue_InContainer(Object);
				}
				else
				{
					if (BoolProp != nullptr)
					{
						Modifier.InitialValue = BoolProp->GetPropertyValue_InContainer(Object) ? 1.f : 0.f;
					}
				}

				Modifier.InitialValueValid = true;
			}

			float NewValue = Modifier.InitialValue;
			NewValue *= FMath::Lerp(Modifier.MinMultiplier, Modifier.MaxMultiplier, Value);

			if (FloatProp != nullptr)
			{
				FloatProp->SetPropertyValue_InContainer(Object, NewValue);
			}
			else
			{
				if (BoolProp != nullptr)
				{
					BoolProp->SetPropertyValue_InContainer(Object, NewValue > 0.5f);
				}
			}
		}
	}
}

// UTuningComponent

UTuningComponent::UTuningComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTuningComponent::AssignSettings(UObject* Settings)
{
	if (Settings == nullptr)
	{
		return;
	}

	for (auto& Entry : Entries)
	{
		UFloatProperty* FloatProp = Cast<UFloatProperty>(Settings->GetClass()->FindPropertyByName(Entry.Name));
		if (FloatProp != nullptr)
		{
			Entry.Value = FloatProp->GetPropertyValue_InContainer(Settings);
		}
	}
}

void UTuningComponent::ApplySettings()
{
	for (auto& Entry : Entries)
	{
		Entry.Apply(GetOwner());
	}

	// TODO: Re-initialize vehicle.
}

void UTuningComponent::SetValue(FName Name, float NewValue)
{
	for (auto& Entry : Entries)
	{
		if (Entry.Name == Name)
		{
			Entry.Value = NewValue;
		}
	}
}