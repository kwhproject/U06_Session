#include "CMenu.h"

void UCMenu::SetOwingGameInstance(IIMenuInterface* InOwingGameInstance)
{
	OwingGameInstance = InOwingGameInstance;
}

void UCMenu::SetInputGameMode()
{
	RemoveFromParent();
	bIsFocusable = false;

	FInputModeGameOnly inputMode;

	UWorld* world = GetWorld();
	if (world == nullptr) return;

	APlayerController* controller = world->GetFirstPlayerController();
	if (controller == nullptr) return;

	controller->SetInputMode(inputMode);
	controller->bShowMouseCursor = false;
}

void UCMenu::SetInputUIMode()
{
	AddToViewport();
	bIsFocusable = true;

	FInputModeUIOnly inputMode;
	inputMode.SetWidgetToFocus(TakeWidget());
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	UWorld* world = GetWorld();
	if (world == nullptr) return;

	APlayerController* controller = world->GetFirstPlayerController();
	if (controller == nullptr) return;

	controller->SetInputMode(inputMode);
	controller->bShowMouseCursor = true;
}
