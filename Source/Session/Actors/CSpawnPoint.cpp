#include "CSpawnPoint.h"
#include "Global.h"
#include "Components/CapsuleComponent.h"

ACSpawnPoint::ACSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateSceneComponent(this, &Capsule, "Capsule");
}

void ACSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
	Capsule->bHiddenInGame = bHiddenInGame;
	OnActorBeginOverlap.AddDynamic(this, &ACSpawnPoint::ActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ACSpawnPoint::ActorEndOverlap);
}

void ACSpawnPoint::OnConstruction(const FTransform& Transform)
{
	if (Team == ETeamType::Red)
		Capsule->ShapeColor = FColor::Red;
	else
		Capsule->ShapeColor = FColor::Blue;
}

void ACSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Capsule->UpdateOverlaps();
}

void ACSpawnPoint::ActorBeginOverlap(AActor* OverlapActor, AActor* OtherActor)
{
	if (HasAuthority())
	{
		if (OverlappedActors.Find(OtherActor) < 0)
			OverlappedActors.Add(OtherActor);
	}
}

void ACSpawnPoint::ActorEndOverlap(AActor* OverlapActor, AActor* OtherActor)
{
	if (HasAuthority())
	{
		if (OverlappedActors.Find(OtherActor) >= 0)
			OverlappedActors.Remove(OtherActor);
	}
}

