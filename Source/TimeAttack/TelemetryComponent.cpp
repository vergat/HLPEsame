#include "TimeAttack.h"
#include "TelemetryComponent.h"

#include "RenderUtils.h"
#include "Engine.h"

#include "Engine/Canvas.h"
#include "Engine/Font.h"

#include "CanvasItem.h"
#include "CanvasTypes.h"

#pragma optimize ("", off)

static TAutoConsoleVariable<FString> CVarTelemetry(
	TEXT("Telemetry"),
	FString(),
	TEXT("Current telemetry values.\n"),
	ECVF_Cheat);

UTelemetryComponent::UTelemetryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTelemetryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTelemetryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTelemetryComponent::DrawTelemetry(UCanvas* Canvas, float& YL, float& YPos)
{
	static const float GraphWidth = 200.0f; 
	static const float GraphHeight = 100.0f;
	
	FString TelemetryList = CVarTelemetry.GetValueOnAnyThread(); // Example: "Property0;Property1;Property2"
	
	if (TelemetryList.Len() == 0) // Check for a valid telemetry list.
		return;

	// If you've entered a new telemetry list, clear previous values.

	static FVector2D Range(0.f, 0.01f);
	static int32 CurrentGraphPos = 0;
	static FString LastTelemetryList;

	if (!TelemetryList.Equals(LastTelemetryList))
	{
		LastTelemetryList = TelemetryList;
		Range = FVector2D(0.f, 0.01f);
		CurrentGraphPos = 0;
	}

	// Parse TelemetryList and get a list of properties to plot.

	TArray<FString> TelemetryArray;
	TelemetryList.ParseIntoArray(TelemetryArray, TEXT(";"));

	TArray<float> TelemetryValues;

	for (auto& Telemetry : TelemetryArray)
	{
		float Value = 0.f;

		// TODO: Get the correct property's value using reflection.

		// Support the following queries:
		
		// 1) VehicleSimData.ChassisSimData.Speed

		// 2) VehicleSimData.WheelSimData[i].Speed
		// 3) VehicleSimData.WheelSimData[i].SteerAngle
		// 4) VehicleSimData.WheelSimData[i].RotationAngle
		// 5) VehicleSimData.WheelSimData[i].SuspensionOffset

		// HINT: See UIgnitionGameplayFunctionLibrary::RetrieveProperty.

		{
			Value = FMath::RandRange(0.f, 1.f);
		}

		TelemetryValues.Add(Value);
	}

	// Adjust graph ranges.

	for (auto Value : TelemetryValues)
	{
		if (Value < Range.X)
		{
			Range.X = Value;
		}
		else if (Value > Range.Y)
		{
			Range.Y = Value;
		}
	}

	// Draw graph values, for each graph.

	static TArray<TArray<float>> GraphValues;

	float SavedYPos = YPos;
	float CurX = 4;

	int32 TelemetryIdx = 0;
	
	for (auto Value : TelemetryValues)
	{
		if (GraphValues.Num() <= TelemetryIdx)
		{
			GraphValues.SetNum(TelemetryIdx + 1);
		}
		
		GraphValues[TelemetryIdx].SetNum(128);
		GraphValues[TelemetryIdx][CurrentGraphPos] = Value;

		float OutX = GraphWidth;
		YPos = SavedYPos;
		
		FString FinalLabel = FString::Printf(TEXT(" %0.3f [%0.3f %0.3f]"), Value, Range.X, Range.Y);
		DrawGraph(TelemetryArray[TelemetryIdx], FinalLabel, Canvas, FLinearColor(0.0f, 0.125f, 0.0f, 0.25f), FLinearColor(1.0f, 0.5f, 0.0f, 1.0f), CurX, YPos, GraphWidth, GraphHeight, GraphValues[TelemetryIdx], Range, static_cast<float>(CurrentGraphPos) / GraphValues[TelemetryIdx].Num(), OutX, YPos);
		
		CurX += GraphWidth + 10.f;

		++TelemetryIdx;
	}

	if (GraphValues.Num() > 0)
	{
		CurrentGraphPos = (CurrentGraphPos + 1) % GraphValues[0].Num();
	}
}

void UTelemetryComponent::DrawGraph(const FString& Label, const FString& InfoLabel, UCanvas* Canvas, const FLinearColor& BackgroundColor, const FLinearColor& Color, float GraphX, float GraphY, float GraphWidth, float GraphHeight, const TArray<float>& Values, const FVector2D& Range, float CurValue, float& OutX, float& OutY)
{
	Canvas->SetDrawColor(FColor(255, 255, 0));
	
	UFont* Font = GEngine->GetSmallFont();

	// Draw label.

	float XL, YL;

	{
		Canvas->TextSize(Font, Label, XL, YL);

		float ScaleX = 1.f;
		if (XL > GraphWidth)
		{
			ScaleX = GraphWidth / XL;
		}

		Canvas->DrawText(Font, Label, GraphX, GraphY, ScaleX);
	}

	// Spacing.

	{
		GraphY += YL + 4.0f;
	}

	// Draw info.

	{
		Canvas->TextSize(Font, InfoLabel, XL, YL);

		float ScaleX = 1.f;
		if (XL > GraphWidth)
		{
			ScaleX = GraphWidth / XL;
		}

		Canvas->DrawText(Font, InfoLabel, GraphX, GraphY, ScaleX);
	}

	// Spacing.

	{
		GraphY += YL + 4.0f;
	}

	float LineGraphHeight = GraphHeight - YL - 4.0f;

	// Draw the background.

	{
		FVector2D InPosition(GraphX, GraphY);
		FVector2D InSize(GraphWidth, LineGraphHeight);

		FCanvasTileItem TileItem(InPosition, GWhiteTexture, InSize, BackgroundColor);
		TileItem.BlendMode = SE_BLEND_Translucent;
	
		Canvas->DrawItem(TileItem);
	}

	// Update draw color.

	{
		Canvas->SetDrawColor(FColor(0, 0, 32, 92));
	}

	// Draw zero reference.

	{
		float Y = FMath::GetMappedRangeValueClamped(Range, FVector2D(1.f, 0.f), 0.f);

		FVector2D InPosition(GraphX, GraphY + Y * LineGraphHeight);
		FVector2D InEndPosition(GraphX + GraphWidth, GraphY + Y * LineGraphHeight);

		FCanvasLineItem ZeroReferenceLine(InPosition, InEndPosition);
		ZeroReferenceLine.SetColor(FLinearColor(.25f, .25f, .25f, .25f));
		ZeroReferenceLine.Draw(Canvas->Canvas);
	}

	// Update draw color.

	{
		Canvas->SetDrawColor(FColor(0, 32, 0, 128));
	}

	for (int32 Index = 1; Index < Values.Num(); ++Index)
	{
		float X1 = static_cast<float>(Index - 1) / Values.Num();
		float Y1 = FMath::GetMappedRangeValueClamped(Range, FVector2D(1.f, 0.f), Values[Index - 1]);
		float X2 = static_cast<float>(Index) / Values.Num();
		float Y2 = FMath::GetMappedRangeValueClamped(Range, FVector2D(1.f, 0.f), Values[Index]);

		// Draw graph segment.

		{
			FVector2D InPosition(GraphX + X1 * GraphWidth, GraphY + Y1 * LineGraphHeight);
			FVector2D InEndPosition(GraphX + X2 * GraphWidth, GraphY + Y2 * LineGraphHeight);

			FCanvasLineItem LineItem(InPosition, InEndPosition);
			LineItem.SetColor(Color);
			LineItem.Draw(Canvas->Canvas);
		}

		// Draw reference line.

		if (CurValue >= X1 && (CurValue < X2 || Index == Values.Num() - 1))
		{
			FVector2D InPosition(GraphX + X1 * GraphWidth, GraphY + Y1 * LineGraphHeight);
			FVector2D InEndPosition(GraphX + X1 * GraphWidth, GraphY + LineGraphHeight);

			FCanvasLineItem CurrentPositionLine(InPosition, InEndPosition);
			CurrentPositionLine.SetColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
			CurrentPositionLine.Draw(Canvas->Canvas);
		}
	}

	OutX = FMath::Max(XL, GraphWidth);
	OutY = GraphY + GraphHeight;
}