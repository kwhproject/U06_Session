#include "CMovingPlatform.h"
#include "Materials/MaterialInstanceConstant.h"

ACMovingPlatform::ACMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	SetMobility(EComponentMobility::Movable);

	ConstructorHelpers::FObjectFinder<UStaticMesh> meshAsset(TEXT("/Game/Geometry/Meshes/1M_Cube"));
	if (meshAsset.Succeeded())
		GetStaticMeshComponent()->SetStaticMesh(meshAsset.Object);

	ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> materialAsset(TEXT("/Game/Geometry/Materials/MAT_MovingPlatform_Red"));
	if (materialAsset.Succeeded())
		GetStaticMeshComponent()->SetMaterial(0, materialAsset.Object);

	GetStaticMeshComponent()->SetWorldScale3D(FVector(1.5f, 1.5f, 0.25f));
}

void ACMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	OriginWS = GetActorLocation();
	TargetWS = GetTransform().TransformPosition(Target);

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}
	
}

void ACMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ActiveTriggers < 1) return;

	if (HasAuthority())
	{
		FVector currentLocation = GetActorLocation();

		float totalDistance = (OriginWS - TargetWS).Size();
		float distanceFromOrigin = (OriginWS - currentLocation).Size();

		if (distanceFromOrigin >= totalDistance)
		{
			FVector temp = OriginWS;
			OriginWS = TargetWS;
			TargetWS = temp;
		}

		FVector direction = (TargetWS - OriginWS).GetSafeNormal();

		currentLocation += direction * Speed * DeltaTime;
		SetActorLocation(currentLocation);
	}


	
}

void ACMovingPlatform::AddActiveTrigger()
{
	ActiveTriggers++;
}

void ACMovingPlatform::RemoveActiveTrigger()
{
	if (ActiveTriggers > 0)
		ActiveTriggers--;
}
