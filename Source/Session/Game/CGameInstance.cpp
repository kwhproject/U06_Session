#include "CGameInstance.h"
#include "Blueprint/UserWidget.h"

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
	UEngine* engine = GetEngine();
	if (engine == nullptr) return;

	engine->AddOnScreenDebugMessage(-1, 2, FColor::Green, TEXT("Host"), true, FVector2D(2));

	UWorld* world = GetWorld();
	if (world == nullptr) return;

	world->ServerTravel("/Game/Maps/Play?listen");
	
}

void UCGameInstance::Join(const FString& InAddress)
{
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
	UUserWidget* menuWidget = CreateWidget<UUserWidget>(this, MenuWidgetClass);

	if (menuWidget == nullptr) return;
	menuWidget->AddToViewport();
	menuWidget->bIsFocusable = true;
	
	FInputModeUIOnly inputMode;
	inputMode.SetWidgetToFocus(menuWidget->TakeWidget());
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	APlayerController* controller = GetFirstLocalPlayerController();
	if (controller == nullptr) return;
	controller->SetInputMode(inputMode);
	controller->bShowMouseCursor = true;
}
