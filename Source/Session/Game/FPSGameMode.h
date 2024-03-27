#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameMode.generated.h"

UCLASS(minimalapi)
class AFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFPSGameMode();

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void BeginPlay() override;

private:
	void MoveToSpawnPoint(class AFPSCharacter* InPlayer);

private:
	TArray<class ACSpawnPoint*> RedTeamSpawnPoints;
	TArray<class ACSpawnPoint*> BlueTeamSpawnPoints;

	TArray<class AFPSCharacter*> RedTeamCharacters;
	TArray<class AFPSCharacter*> BlueTeamCharacters;
};



