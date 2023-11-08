// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASIPlayerBoundActor.generated.h"

class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class SPACEINVADERS_API AASIPlayerBoundActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AASIPlayerBoundActor();

//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//
//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(Category = "Component", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SceneComp = nullptr;

	UPROPERTY(Category = "Components", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMeshComp = nullptr;

};
