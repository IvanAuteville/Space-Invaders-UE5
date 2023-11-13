// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SIInvadersGameOverBound.generated.h"

class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInvadersReachedBottom);

UCLASS()
class SPACEINVADERS_API ASIInvadersGameOverBound : public AActor
{
	GENERATED_BODY()
	
public:	
	ASIInvadersGameOverBound();

public:
	// Delegates
	UPROPERTY()
	FOnInvadersReachedBottom OnInvadersReachedBottom;

protected:
	void BeginPlay() override final;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override final;

private:
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(Category = "Components", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMeshComp = nullptr;

};