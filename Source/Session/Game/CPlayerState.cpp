#include "CPlayerState.h"
#include "Net/UnrealNetwork.h"

ACPlayerState::ACPlayerState(const FObjectInitializer& ObjectInitializer)
	: Health(100)
	, Death(0)
	, Team(ETeamType::Red)
	, SpawnRotation(FRotator::ZeroRotator)
{

}

void ACPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPlayerState, Health);
	DOREPLIFETIME(ACPlayerState, Death);
	DOREPLIFETIME(ACPlayerState, Team);
	DOREPLIFETIME(ACPlayerState, SpawnRotation);
}