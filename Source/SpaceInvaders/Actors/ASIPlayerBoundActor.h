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

private:
	UPROPERTY(Category = "Component", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SceneComp = nullptr;

	UPROPERTY(Category = "Components", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMeshComp = nullptr;

};
