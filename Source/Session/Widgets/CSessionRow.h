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
	void SetClicked(bool InClicked);

private:
	UFUNCTION()
		void OnClicked();

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsClicked() { return bClicked; }

public:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SessionName;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* HostUserName;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ConnectionFraction;

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* RowButton;

protected:
	UPROPERTY(BlueprintReadOnly)
		bool bClicked;

private:
	class UCMainMenu* ParentWidget;

	uint32 SelfIndex;
};
