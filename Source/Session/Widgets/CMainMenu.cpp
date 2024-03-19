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

	if (JoinMenu_Confirm == nullptr) return false;
	JoinMenu_Confirm->OnClicked.AddDynamic(this, &UCMainMenu::JoinServer);

	if (QuitButton == nullptr) return false;
	QuitButton->OnClicked.AddDynamic(this, &UCMainMenu::QuitGame);

	UE_LOG(LogTemp, Error, TEXT("OwingGameInstance"));
	

	return true;
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

void UCMainMenu::JoinServer()
{
	if (OwingGameInstance == nullptr) return;
	if (IPAddressField == nullptr) return;

	const FString& address = IPAddressField->GetText().ToString();
	OwingGameInstance->Join(address);


	
}

void UCMainMenu::QuitGame()
{
	UWorld* world = GetWorld();
	if (world == nullptr) return;
	APlayerController* controller = world->GetFirstPlayerController();

	if (controller == nullptr) return;
	controller->ConsoleCommand("Quit");
}
