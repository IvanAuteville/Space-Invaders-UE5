// Copyright Ivan Ariel Garcia Auteville.


#include "ASIPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "Kismet/GameplayStatics.h"

//#include "CombatSystem/Interfaces/Selectable.h"

#include "SpaceInvaders/Utils/InputUtils.h"

AASIPlayerController::AASIPlayerController()
{
	bShowMouseCursor = true;

	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	bEnableTouchEvents = false;
	bEnableTouchOverEvents = false;

	DefaultMouseCursor = EMouseCursor::Hand;
	DefaultClickTraceChannel = ECollisionChannel::ECC_Visibility;
	HitResultTraceDistance = 1000000.f;
	
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
	SetInputModeGameAndUI();
	AddInputMapping();
}

void AASIPlayerController::SetInputModeGameAndUI()
{
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	InputMode.SetHideCursorDuringCapture(false);
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

	UE_LOG(LogTemp, Warning, TEXT("Select Input Triggered!"));
}

void AASIPlayerController::PauseGameInputTriggered(const FInputActionValue& Value)
{
	if (Value.GetValueType() != EInputActionValueType::Boolean)
		return;

	bGamePaused = !bGamePaused;
	UGameplayStatics::SetGamePaused(GetWorld(), bGamePaused);
}