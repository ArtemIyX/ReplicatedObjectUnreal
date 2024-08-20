// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReplicatedActorExample.generated.h"


UCLASS()
class REPLICATEDOBJECT_API AReplicatedActorExample : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AReplicatedActorExample();
protected:
	UPROPERTY(Replicated)
	class UAdvancedReplicatedObject* MyObjectPtr;

	UPROPERTY(Replicated)
	TArray<UAdvancedReplicatedObject*> MyArray;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
