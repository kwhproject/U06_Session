#include "CMainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "CSessionRow.h"

UCMainMenu::UCMainMenu(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> sessionRowClassAsset(TEXT("/Game/Widgets/WB_SessionRow"));
	if (sessionRowClassAsset.Succeeded())
		SessionRowClass = sessionRowClassAsset.Class;
}

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

	if(!!OwingGameInstance)
		OwingGameInstance->ShowJoinableSessionList();
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
	

	if (SelectedSessionIndex.IsSet())
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Session Index is %d"), SelectedSessionIndex.GetValue());
		OwingGameInstance->Join(SelectedSessionIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Session Index is not set"));
	}

}

void UCMainMenu::QuitGame()
{
	UWorld* world = GetWorld();
	if (world == nullptr) return;
	APlayerController* controller = world->GetFirstPlayerController();

	if (controller == nullptr) return;
	controller->ConsoleCommand("Quit");
}

void UCMainMenu::SetSessionList(TArray<FSessionData> InSessionDatas)
{
	UWorld* world = GetWorld();
	if (world == nullptr) return;

	SessionList->ClearChildren();

	uint32 i = 0;
	for (const auto& sessionData : InSessionDatas)
	{
		UCSessionRow* row = CreateWidget<UCSessionRow>(world, SessionRowClass);
		if (row == nullptr) return;
		row->SessionName->SetText(FText::FromString(sessionData.Name));
		row->HostUserName->SetText(FText::FromString(sessionData.HostUserName));
		FString fractionStr = FString::Printf(TEXT("%d/%d"), sessionData.CurrentPlayers, sessionData.MaxPlayers);
		row->ConnectionFraction->SetText(FText::FromString(fractionStr));

		row->PostCreated(this, i++);

		if (SessionList == nullptr) return;
		SessionList->AddChild(row);
	}
		

}

void UCMainMenu::SetSelectedSessionIndex(uint32 InIndex)
{
	SelectedSessionIndex = InIndex;

	for (int i = 0; i < SessionList->GetChildrenCount(); i++)
	{
		UCSessionRow* row = Cast<UCSessionRow>(SessionList->GetChildAt(i));
		if (!!row)
		{
			row->SetClicked
			(
				(SelectedSessionIndex.IsSet()) && 
				(i == SelectedSessionIndex.GetValue())
			);
		}
	}
}
