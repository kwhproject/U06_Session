#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IMenuInterface.generated.h"

UINTERFACE(MinimalAPI)
class UIMenuInterface : public UInterface
{
	GENERATED_BODY()
};

class SESSION_API IIMenuInterface
{
	GENERATED_BODY()

public:
	virtual void Host() = 0;
	virtual void Join(uint32 InIndex) = 0;
	virtual void TravelToMainMenu() = 0;
	virtual void ShowJoinableSessionList() = 0;
};
