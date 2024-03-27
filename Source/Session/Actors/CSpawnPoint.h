#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/CGameState.h"
#include "CSpawnPoint.generated.h"

UCLASS()
class SESSION_API ACSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ACSpawnPoint();

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	virtual void Tick(float DeltaTime) override;

	bool IsBlocked(){ return OverlappedActors.Num() > 0; }
	ETeamType GetTeam() { return Team; }
private:
	UFUNCTION()
		void ActorBeginOverlap(AActor* OverlapActor, AActor* OtherActor);

	UFUNCTION()
		void ActorEndOverlap(AActor* OverlapActor, AActor* OtherActor);

private:
	UPROPERTY(EditInstanceOnly)
		ETeamType Team;

	UPROPERTY(EditInstanceOnly)
		bool bHiddenInGame;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCapsuleComponent* Capsule;

	TArray<AActor*> OverlappedActors;
};
