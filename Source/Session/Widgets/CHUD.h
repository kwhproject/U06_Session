#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CHUD.generated.h"

UCLASS()
class SESSION_API UCHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetHealthText(FString InText);
	void SetDeathText(FString InText);
	void SetScoreText(FString InText);

private:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Health;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Death;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Score;
};
