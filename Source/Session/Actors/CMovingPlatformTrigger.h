#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CMovingPlatformTrigger.generated.h"

UCLASS()
class SESSION_API ACMovingPlatformTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	ACMovingPlatformTrigger();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		void OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* Box;

private:
	UPROPERTY(EditInstanceOnly, Category = "Platforms")
		TArray<class ACMovingPlatform*> Platforms;
};
