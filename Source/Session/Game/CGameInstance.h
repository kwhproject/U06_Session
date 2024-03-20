#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Widgets/IMenuInterface.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "CGameInstance.generated.h"

UCLASS()
class SESSION_API UCGameInstance : public UGameInstance, public IIMenuInterface
{
	GENERATED_BODY()
	
public:
	UCGameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;

public:
	UFUNCTION(Exec)
		void Host() override;	

	UFUNCTION(Exec)
		void Join(uint32 InIndex) override;

	UFUNCTION(BlueprintCallable, Exec)
		void LoadMenu();

	UFUNCTION(BlueprintCallable, Exec)
		void LoadGameMenu();

	void TravelToMainMenu() override;
	void ShowJoinableSessionList() override;

private:
	void CreateSession();

	void OnCreateSessionCompleted(FName InSessionName, bool InSuccess);
	void OnDestroySessionCompleted(FName InSessionName, bool InSuccess);
	void OnFindSessionCompleted(bool InSuccess);
	void OnJoinSessionCompleted(FName InSessionName, EOnJoinSessionCompleteResult::Type InResult);

private:
	TSubclassOf<class UUserWidget> MenuWidgetClass;
	TSubclassOf<class UUserWidget> GameWidgetClass;

	class UCMainMenu* MenuWidget;
	class UCGameMenu* GameWidget;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
};
