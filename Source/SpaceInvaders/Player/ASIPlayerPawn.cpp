// Copyright Ivan Ariel Garcia Auteville.


#include "ASIPlayerPawn.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/InputComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "SpaceInvaders/Projectiles/ASIBaseProjectile.h"
#include "SpaceInvaders/Player/ASIPlayerController.h"
#include "SpaceInvaders/Utils/InputUtils.h"

AASIPlayerPawn::AASIPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// Init Components
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	SetRootComponent(BoxComp);
	BoxComp->SetGenerateOverlapEvents(true);
	BoxComp->SetNotifyRigidBodyCollision(true);
	BoxComp->CanCharacterStepUpOn = ECB_No;

	// Collision Presets NOTES:
	// Custom Preset
	// Collision Enabled (Query Only)
	// Object Type: Pawn
	// Ignore everything, Block WorldStatic

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(BoxComp);
	StaticMeshComp->SetGenerateOverlapEvents(true);
	StaticMeshComp->SetNotifyRigidBodyCollision(true);
	StaticMeshComp->CanCharacterStepUpOn = ECB_No;
	StaticMeshComp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	// TODO: test OnHit/OnOverlap on the Mesh for more precise collisions with Projectiles

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(StaticMeshComp);

	AutoReceiveInput = EAutoReceiveInput::Player0;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = nullptr;
}

void AASIPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<AASIPlayerController>(GetController());

	check(PlayerController);

	//PlayerController->SetViewTargetWithBlend(this);
	
	AddInputMapping();
}

void AASIPlayerPawn::AddInputMapping()
{
	check(PlayerController);

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = nullptr;
	InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	check(InputSubsystem);

	if (InputSubsystem->HasMappingContext(PlayerPawnInputMappingContext))
		return;

	InputSubsystem->AddMappingContext(PlayerPawnInputMappingContext, 0);
}

void AASIPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	check(InputComp);

	{
		using namespace Utils::InputActionsUtils;

		BindInputStartedTriggeredCompleted(InputComp, MoveInputAction, this,
			&ThisClass::MoveInputTriggered, &ThisClass::MoveInputTriggered, &ThisClass::MoveInputTriggered);

		BindInputStarted(InputComp, FireInputAction, this, &ThisClass::FireInputTriggered);
		
		BindInputStarted(InputComp, ActivatePowerUpInputAction, this, &ThisClass::ActivatePowerUpInputTriggered);
	}
}

void AASIPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bMovementEnabled)
		return;

	Move(DeltaTime);
}

void AASIPlayerPawn::MoveInputTriggered(const FInputActionValue& Value)
{
	if (Value.GetValueType() != EInputActionValueType::Axis1D)
		return;

	CachedMovementInputValue = Value.Get<float>();

	UE_LOG(LogTemp, Warning, TEXT("Moving"));
}

void AASIPlayerPawn::FireInputTriggered(const FInputActionValue& Value)
{
	if (Value.GetValueType() != EInputActionValueType::Boolean)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Firing"));
}

void AASIPlayerPawn::ActivatePowerUpInputTriggered(const FInputActionValue& Value)
{
	if (Value.GetValueType() != EInputActionValueType::Boolean)
		return;

	UE_LOG(LogTemp, Warning, TEXT("PowerUp!"));
}

void AASIPlayerPawn::Move(float DeltaTime)
{
	FVector LocalOffset(CachedMovementInputValue * MovementSpeed * DeltaTime, 0.0, 0.0);
	AddActorLocalOffset(LocalOffset, true);
}

void AASIPlayerPawn::Fire()
{
	//if (!InputEnabled())
	//	return;

	//AASIBaseProjectile* Projectile = GetWorld()->SpawnActor<AASIBaseProjectile>(ProjectileClass,
	//	ProjectileSpawnPoint->GetComponentLocation(),
	//	ProjectileSpawnPoint->GetComponentRotation());
	//Projectile->SetOwner(this);
}

void AASIPlayerPawn::ActivatePowerUp()
{

}

void AASIPlayerPawn::HandleDestruction()
{
	//UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	//UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	//GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
}