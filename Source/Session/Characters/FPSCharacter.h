#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Game/CGameState.h"
#include "FPSCharacter.generated.h"

class UInputComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class USoundBase;
class UAnimMontage;
class UParticleSystemComponent;

UCLASS(config=Game)
class AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* FP_Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FP_Gun;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* TP_Gun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(VisibleDefaultsOnly, Category = Particle)
	UParticleSystemComponent* FP_GunShotParticle;

	UPROPERTY(VisibleDefaultsOnly, Category = Particle)
	UParticleSystemComponent* TP_GunShotParticle;

public:
	AFPSCharacter();

	class ACPlayerState* GetSelfPlayerState();
	void SetSelfPlayerState(class ACPlayerState* NewState);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TSubclassOf<class ACBullet> BulletClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UAnimMontage* FP_FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UAnimMontage* TP_FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UAnimMontage* TP_HittedAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float WeaponRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float WeaponDamage;

protected:
	virtual void BeginPlay() override;
	void PossessedBy(AController* NewController);

	void OnFire();

	UFUNCTION(Reliable, Server)
		void OnServerFire(const FVector& InLineStart, const FVector& InLineEnd);
	void OnServerFire_Implementation(const FVector& InLineStart, const FVector& InLineEnd);

	UFUNCTION(NetMulticast, Unreliable)
		void NetMulticast_ShootEffects();
	void NetMulticast_ShootEffects_Implementation();
	
	UFUNCTION(NetMulticast, Unreliable)
		void PlayDead();
	void PlayDead_Implementation();

	UFUNCTION(NetMulticast, Unreliable)
		void PlayDamage();
	void PlayDamage_Implementation();

public:
	UFUNCTION(NetMulticast, Reliable)
		void SetTeamColor(ETeamType InTeamType);
	void SetTeamColor_Implementation(ETeamType InTeamType);

	UFUNCTION(Client, Reliable)
		void ForceRotation(FRotator NewRotation);
	void ForceRotation_Implementation(FRotator NewRotation);

protected:
	void MoveForward(float Val);
	void MoveRight(float Val);

	
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	FHitResult WeaponTrace(const FVector& StartTrace, const FVector& EndTrace);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

public:
	UPROPERTY(Replicated)
		ETeamType CurrentTeam;

private:
	class UMaterialInstanceDynamic* DynamicMaterial;
	class ACPlayerState* SelfPlayerState;
};