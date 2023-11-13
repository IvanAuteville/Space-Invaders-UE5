// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ASIPlayerPawn.generated.h"

class USceneComponent;
class UStaticMeshComponent;

class UInputComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

class AASIBaseProjectile;
class AASIPlayerController;

//class UNiagaraSystem;

UCLASS(meta = (PrioritizeCategories = "PlayerPawnSettings PlayerPawnInputs PlayerPawnInstance Components"))
class SPACEINVADERS_API AASIPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	AASIPlayerPawn();
	virtual void Tick(float DeltaTime) override final;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override final;

	void HandleDestruction(AActor* Destroyer);

protected:
	virtual void BeginPlay() override final;

private:
	void AddInputMapping();
	void SpawnProjectile();

	UFUNCTION()
	void OnProjectileHit();

	UFUNCTION()
	void MoveInputTriggered(const FInputActionValue& Value);
	UFUNCTION()
	void FireInputTriggered(const FInputActionValue& Value);
	UFUNCTION()
	void ActivatePowerUpInputTriggered(const FInputActionValue& Value);

	void Move(float DeltaTime);
	void Fire();
	void ActivatePowerUp();

	void HandleDestruction();

private:
	// Components
	UPROPERTY(Category = "Component", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMeshComp = nullptr;

	UPROPERTY(Category = "Component", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> ProjectileSpawnPoint = nullptr;

	UPROPERTY(Category = "Components", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<AASIBaseProjectile> ProjectileClass = nullptr;

	// TODO: VFXs
	//UPROPERTY(EditAnywhere, Category = "Pawn Variables")
	//UParticleSystem* DeathParticles;

	//UPROPERTY(Category = "CameraPawnVFXs", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	//TObjectPtr<UNiagaraSystem> MoveOrderCursorEffect = nullptr;

	//UPROPERTY(Category = "CameraPawnVFXs", EditAnywhere, meta = (AllowPrivateAccess = true))
	//FVector MoveOrderCursorEffectScale = FVector(1.0);
	
	//UPROPERTY(EditAnywhere, Category = "Pawn Variables")
	//USoundBase* DeathSound;
	
	//UPROPERTY(EditAnywhere, Category = "Pawn Variables")
	//TSubclassOf<UCameraShakeBase> DeathCameraShakeClass;

	// Input
	UPROPERTY(Category = "PlayerPawnInputs", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> PlayerPawnInputMappingContext = nullptr;
	
	UPROPERTY(Category = "PlayerPawnInputs", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> MoveInputAction = nullptr;

	UPROPERTY(Category = "PlayerPawnInputs", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> FireInputAction = nullptr;

	UPROPERTY(Category = "PlayerPawnInputs", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> ActivatePowerUpInputAction = nullptr;

	// Properties
	UPROPERTY(Category = "PlayerPawnSettings", EditAnywhere, meta = (AllowPrivateAccess = true))
	bool bMovementEnabled = true;

	UPROPERTY(Category = "PlayerPawnSettings", EditAnywhere, meta = (AllowPrivateAccess = true))
	float MovementSpeed = 50.0f;

	UPROPERTY(Category = "PlayerPawnInstance", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	float CachedMovementInputValue = 0.0f;

	AASIPlayerController* PlayerController = nullptr;

	AASIBaseProjectile* Projectile = nullptr;
	bool bProjectileAvailable = false;
};
