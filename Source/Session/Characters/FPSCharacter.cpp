#include "FPSCharacter.h"
#include "Global.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Actors/CBullet.h"
#include "Game/FPSGameMode.h"
#include "Game/CPlayerState.h"
#include "Engine/TargetPoint.h"

AFPSCharacter::AFPSCharacter()
{
	// Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->SetRelativeLocation(FVector(0, 0, 64.f));
	Camera->bUsePawnControlRotation = true;

	// Character Mesh
	FP_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	FP_Mesh->SetOnlyOwnerSee(true);
	FP_Mesh->SetupAttachment(Camera);
	FP_Mesh->bCastDynamicShadow = false;
	FP_Mesh->CastShadow = false;
	FP_Mesh->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
	FP_Mesh->SetRelativeRotation(FRotator(1.9f, -19.2f, 5.2f));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> fp_MeshAsset(TEXT("/Game/FirstPerson/Character/Mesh/SK_Mannequin_Arms"));
	if (fp_MeshAsset.Succeeded())
		FP_Mesh->SetSkeletalMesh(fp_MeshAsset.Object);

	ConstructorHelpers::FClassFinder<UAnimInstance> fp_AnimClass(TEXT("/Game/FirstPerson/Animations/FirstPerson_AnimBP"));
	if (fp_AnimClass.Succeeded())
		FP_Mesh->SetAnimInstanceClass(fp_AnimClass.Class);

	// Gun Mesh
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->SetupAttachment(FP_Mesh, TEXT("GripPoint"));


	// Character Mesh
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetOwnerNoSee(true);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tp_MeshAsset(TEXT("SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (tp_MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(tp_MeshAsset.Object);

	ConstructorHelpers::FClassFinder<UAnimInstance> tp_AnimClass(TEXT("/Game/AnimStarterPack/UE4ASP_HeroTPP_AnimBlueprint"));
	if (tp_AnimClass.Succeeded())
		GetMesh()->SetAnimInstanceClass(tp_AnimClass.Class);

	// Gun Mesh
	TP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TP_Gun"));
	TP_Gun->SetOwnerNoSee(true);
	TP_Gun->SetupAttachment(GetMesh(), "hand_r");
	TP_Gun->SetRelativeLocation(FVector(-9.8f, 5, 0));
	TP_Gun->SetRelativeRotation(FRotator(0, 90, 0));

	// Gun Asset
	ConstructorHelpers::FObjectFinder<USkeletalMesh> gunAsset(TEXT("/Game/FirstPerson/FPWeapon/Mesh/SK_FPGun"));
	if (gunAsset.Succeeded())
	{
		TP_Gun->SetSkeletalMesh(gunAsset.Object);
		FP_Gun->SetSkeletalMesh(gunAsset.Object);
	}

	// GunShot Particle
	CHelpers::CreateSceneComponent(this, &FP_GunShotParticle, "FP_GunShotParticle", FP_Gun);
	FP_GunShotParticle->SetupAttachment(FP_Gun, "Muzzle");
	FP_GunShotParticle->bAutoActivate = false;
	FP_GunShotParticle->SetOnlyOwnerSee(true);

	CHelpers::CreateSceneComponent(this, &TP_GunShotParticle, "TP_GunShotParticle", TP_Gun);
	TP_GunShotParticle->SetupAttachment(TP_Gun, "Muzzle");
	TP_GunShotParticle->bAutoActivate = false;
	TP_GunShotParticle->SetOwnerNoSee(true);

	
	//Properties
	GetCapsuleComponent()->InitCapsuleSize(44.f, 88.0f);

	BaseTurnRate = 90.f;
	BaseLookUpRate = 90.f;

	WeaponRange = 5000.0f;
	WeaponDamage = 10.0f;
}

ACPlayerState* AFPSCharacter::GetSelfPlayerState()
{
	if (SelfPlayerState == nullptr)
		SelfPlayerState = Cast<ACPlayerState>(GetPlayerState());

	return SelfPlayerState;
}

void AFPSCharacter::SetSelfPlayerState(ACPlayerState* NewState)
{
	CheckFalse(HasAuthority());

	SetPlayerState(NewState);
	SelfPlayerState = NewState;
}

void AFPSCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::OnFire);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFPSCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFPSCharacter::LookUpAtRate);
}

void AFPSCharacter::Respawn()
{
	CheckFalse(HasAuthority());

	AFPSGameMode* gameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	CheckNull(gameMode);

	gameMode->Respawn(this);

	Destroy(true);
}

void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() == false)
	{
		SetTeamColor(CurrentTeam);
	}


}

void AFPSCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	SelfPlayerState = Cast<ACPlayerState>(GetPlayerState());

	if(!!SelfPlayerState)
		SelfPlayerState->Health = 100.f;
}

void AFPSCharacter::OnFire()
{
	CheckTrue(GetSelfPlayerState()->Health <= 0);

	if (FP_FireAnimation != NULL)
	{
		UAnimInstance* AnimInstance = FP_Mesh->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FP_FireAnimation, 1.f);
		}
	}

	if (!!FP_GunShotParticle)
		FP_GunShotParticle->Activate(true);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	FVector ShootDir = FVector::ZeroVector;
	FVector StartTrace = FVector::ZeroVector;

	if (PlayerController)
	{
		FRotator CamRot;
		PlayerController->GetPlayerViewPoint(StartTrace, CamRot);
		ShootDir = CamRot.Vector();

		StartTrace = StartTrace + ShootDir * ((GetActorLocation() - StartTrace) | ShootDir);
	}

	const FVector EndTrace = StartTrace + ShootDir * WeaponRange;

	OnServerFire(StartTrace, EndTrace);
}

void AFPSCharacter::OnServerFire_Implementation(const FVector& InLineStart, const FVector& InLineEnd)
{
	WeaponTrace(InLineStart, InLineEnd);
	NetMulticast_ShootEffects();
}


void AFPSCharacter::NetMulticast_ShootEffects_Implementation()
{
	if (!!TP_FireAnimation)
	{
		UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
		if (!!animInstance)
		{
			animInstance->Montage_Play(TP_FireAnimation);
		}
	}

	if (!!FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	if (!!TP_GunShotParticle)
		TP_GunShotParticle->Activate(true);

	if (!!BulletClass)
		GetWorld()->SpawnActor<ACBullet>(BulletClass, TP_Gun->GetSocketLocation("Muzzle"), TP_Gun->GetSocketRotation("Muzzle"));
}

void AFPSCharacter::PlayDead_Implementation()
{
	FP_Mesh->SetVisibility(false);
	FP_Gun->SetVisibility(false);

	//GetMesh()->SetVisibility(false);
	//TP_Gun->SetVisibility(false);

	GetMesh()->SetCollisionProfileName("Spectator");
	GetMesh()->SetEnablePhysicsBlending(1.f);
	GetMesh()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AFPSCharacter::PlayDamage_Implementation()
{
	if (!!TP_HittedAnimation)
	{
		UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
		if (!!animInstance)
		{
			animInstance->Montage_Play(TP_HittedAnimation);
		}
	}

}

void AFPSCharacter::SetTeamColor_Implementation(ETeamType InTeamType)
{
	FLinearColor color;

	if (InTeamType == ETeamType::Red)
		color = FLinearColor::Red;
	else
		color = FLinearColor::Blue;

	if (DynamicMaterial == nullptr)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), nullptr);
		DynamicMaterial->SetVectorParameterValue("BodyColor", color);

		FP_Mesh->SetMaterial(0, DynamicMaterial);
		GetMesh()->SetMaterial(0, DynamicMaterial);
	}
}

void AFPSCharacter::ForceRotation_Implementation(FRotator NewRotation)
{
	SetActorRotation(NewRotation);

	if (!!GetController())
		GetController()->SetControlRotation(NewRotation);
}

void AFPSCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFPSCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFPSCharacter::TurnAtRate(float Rate)
{
	CheckNull(SelfPlayerState);
	CheckTrue(SelfPlayerState->Health <= 0);

	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFPSCharacter::LookUpAtRate(float Rate)
{
	CheckNull(SelfPlayerState);
	CheckTrue(SelfPlayerState->Health <= 0);

	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

FHitResult AFPSCharacter::WeaponTrace(const FVector& StartTrace, const FVector& EndTrace)
{
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), true, GetInstigator());
	TraceParams.bReturnPhysicalMaterial = true;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_Pawn, TraceParams);

	CheckFalseResult(Hit.IsValidBlockingHit(), Hit);

	AFPSCharacter* other = Cast<AFPSCharacter>(Hit.GetActor());
	if (other != nullptr &&
		other->CurrentTeam != this->CurrentTeam) // 살아 있을 때
	{
		FDamageEvent damageEvent;
		other->TakeDamage(WeaponDamage, damageEvent, GetController(), this);
	}

	return Hit;
}

float AFPSCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CheckTrueResult(DamageCauser == this, DamageAmount);
	SelfPlayerState->Health -= DamageAmount;

	// Dead
	if (SelfPlayerState->Health <= 0)
	{
		PlayDead();

		SelfPlayerState->Death++;

		AFPSCharacter* other = Cast<AFPSCharacter>(DamageCauser);
		if (!!other)
			other->SelfPlayerState->Score += 1.f;

		FTimerHandle handle;
		GetWorldTimerManager().SetTimer(handle, this, &AFPSCharacter::Respawn, 3.f, false);

		return DamageAmount;
	}

	// Hitted
	PlayDamage();

	return DamageAmount;
}

void AFPSCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSCharacter, CurrentTeam);
}