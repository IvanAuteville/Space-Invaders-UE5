// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceInvaders/Enums/HorizontalMovementType.h"
#include "ASIUFOActor.generated.h"

class UStaticMeshComponent;
class UAudioComponent;

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUFODestroyedByPlayer);

UCLASS(meta = (PrioritizeCategories = "UFOSettings UFOInstance Components"))
class SPACEINVADERS_API AASIUFOActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AASIUFOActor();
	void Tick(float DeltaTime) override final;
	void SetHorizontalMovementDirection(const EHorizontalMovementType MovementDirection);

	void HandleDestruction();
//public:
//	// Delegates
//	UPROPERTY(BlueprintAssignable)
//	FOnUFODestroyedByPlayer OnUFODestroyedByPlayer;

protected:
	void BeginPlay() override final;

private:
	//UFUNCTION()
	//void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	//		FVector NormalImpulse, const FHitResult& Hit);

	void Move(float DeltaTime);

private:
	UPROPERTY(Category = "Components", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMeshComp = nullptr;

	UPROPERTY(Category = "Components", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<UAudioComponent> AudioComp = nullptr;

	UPROPERTY(Category = "UFOSettings", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float MovementSpeed = 200.0f;

	UPROPERTY(Category = "UFOInstance", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	EHorizontalMovementType HorizontalMovementType = EHorizontalMovementType::None;

	UPROPERTY(Category = "UFOInstance", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	bool bEnabled = false;
};
