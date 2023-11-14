// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceInvaders/Enums/HorizontalMovementType.h"
#include "SpaceInvaders/Interfaces/Destructible.h"
#include "ASIUFOActor.generated.h"

class UStaticMeshComponent;
class UAudioComponent;
class UWidgetComponent;
class USIUFOScoreWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUFODestroyed, AActor*, Destroyer);

UCLASS(meta = (PrioritizeCategories = "UFOSettings UFOInstance Components"))
class SPACEINVADERS_API AASIUFOActor : public AActor, public IDestructible
{
	GENERATED_BODY()
	
public:	
	AASIUFOActor();
	void Tick(float DeltaTime) override final;

	void SetHorizontalMovementDirection(const EHorizontalMovementType MovementDirection);
	void SetRandomScore(const int32 UFOScore);

	/** Start IDestructible Interface **/
	void HandleDestruction(AActor* DestroyerActor) override final;
	/** End IDestructible Interface **/

public:
	// Delegates
	UPROPERTY(BlueprintAssignable)
	FOnUFODestroyed OnUFODestroyed;

protected:
	void BeginPlay() override final;

private:
	void Move(const float DeltaTime);

	UFUNCTION()
	void OnGamePaused(const bool bPaused);

private:
	UPROPERTY(Category = "Components", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMeshComp = nullptr;

	UPROPERTY(Category = "Components", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<UAudioComponent> AudioComp = nullptr;

	UPROPERTY(Category = "Component", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<UWidgetComponent> WidgetComp = nullptr;

	UPROPERTY(Category = "UFOInstance", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	EHorizontalMovementType HorizontalMovementType = EHorizontalMovementType::None;

	UPROPERTY(Category = "UFOInstance", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	float BaseMovementSpeed = 200.0f;

	UPROPERTY(Category = "UFOInstance", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	bool bEnabled = true;
		
	UPROPERTY(Category = "UFOInstance", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	USIUFOScoreWidget* ScoreWidget = nullptr;

};
