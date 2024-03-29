#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FPSHUD.generated.h"

UCLASS()
class AFPSHUD : public AHUD
{
	GENERATED_BODY()

public:
	AFPSHUD();

	virtual void DrawHUD() override;

protected:
	virtual void BeginPlay() override;

private:
	class UTexture2D* CrosshairTex;

	TSubclassOf<class UCHUD> HUDWidgetClass;

	UPROPERTY()
		class UCHUD* HUDWidget;
};

