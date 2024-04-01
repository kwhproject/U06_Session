#include "CHUD.h"
#include "Global.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

bool UCHUD::Initialize()
{
	bool Success = Super::Initialize();
	CheckFalseResult(Success, false);

	CheckNullResult(DeadImage, false);
	DeadImage->SetVisibility(ESlateVisibility::Hidden);

	return true;
}

void UCHUD::SetHealthText(FString InText)
{
	CheckNull(Health);
	Health->SetText(FText::FromString(InText));
}

void UCHUD::SetDeathText(FString InText)
{
	CheckNull(Death);
	Death->SetText(FText::FromString(InText));
}

void UCHUD::SetScoreText(FString InText)
{
	CheckNull(Score);
	Score->SetText(FText::FromString(InText));
}

void UCHUD::SetVisibleDeadImage(bool InVisible)
{
	InVisible ?
		DeadImage->SetVisibility(ESlateVisibility::Visible) :
		DeadImage->SetVisibility(ESlateVisibility::Hidden);
}

