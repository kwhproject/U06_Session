#include "CGameMenu.h"
#include "Components/Button.h"

bool UCGameMenu::Initialize()
{
	bool bSuccess = Super::Initialize();
	if (bSuccess == false) return false;

	if (CancelButton == nullptr) return false;
	CancelButton->OnClicked.AddDynamic(this, &UCGameMenu::CancelButtonPressed);

	if (QuitButton == nullptr) return false;
	QuitButton->OnClicked.AddDynamic(this, &UCGameMenu::QuitButtonPressed);

	return true;
}

void UCGameMenu::CancelButtonPressed()
{
	SetInputGameMode();
}

void UCGameMenu::QuitButtonPressed()
{
	if (OwingGameInstance == nullptr) return;

	SetInputGameMode();
	OwingGameInstance->TravelToMainMenu();
}
