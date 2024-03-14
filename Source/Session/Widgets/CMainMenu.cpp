#include "CMainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

bool UCMainMenu::Initialize()
{
	bool bSuccess = Super::Initialize();
	if (bSuccess == false) return false;

	if (HostButton == nullptr) return false;
	HostButton->OnClicked.AddDynamic(this, &UCMainMenu::HostServer);

	if (JoinButton == nullptr) return false;
	JoinButton->OnClicked.AddDynamic(this, &UCMainMenu::OpenJoinMenu);

	if (JoinMenu_Back == nullptr) return false;
	JoinMenu_Back->OnClicked.AddDynamic(this, &UCMainMenu::OpenMainMenu);

	UE_LOG(LogTemp, Error, TEXT("OwingGameInstance"));

	return true;
}

void UCMainMenu::SetOwingGameInstance(IIMenuInterface* InOwingGameInstance)
{
	OwingGameInstance = InOwingGameInstance;
}

void UCMainMenu::SetInputGameMode()
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

void UCMainMenu::SetInputUIMode()
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

void UCMainMenu::HostServer()
{
	if (!!OwingGameInstance)
	OwingGameInstance->Host();
}

void UCMainMenu::OpenJoinMenu()
{
	if (MenuSwitcher == nullptr) return;
	if (JoinMenu == nullptr) return;

	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UCMainMenu::OpenMainMenu()
{
	if (MenuSwitcher == nullptr) return;
	if (MainMenu == nullptr) return;

	MenuSwitcher->SetActiveWidget(MainMenu);
}
