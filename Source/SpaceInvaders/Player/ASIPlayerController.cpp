// Copyright Ivan Ariel Garcia Auteville.


#include "ASIPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "Kismet/GameplayStatics.h"
#include "SpaceInvaders/Utils/InputUtils.h"

AASIPlayerController::AASIPlayerController()
{
	bShowMouseCursor = false;

	bEnableClickEvents = false;
	bEnableMouseOverEvents = false;

	bEnableTouchEvents = false;
	bEnableTouchOverEvents = false;
	
	bGamePaused = false;
}

void AASIPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(InputComponent);
	check(InputComp);

	{
		using namespace Utils::InputActionsUtils;

		BindInputStarted(InputComp, SelectInputAction, this, &ThisClass::SelectInputTriggered);
		BindInputStarted(InputComp, PauseGameInputAction, this, &ThisClass::PauseGameInputTriggered);
	}
}

void AASIPlayerController::BeginPlay()
{
	SetInputModeGameOnly();
	AddInputMapping();
}

void AASIPlayerController::SetInputModeGameAndUI()
{
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	InputMode.SetHideCursorDuringCapture(false);

	SetInputMode(InputMode);
}

void AASIPlayerController::SetInputModeGameOnly()
{
	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(true);

	SetInputMode(InputMode);
}

void AASIPlayerController::AddInputMapping()
{
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = nullptr;
	InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	check(InputSubsystem);

	if (InputSubsystem->HasMappingContext(PlayerInputMappingContext))
		return;

	// NOTE: Input Priorities should be ideally defined in other place
	// The first to consume Input should be always the Pawn (1) if exists, then the PlayerController (0)
	InputSubsystem->AddMappingContext(PlayerInputMappingContext, 0);
}

void AASIPlayerController::SelectInputTriggered(const FInputActionValue& Value)
{
	if (Value.GetValueType() != EInputActionValueType::Boolean)
		return;

	const UWorld* CurrentWorld = GetWorld();
	const UWorld* GameWorld = GamePlayLevel.Get();

	if (CurrentWorld == GameWorld)
		return;

	UGameplayStatics::OpenLevelBySoftObjectPtr(CurrentWorld, GamePlayLevel, true);
}

void AASIPlayerController::PauseGameInputTriggered(const FInputActionValue& Value)
{
	if (Value.GetValueType() != EInputActionValueType::Boolean)
		return;

	bGamePaused = !bGamePaused;
	UGameplayStatics::SetGamePaused(GetWorld(), bGamePaused);
}