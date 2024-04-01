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

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	void SpawnHost();
	virtual void BeginPlay() override;

public:
	void Respawn(class AFPSCharacter* InPlayer);

private:
	void MoveToSpawnPoint(class AFPSCharacter* InPlayer);

private:
	TArray<class ACSpawnPoint*> RedTeamSpawnPoints;
	TArray<class ACSpawnPoint*> BlueTeamSpawnPoints;

	TArray<class AFPSCharacter*> RedTeamCharacters;
	TArray<class AFPSCharacter*> BlueTeamCharacters;
	TArray<class AFPSCharacter*> WaitingForSpawnCharacters;
};



