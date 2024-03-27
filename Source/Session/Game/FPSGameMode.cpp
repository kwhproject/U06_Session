#include "FPSGameMode.h"
#include "Global.h"
#include "FPSHUD.h"
#include "Characters/FPSCharacter.h"
#include "Engine/TargetPoint.h"
#include "UObject/ConstructorHelpers.h"
#include "Actors/CSpawnPoint.h"
#include "CGameState.h"
#include "CPlayerState.h"
#include "EngineUtils.h"

AFPSGameMode::AFPSGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Player/BP_CPlayer"));
	
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	HUDClass = AFPSHUD::StaticClass();
	GameStateClass = ACGameState::StaticClass();
	PlayerStateClass = ACPlayerState::StaticClass();
}

void AFPSGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AFPSCharacter* playerPawn = Cast<AFPSCharacter>(NewPlayer->GetPawn());
	ACPlayerState* playerState = Cast<ACPlayerState>(NewPlayer->PlayerState);

	if (!!playerPawn && playerState)
	{
		// Set PlayerState to Character from Controller
		playerPawn->SetPlayerState(playerState);

		// Team Contact
		if (RedTeamCharacters.Num() > BlueTeamCharacters.Num())
		{
			BlueTeamCharacters.Add(playerPawn);
			playerState->Team = ETeamType::Blue;
		}
		else
		{
			RedTeamCharacters.Add(playerPawn);
			playerState->Team = ETeamType::Red;
		}

		// Team Information to Character
		playerPawn->CurrentTeam = playerState->Team;
		playerPawn->SetTeamColor(playerState->Team);

		MoveToSpawnPoint(playerPawn);
	}
}

void AFPSGameMode::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ACSpawnPoint> iter(GetWorld()); iter; ++iter)
	{
		if (iter->GetTeam() == ETeamType::Red)
			RedTeamSpawnPoints.Add(*iter);
		else
			BlueTeamSpawnPoints.Add(*iter);
	}

}

void AFPSGameMode::MoveToSpawnPoint(AFPSCharacter* InPlayer)
{
	TArray<AActor*> centerPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), centerPoints);
	
	TArray<ACSpawnPoint*>* targetPoints;

	if (InPlayer->CurrentTeam == ETeamType::Red)
		targetPoints = &RedTeamSpawnPoints;
	else
		targetPoints = &BlueTeamSpawnPoints;

	for (const auto& point : *targetPoints)
	{
		// SpawnPoint is not blocked
		if (point->IsBlocked() == false)
		{
			// InPlayer->SetActorLocation(point->GetActorLocation());
			FRotator rotation = FRotator::ZeroRotator;
			if (!!centerPoints[0])
			{
				rotation = UKismetMathLibrary::FindLookAtRotation
				(
					InPlayer->GetActorLocation(),
					centerPoints[0]->GetActorLocation()
				);
				//InPlayer->SetActorRotation(rotation);
			}
				
			FTransform transform;
			transform.SetLocation(point->GetActorLocation());
			transform.SetRotation(FQuat(rotation));
			InPlayer->SetActorTransform(transform);

			point->UpdateOverlaps();

			return;
		}
	}

	// SpawnPoint is blocked
}
