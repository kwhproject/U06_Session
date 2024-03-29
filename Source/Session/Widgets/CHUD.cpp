#include "CHUD.h"
#include "Components/TextBlock.h"

void UCHUD::SetHealthText(FString InText)
{
	Health->SetText(FText::FromString(InText));
}

void UCHUD::SetDeathText(FString InText)
{
	Death->SetText(FText::FromString(InText));
}

void UCHUD::SetScoreText(FString InText)
{
	Score->SetText(FText::FromString(InText));
}

