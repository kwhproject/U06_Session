#pragma once

#include "CoreMinimal.h"
#include "IMenuInterface.h"
#include "CMenu.h"
#include "CMainMenu.generated.h"

USTRUCT()
struct FSessionData
{
	GENERATED_BODY()

public:
	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUserName;
};

UCLASS()
class SESSION_API UCMainMenu : public UCMenu
{
	GENERATED_BODY()
	
public:
	UCMainMenu(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
		void HostServer();

	UFUNCTION()
		void OpenJoinMenu();

	UFUNCTION()
		void OpenMainMenu();

	UFUNCTION()
		void JoinServer();

	UFUNCTION()
		void QuitGame();

public:
	void SetSessionList(TArray<FSessionData> InSessionDatas);
	void SetSelectedSessionIndex(uint32 InIndex);

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* JoinMenu_Back;

	UPROPERTY(meta = (BindWidget))
		class UButton* JoinMenu_Confirm;

	UPROPERTY(meta = (BindWidget))
		class UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
		class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
		class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
		class UPanelWidget* SessionList;

private:
	TSubclassOf<class UUserWidget> SessionRowClass;
	TOptional<uint32> SelectedSessionIndex;

};
