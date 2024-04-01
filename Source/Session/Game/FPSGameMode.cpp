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

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.25;
}

void AFPSGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (WaitingForSpawnCharacters.Num() > 0)
	{
		//Todo. Iterator Error
		for (int32 i = 0; i < WaitingForSpawnCharacters.Num(); i++)
		{
			if (WaitingForSpawnCharacters[i] == nullptr &&
				WaitingForSpawnCharacters[i]->IsPendingKill())
				continue;

			MoveToSpawnPoint(WaitingForSpawnCharacters[i]);
		}

	}
}

void AFPSGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AFPSCharacter* playerPawn = Cast<AFPSCharacter>(NewPlayer->GetPawn());
	ACPlayerState* playerState = Cast<ACPlayerState>(NewPlayer->PlayerState);

	if (!!playerPawn && playerState)
	{
		// Team Contact
		if (RedTeamCharacters.Num() > BlueTeamCharacters.Num())
		{
			BlueTeamCharacters.Add(playerPawn);
			playerState->Team = ETeamType::Blue;

			NewPlayer->StartSpot = FindPlayerStart(NewPlayer, "BlueTeam");
			playerState->SpawnRotation = FRotator(0, 45, 0);
		}
		else
		{
			RedTeamCharacters.Add(playerPawn);
			playerState->Team = ETeamType::Red;

			NewPlayer->StartSpot = FindPlayerStart(NewPlayer, "RedTeam");
			playerState->SpawnRotation = FRotator(0, 90, 0);

			
		}

		// Team Information to Character
		playerPawn->CurrentTeam = playerState->Team;
		playerPawn->SetTeamColor(playerState->Team);

		for (TActorIterator<ACSpawnPoint> iter(GetWorld()); iter; ++iter)
		{
			if (iter->GetTeam() == ETeamType::Red)
				RedTeamSpawnPoints.Add(*iter);
			else
				BlueTeamSpawnPoints.Add(*iter);
		}

		if(playerPawn->IsPawnControlled())
		playerPawn->ForceRotation(playerState->SpawnRotation);

		MoveToSpawnPoint(playerPawn);
		SpawnHost();
	}
}

void AFPSGameMode::SpawnHost()
{
	UWorld* world = GetWorld();
	if (!!world)
	{
		APlayerController* hostController = world->GetFirstPlayerController();
		if (!!hostController)
		{
			APawn* hostPawn = hostController->GetPawn();
			AFPSCharacter* hostCharacter = Cast<AFPSCharacter>(hostPawn);

			if (!!hostCharacter)
				MoveToSpawnPoint(hostCharacter);
		}
	}
}

void AFPSGameMode::BeginPlay()
{
	Super::BeginPlay();

}

void AFPSGameMode::Respawn(AFPSCharacter* InPlayer)
{
	AController* controller = InPlayer->GetController();
	
	// Detach Controller
	InPlayer->DetachFromControllerPendingDestroy();

	// Respawn Character
	AFPSCharacter* player = Cast<AFPSCharacter>(GetWorld()->SpawnActor(DefaultPawnClass));
	CheckNull(player);

	// RE-Possess
	controller->Possess(player);

	// Move to SpawnPoint
	ACPlayerState* playerState = Cast<ACPlayerState>(controller->PlayerState);
	if (!!playerState)
	{
		player->CurrentTeam = playerState->Team;
		player->SetTeamColor(playerState->Team);

		if (player->IsPawnControlled())
			player->ForceRotation(playerState->SpawnRotation);

		MoveToSpawnPoint(player);
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
		// SpawnPoint & Character Overlap Check
		point->UpdateOverlaps();

		// SpawnPoint is not blocked
		if (point->IsBlocked() == false)
		{
			InPlayer->SetActorLocation(point->GetActorLocation());


			if (WaitingForSpawnCharacters.Find(InPlayer) >= 0)
			WaitingForSpawnCharacters.Remove(InPlayer);

			return;
		}
	}
	
	// SpawnPoint is blocked
	if (WaitingForSpawnCharacters.Find(InPlayer) < 0)
		WaitingForSpawnCharacters.Add(InPlayer);
}
