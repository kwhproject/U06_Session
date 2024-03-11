#include "SessionGameMode.h"
#include "Characters/SessionCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASessionGameMode::ASessionGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
