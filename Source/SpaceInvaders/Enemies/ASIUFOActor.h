// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASIUFOActor.generated.h"

class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class SPACEINVADERS_API AASIUFOActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AASIUFOActor();

//protected:
//	virtual void BeginPlay() override;
//
//public:	
//	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(Category = "Component", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SceneComp = nullptr;

	UPROPERTY(Category = "Components", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMeshComp = nullptr;

};
