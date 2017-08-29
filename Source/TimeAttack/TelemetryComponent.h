#pragma once

#include "Components/ActorComponent.h"
#include "TelemetryComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TIMEATTACK_API UTelemetryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UTelemetryComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// BUSINESS LOGIC

	void DrawTelemetry(UCanvas* Canvas, float& YL, float& YPos);

private: 

	// INTERNALS

	void DrawGraph(const FString& Label, const FString& InfoLabel, UCanvas* Canvas, const FLinearColor& BackgroundColor, const FLinearColor& Color, float GraphX, float GraphY, float GraphWidth, float GraphHeight, const TArray<float>& Values, const FVector2D& Range, float CurValue, float& OutX, float& OutY);
};