#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IMenuInterface.h"
#include "CMainMenu.generated.h"

UCLASS()
class SESSION_API UCMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;

public:
	void SetOwingGameInstance(IIMenuInterface* InOwingGameInstance);
	void SetInputGameMode();
	void SetInputUIMode();

private:
	UFUNCTION()
		void HostServer();

	UFUNCTION()
		void OpenJoinMenu();

	UFUNCTION()
		void OpenMainMenu();

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
		class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
		class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
		class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* IPAddressField;

private:
	IIMenuInterface* OwingGameInstance;
};
