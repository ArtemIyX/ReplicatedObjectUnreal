// Fill out your copyright notice in the Description page of Project Settings.
#include "Data/AdvancedReplicatedObject.h"

#include "Net/UnrealNetwork.h"

UAdvancedReplicatedObject::UAdvancedReplicatedObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Owner = GetTypedOuter<AActor>();
}


void UAdvancedReplicatedObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UAdvancedReplicatedObject, Owner);
}

bool UAdvancedReplicatedObject::CallRemoteFunction(UFunction* Function, void* Parms, FOutParmRec* OutParms, FFrame* Stack)
{
	bool bProcessed = false;

	if (AActor* MyOwner = GetOwner())
	{
		FWorldContext* const Context = GEngine->GetWorldContextFromWorld(GetWorld());
		if (Context != nullptr)
		{
			for (FNamedNetDriver& Driver : Context->ActiveNetDrivers)
			{
				if (Driver.NetDriver != nullptr && Driver.NetDriver->ShouldReplicateFunction(MyOwner, Function))
				{
					Driver.NetDriver->ProcessRemoteFunction(MyOwner, Function, Parms, OutParms, Stack, this);
					bProcessed = true;
				}
			}
		}
	}

	return bProcessed;
}

int32 UAdvancedReplicatedObject::GetFunctionCallspace(UFunction* Function, FFrame* Stack)
{
	if (AActor* MyOwner = GetOwner())
	{
		return MyOwner->GetFunctionCallspace(Function, Stack);
	}
	return FunctionCallspace::Local;
}

bool UAdvancedReplicatedObject::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	return false;
}

void UAdvancedReplicatedObject::PostInitProperties()
{
	UObject::PostInitProperties();
	if (GetWorld())
	{
		ObjectBeginPlay();
	}
}

void UAdvancedReplicatedObject::BeginDestroy()
{
	if(GetWorld())
	{
		ObjectEndPlay();
	}
	UObject::BeginDestroy();
}

AActor* UAdvancedReplicatedObject::GetOwner() const
{
	return Owner;
}

void UAdvancedReplicatedObject::SetOwner(AActor* NewOwner)
{
	if (Owner != NewOwner && !this->HasAnyFlags(RF_InternalPendingKill | RF_InternalGarbage))
	{
		Owner = NewOwner;
	}
}

ENetRole UAdvancedReplicatedObject::GetOwnerRole() const
{
	if (AActor* MyOwner = GetOwner())
	{
		return MyOwner->GetLocalRole();
	}
	return ROLE_None;
}

bool UAdvancedReplicatedObject::IsSupportedForNetworking() const
{
	return true;
}

UWorld* UAdvancedReplicatedObject::GetWorld() const
{
	if (const UObject* outer = GetOuter())
	{
		return outer->GetWorld();
	}
	return nullptr;
}

bool UAdvancedReplicatedObject::HasAuthority() const
{
	return GetOwner() && GetOwner()->HasAuthority();
}
