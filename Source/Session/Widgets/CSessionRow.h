#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CSessionRow.generated.h"

UCLASS()
class SESSION_API UCSessionRow : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void PostCreated(class UCMainMenu* InParentWidget, uint32 InIndex);

private:
	UFUNCTION()
		void OnClicked();

public:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SessionName;

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* RowButton;

private:
	class UCMainMenu* ParentWidget;
	uint32 SelfIndex;
};
