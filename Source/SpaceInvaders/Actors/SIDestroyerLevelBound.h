// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SIDestroyerLevelBound.generated.h"

class UStaticMeshComponent;

UCLASS()
class SPACEINVADERS_API ASIDestroyerLevelBound : public AActor
{
	GENERATED_BODY()
	
public:	
	ASIDestroyerLevelBound();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			FVector NormalImpulse, const FHitResult& Hit);

private:
	UPROPERTY(Category = "Components", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMeshComp = nullptr;

};
