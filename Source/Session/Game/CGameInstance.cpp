#include "CGameInstance.h"
#include "Widgets/CMainMenu.h"

UCGameInstance::UCGameInstance(const FObjectInitializer& ObjectInitializer)
{
	UE_LOG(LogTemp, Error, TEXT("GameInstance Constructor"));

	ConstructorHelpers::FClassFinder<UUserWidget> menuWidgetClass(TEXT("/Game/Widgets/WB_MainMenu"));
	
	if(menuWidgetClass.Succeeded())
		MenuWidgetClass = menuWidgetClass.Class;
}

void UCGameInstance::Init()
{
	UE_LOG(LogTemp, Error, TEXT("GameInstance Init"));

	UE_LOG(LogTemp, Warning, TEXT("Menu Widget Class Name is %s"), *MenuWidgetClass->GetName());
}

void UCGameInstance::Host()
{
	if(!!MenuWidget)
		MenuWidget->SetInputGameMode();

	UEngine* engine = GetEngine();
	if (engine == nullptr) return;
	engine->AddOnScreenDebugMessage(-1, 2, FColor::Green, TEXT("Host"), true, FVector2D(2));

	UWorld* world = GetWorld();
	if (world == nullptr) return;
	world->ServerTravel("/Game/Maps/Play?listen");
	
}

void UCGameInstance::Join(const FString& InAddress)
{
	if (!!MenuWidget)
		MenuWidget->SetInputGameMode();

	UEngine* engine = GetEngine();
	if (engine == nullptr) return;

	engine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Join to %s"), *InAddress), true, FVector2D(2));
	
	APlayerController* controller = GetFirstLocalPlayerController();
	if (controller == nullptr) return;
	controller->ClientTravel(InAddress, ETravelType::TRAVEL_Absolute);
}

void UCGameInstance::LoadMenu()
{
	if (MenuWidgetClass == nullptr) return;
	MenuWidget = CreateWidget<UCMainMenu>(this, MenuWidgetClass);

	if (MenuWidget == nullptr) return;
	MenuWidget->SetInputUIMode();

	MenuWidget->SetOwingGameInstance(this);
}
