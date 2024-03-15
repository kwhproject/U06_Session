#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Widgets/IMenuInterface.h"
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
		void Join(const FString& InAddress) override;

	UFUNCTION(BlueprintCallable, Exec)
		void LoadMenu();

	UFUNCTION(BlueprintCallable, Exec)
		void LoadGameMenu();

	void TravelToMainMenu() override;


private:
	TSubclassOf<class UUserWidget> MenuWidgetClass;
	TSubclassOf<class UUserWidget> GameWidgetClass;

	class UCMainMenu* MenuWidget;
	class UCGameMenu* GameWidget;

};
