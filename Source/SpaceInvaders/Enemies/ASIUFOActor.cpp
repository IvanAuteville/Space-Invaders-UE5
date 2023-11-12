// Copyright Ivan Ariel Garcia Auteville.


#include "ASIUFOActor.h"
//#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

AASIUFOActor::AASIUFOActor()
{
	PrimaryActorTick.bCanEverTick = true;

	//SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	//SceneComp->SetMobility(EComponentMobility::Movable);
	//SetRootComponent(SceneComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetMobility(EComponentMobility::Movable);
	//StaticMeshComp->SetupAttachment(RootComponent);
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
}

void AASIUFOActor::SetHorizontalMovementDirection(const EHorizontalMovementType MovementDirection)
{
	HorizontalMovementType = MovementDirection;
}

void AASIUFOActor::HandleDestruction()
{
	// TODO: VFX
	//UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	
	// TODO: Sound
	//UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());

	Destroy();
}

void AASIUFOActor::BeginPlay()
{
	Super::BeginPlay();

	//StaticMeshComp->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);

	bEnabled = true;

	AudioComp->Play();
}

void AASIUFOActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bEnabled)
		return;
	
	Move(DeltaTime);
}

void AASIUFOActor::Move(float DeltaTime)
{
	const float Direction = (HorizontalMovementType == EHorizontalMovementType::Right) ? 1.0f : -1.0f;
	FVector LocalOffset(Direction * MovementSpeed * DeltaTime, 0.0, 0.0);
	AddActorLocalOffset(LocalOffset, true);
}

//void AASIUFOActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
//	FVector NormalImpulse, const FHitResult& Hit)
//{
//	if (OtherActor == nullptr || OtherActor == this)
//		return;
//
//	// TODO: do something with OtherActor
//	UE_LOG(LogTemp, Warning, TEXT("UFO Collision with %s"), *OtherActor->GetName());
//
//	// TODO: VFX
//	//UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());
//
//	// TODO: Sound
//	//UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
//
//	Disable();
//
//	if (!OnProjectileHit.IsBound())
//		return;
//
//	OnProjectileHit.Broadcast();
//}