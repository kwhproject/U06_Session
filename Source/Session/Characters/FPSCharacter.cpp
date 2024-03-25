#include "FPSCharacter.h"
#include "Global.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"


AFPSCharacter::AFPSCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(44.f, 88.0f);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->SetRelativeLocation(FVector(0, 0, 64.f)); 
	Camera->bUsePawnControlRotation = true;
	
	FP_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	FP_Mesh->SetOnlyOwnerSee(true);			
	FP_Mesh->SetupAttachment(Camera);	
	FP_Mesh->bCastDynamicShadow = false;			
	FP_Mesh->CastShadow = false;
	FP_Mesh->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
	FP_Mesh->SetRelativeRotation(FRotator(1.9f, -19.2f, 5.2f));

	// FP

	ConstructorHelpers::FObjectFinder<USkeletalMesh> fp_meshAsset(TEXT("/Game/FirstPerson/Character/Mesh/SK_Mannequin_Arms"));
	if(fp_meshAsset.Succeeded())
		FP_Mesh->SetSkeletalMesh(fp_meshAsset.Object);

	ConstructorHelpers::FClassFinder<UAnimInstance> fp_AnimClass(TEXT("/Game/FirstPerson/Animations/FirstPerson_AnimBP"));
	if (fp_AnimClass.Succeeded())
		FP_Mesh->SetAnimInstanceClass(fp_AnimClass.Class);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetOwnerNoSee(true);

	// TP
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tp_meshAsset(TEXT("/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin"));
	if (tp_meshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(tp_meshAsset.Object);

	ConstructorHelpers::FClassFinder<UAnimInstance> tp_AnimClass(TEXT("/Game/AnimStarterPack/UE4ASP_HeroTPP_AnimBlueprint"));
	if (tp_AnimClass.Succeeded())
		GetMesh()->SetAnimInstanceClass(tp_AnimClass.Class);

	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			
	FP_Gun->bCastDynamicShadow = false;		
	FP_Gun->CastShadow = false;			
	FP_Gun->SetupAttachment(FP_Mesh, TEXT("GripPoint"));

	TP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TP_Gun"));
	TP_Gun->SetOwnerNoSee(true);
	TP_Gun->SetupAttachment(GetMesh(),"hand_r");
	TP_Gun->SetRelativeLocation(FVector(-9.8f, 5.f, 0.f));
	TP_Gun->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> gunAsset(TEXT("/Game/FirstPerson/FPWeapon/Mesh/SK_FPGun"));
	if (gunAsset.Succeeded())
	{
		TP_Gun->SetSkeletalMesh(gunAsset.Object);
		FP_Gun->SetSkeletalMesh(gunAsset.Object);
	}

	WeaponRange = 5000.0f;
	WeaponDamage = 10.0f;

}

void AFPSCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::OnFire);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFPSCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFPSCharacter::LookUpAtRate);
}



void AFPSCharacter::OnServer_Implementation()
{
	OnNetMulticast();
}

void AFPSCharacter::OnNetMulticast_Implementation()
{
	CLog::Print("OnNetMulticast");
}

void AFPSCharacter::OnFire()
{
	OnServer();

	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	if (FP_FireAnimation != NULL)
	{
		UAnimInstance* AnimInstance = FP_Mesh->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FP_FireAnimation, 1.f);
		}
	}

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

	const FHitResult Impact = WeaponTrace(StartTrace, EndTrace);

	AActor* DamagedActor = Impact.GetActor();
	UPrimitiveComponent* DamagedComponent = Impact.GetComponent();

	if ((DamagedActor != NULL) && (DamagedActor != this) && (DamagedComponent != NULL) && DamagedComponent->IsSimulatingPhysics())
	{
		DamagedComponent->AddImpulseAtLocation(ShootDir * WeaponDamage, Impact.Location);
	}
	
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
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFPSCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

FHitResult AFPSCharacter::WeaponTrace(const FVector& StartTrace, const FVector& EndTrace) const
{
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), true, GetInstigator());
	TraceParams.bReturnPhysicalMaterial = true;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_GameTraceChannel1, TraceParams);

	return Hit;
}