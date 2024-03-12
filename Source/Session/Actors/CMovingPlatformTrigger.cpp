#include "CMovingPlatformTrigger.h"
#include "Components/BoxComponent.h"
#include "CMovingPlatform.h"

ACMovingPlatformTrigger::ACMovingPlatformTrigger()
{
	Box = CreateDefaultSubobject<UBoxComponent>(FName("Box"));

	if(!!Box)
		RootComponent = Box;

	Box->bHiddenInGame = false;
}

void ACMovingPlatformTrigger::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ACMovingPlatformTrigger::OnBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ACMovingPlatformTrigger::OnEndOverlap);
}

void ACMovingPlatformTrigger::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	for (const auto& platform : Platforms)
		platform->AddActiveTrigger();

}

void ACMovingPlatformTrigger::OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	for (const auto& platform : Platforms)
		platform->RemoveActiveTrigger();
}

