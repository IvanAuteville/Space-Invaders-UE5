// Copyright Ivan Ariel Garcia Auteville.


#include "ASIUFOActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UMG/Public/Components/WidgetComponent.h"
#include "SpaceInvaders/GameModes/ASIGameModeBase.h"
#include "SpaceInvaders/Projectiles/ASIBaseProjectile.h"
#include "SpaceInvaders/UI/SIUFOScoreWidget.h"

AASIUFOActor::AASIUFOActor()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetMobility(EComponentMobility::Movable);
	SetRootComponent(StaticMeshComp);

	StaticMeshComp->SetGenerateOverlapEvents(false);
	StaticMeshComp->SetNotifyRigidBodyCollision(false);
	StaticMeshComp->CanCharacterStepUpOn = ECB_No;
	StaticMeshComp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	// Collision Presets NOTES:
	// Custom Preset
	// Collision Enabled (Query Only)
	// Object Type: WorldDynamic
	// Ignore everything, Block WoldDynamic (Projectiles) and WorldStatic (bounds)

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	AudioComp->SetupAttachment(RootComponent);
	AudioComp->SetVolumeMultiplier(1.0f);
	AudioComp->SetPitchMultiplier(1.0f);

	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	WidgetComp->SetupAttachment(GetRootComponent());
	WidgetComp->SetWidgetSpace(EWidgetSpace::World);
	WidgetComp->SetDrawAtDesiredSize(true);
	WidgetComp->SetRelativeRotation(FRotator(0.0, 90.0, 90.0));
	WidgetComp->SetPivot(FVector2D(0.5, 0.25));
}

void AASIUFOActor::BeginPlay()
{
	Super::BeginPlay();

	AASIGameModeBase* MyGameMode = Cast<AASIGameModeBase>(UGameplayStatics::GetGameMode(this));
	MyGameMode->OnGamePaused.AddDynamic(this, &ThisClass::OnGamePaused);

	ScoreWidget = Cast<USIUFOScoreWidget>(WidgetComp->GetWidget());

	AudioComp->Play();
}

void AASIUFOActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bEnabled)
		return;

	Move(DeltaTime);
}

void AASIUFOActor::Move(const float DeltaTime)
{
	const float Direction = (HorizontalMovementType == EHorizontalMovementType::Right) ? 1.0f : -1.0f;
	FVector LocalOffset(Direction * BaseMovementSpeed * DeltaTime, 0.0, 0.0);
	AddActorLocalOffset(LocalOffset, true);
}

void AASIUFOActor::OnGamePaused(const bool bPaused)
{
	SetActorTickEnabled(!bPaused);
	bEnabled = !bPaused;
}

void AASIUFOActor::SetHorizontalMovementDirection(const EHorizontalMovementType MovementDirection)
{
	HorizontalMovementType = MovementDirection;
}

void AASIUFOActor::HandleDestruction(AActor* DestroyerActor)
{
	// TODO: VFX
	//UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	
	// TODO: Sound
	//UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	
	if (OnUFODestroyed.IsBound())
	{
		OnUFODestroyed.Broadcast(DestroyerActor);
	}

	// Did the Player kill us?
	AASIBaseProjectile* Projectile = Cast<AASIBaseProjectile>(DestroyerActor);
	if (IsValid(Projectile))
	{
		bEnabled = false;

		AudioComp->Stop();
		StaticMeshComp->SetHiddenInGame(true);

		ScoreWidget->SetVisibility(ESlateVisibility::Visible);
		
		SetLifeSpan(0.5f);
	}
	else
	{
		Destroy();
	}
}

void AASIUFOActor::SetRandomScore(const int32 UFOScore)
{
	ScoreWidget->SetScore(UFOScore);
}
