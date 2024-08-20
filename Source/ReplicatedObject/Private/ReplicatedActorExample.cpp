// Fill out your copyright notice in the Description page of Project Settings.


#include "ReplicatedActorExample.h"

#include "Data/AdvancedReplicatedObject.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AReplicatedActorExample::AReplicatedActorExample(): MyObjectPtr(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AReplicatedActorExample::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		MyObjectPtr = NewObject<UAdvancedReplicatedObject>(this, UAdvancedReplicatedObject::StaticClass());
		MyArray.Add(MyObjectPtr = NewObject<UAdvancedReplicatedObject>(this, UAdvancedReplicatedObject::StaticClass()));
		MyArray.Add(MyObjectPtr = NewObject<UAdvancedReplicatedObject>(this, UAdvancedReplicatedObject::StaticClass()));
	}
}

void AReplicatedActorExample::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AReplicatedActorExample, MyObjectPtr);
	DOREPLIFETIME(AReplicatedActorExample, MyArray);
}

bool AReplicatedActorExample::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool sup = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	if (IsValid(MyObjectPtr))
	{
		sup |= Channel->ReplicateSubobject(MyObjectPtr, *Bunch, *RepFlags);
		sup |= MyObjectPtr->ReplicateSubobjects(Channel, Bunch, RepFlags);
	}
	int32 n = MyArray.Num();
	if(n > 0)
	{
		for(int32 i = 0; i < n; ++i)
		{
			sup |= Channel->ReplicateSubobject(MyArray[i], *Bunch, *RepFlags);
			sup |= MyArray[i]->ReplicateSubobjects(Channel, Bunch, RepFlags);
		}
	}
	return sup;
}

// Called every frame
void AReplicatedActorExample::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
