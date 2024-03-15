#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IMenuInterface.h"
#include "CMenu.generated.h"

UCLASS()
class SESSION_API UCMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetOwingGameInstance(IIMenuInterface* InOwingGameInstance);

	void SetInputGameMode();
	void SetInputUIMode();

protected:
	IIMenuInterface* OwingGameInstance;
};
