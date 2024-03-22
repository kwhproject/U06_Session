#pragma once

#include "CoreMinimal.h"
#include "Game/SessionGameMode.h"
#include "CLobbyGameMode.generated.h"

UCLASS()
class SESSION_API ACLobbyGameMode : public ASessionGameMode
{
	GENERATED_BODY()
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

private:
	void StartGame();

private:
	uint32 NumberOfPlayers;
	FTimerHandle GameStartHandle;
};
