// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASIBunkerActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class SPACEINVADERS_API AASIBunkerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AASIBunkerActor();

//protected:
//	virtual void BeginPlay() override;
//
//public:	
//	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(Category = "Component", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMeshComp = nullptr;
};
