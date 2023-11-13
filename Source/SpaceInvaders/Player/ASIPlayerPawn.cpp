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
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMeshComp);
	StaticMeshComp->SetGenerateOverlapEvents(false);
	StaticMeshComp->SetNotifyRigidBodyCollision(false);
	StaticMeshComp->CanCharacterStepUpOn = ECB_No;
	StaticMeshComp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	
	// Collision Presets NOTES:
	// Custom Preset
	// Collision Enabled (Query Only)
	// Object Type: Pawn
	// Ignore everything, Block WorldStatic and WorldDynamic (Invaders)

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
	
	AddInputMapping();
	SpawnProjectile();
}

void AASIPlayerPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	OnPlayerPawnKilled.Clear();
}

void AASIPlayerPawn::SpawnProjectile()
{
	Projectile = GetWorld()->SpawnActor<AASIBaseProjectile>(ProjectileClass,
		ProjectileSpawnPoint->GetComponentLocation(),
		ProjectileSpawnPoint->GetComponentRotation());

	Projectile->SetOwner(this);

	Projectile->OnProjectileHit.AddDynamic(this, &ThisClass::OnProjectileHit);

	bProjectileAvailable = true;
}

void AASIPlayerPawn::OnProjectileHit()
{
	bProjectileAvailable = true;
}

void AASIPlayerPawn::AddInputMapping()
{
	check(PlayerController);

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = nullptr;
	InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	check(InputSubsystem);

	if (InputSubsystem->HasMappingContext(PlayerPawnInputMappingContext))
		return;

	// NOTE: Input Priorities should be ideally defined in other place
	// The first to consume Input should be always the Pawn (1) if exists, then the PlayerController (0)
	InputSubsystem->AddMappingContext(PlayerPawnInputMappingContext, 1);
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
}

void AASIPlayerPawn::FireInputTriggered(const FInputActionValue& Value)
{
	if (Value.GetValueType() != EInputActionValueType::Boolean)
		return;

	Fire();
}

void AASIPlayerPawn::ActivatePowerUpInputTriggered(const FInputActionValue& Value)
{
	if (Value.GetValueType() != EInputActionValueType::Boolean)
		return;

	ActivatePowerUp();
}

void AASIPlayerPawn::Move(float DeltaTime)
{
	FVector LocalOffset(CachedMovementInputValue * MovementSpeed * DeltaTime, 0.0, 0.0);
	AddActorLocalOffset(LocalOffset, true);
}

void AASIPlayerPawn::Fire()
{
	if (!bProjectileAvailable)
		return;

	Projectile->SetActorLocationAndRotation(ProjectileSpawnPoint->GetComponentLocation(),
		ProjectileSpawnPoint->GetComponentRotation(), false, nullptr, ETeleportType::TeleportPhysics);

	Projectile->Fire();

	bProjectileAvailable = false;
}

void AASIPlayerPawn::ActivatePowerUp()
{
	UE_LOG(LogTemp, Warning, TEXT("ActivatePowerUp!"));

	// TODO
}

void AASIPlayerPawn::HandleDestruction(AActor* DestroyerActor)
{
	// TODO: VFXs
	//UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	
	// TODO: Sounds
	//UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());

	AASIBaseProjectile* ProjectileActor = Cast<AASIBaseProjectile>(DestroyerActor);
	if (IsValid(ProjectileActor) && OnPlayerPawnKilled.IsBound())
	{
		OnPlayerPawnKilled.Broadcast();
	}

	Destroy(false, true);
}
