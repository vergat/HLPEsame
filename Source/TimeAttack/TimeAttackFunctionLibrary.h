#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "TimeAttackFunctionLibrary.generated.h"

UCLASS()
class TIMEATTACK_API UTimeAttackFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
		
public:

	/** Query functionalities */
	static UProperty* RetrieveProperty(UObject* Object, FString Path, UObject*& OutTargetObject);
};