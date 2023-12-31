// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceInvaders/Interfaces/Destructible.h"
#include "ASIBaseProjectile.generated.h"

class UProjectileMovementComponent;
class UParticleSystem;
class UParticleSystemComponent;
class USoundBase;
class UCameraShakeBase;
class AASIBaseProjectile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProjectileHit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProjectileReady, AASIBaseProjectile*, Projectile);

// TODO: TYPO double A
UCLASS()
class SPACEINVADERS_API AASIBaseProjectile : public AActor, public IDestructible
{
	GENERATED_BODY()
	
public:	
	AASIBaseProjectile();
	void Fire();

	/** Start IDestructible Interface **/
	void HandleDestruction(AActor* DestroyerActor) override final;
	/** End IDestructible Interface **/

public:
	// Delegates
	UPROPERTY(BlueprintAssignable)
	FOnProjectileHit OnProjectileHit;

	UPROPERTY(BlueprintAssignable)
	FOnProjectileReady OnProjectileReady;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			FVector NormalImpulse, const FHitResult& Hit);

	void Enable();
	void Disable();

	UFUNCTION()
	void OnGamePaused(const bool bPaused);

private:
	UPROPERTY(Category = "Component", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SceneComp = nullptr;

	UPROPERTY(Category = "Component", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> MeshComp = nullptr;

	UPROPERTY(Category = "Component", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComp = nullptr;

	//UPROPERTY(VisibleAnywhere, Category = "Projectile Components")
	//UParticleSystemComponent* TrailParticles;
	
	//UPROPERTY(VisibleAnywhere, Category = "Projectile Components")
	//UParticleSystem* HitParticles;

	//UPROPERTY(EditAnywhere, Category = "Projectile Variables")
	//USoundBase* LaunchSound;
	
	//UPROPERTY(EditAnywhere, Category = "Projectile Variables")
	//USoundBase* HitSound;
	
	//UPROPERTY(EditAnywhere, Category = "Projectile Variables")
	//TSubclassOf<UCameraShakeBase> HitCameraShakeClass;

	//UPROPERTY(Category = "Variables", EditAnywhere, meta = (AllowPrivateAccess = true))
	//float InitialSpeed = 0.0f;

	UPROPERTY(Category = "Variables", EditAnywhere, meta = (AllowPrivateAccess = true))
	float Speed = 1000.0f;

	/* Offset from Scene Component to adjust the Mesh Pivot properly, set via BPs */
	UPROPERTY(Category = "Variables", EditAnywhere, meta = (AllowPrivateAccess = true))
	FVector MeshOffset = FVector::Zero();

	UPROPERTY(Category = "Instance", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	bool bEnabled = true;
};
