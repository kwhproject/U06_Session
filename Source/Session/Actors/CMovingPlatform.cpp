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

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

void ACMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		FVector currentLocation = GetActorLocation();
		FVector targetWS = GetTransform().TransformPosition(Target);

		FVector direction = (targetWS - currentLocation).GetSafeNormal();

		currentLocation += direction * Speed * DeltaTime;
		SetActorLocation(currentLocation);
	}


	
}
