// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AdvancedReplicatedObject.generated.h"

/**
 * UAdvancedReplicatedObject is a subclass of UObject designed to support network replication.
 * This class allows for the replication of its properties across the network, providing
 * functionalities to handle ownership and networking features. It includes overridden methods
 * for managing replication and lifecycle events.
 *
 * @class UAdvancedReplicatedObject
 * @brief A UObject that supports network replication and custom replication handling.
 * @note UObject itself does not support replication natively, so this class provides
 *       mechanisms to replicate properties and manage network-related functionality.
 */
UCLASS(Blueprintable, BlueprintType)
class REPLICATEDOBJECT_API UAdvancedReplicatedObject : public UObject
{
	GENERATED_BODY()
public:
	/**
	* Constructor for UAdvancedReplicatedObject.
	* Initializes the object using the provided object initializer.
	*
	* @param ObjectInitializer The object initializer to use for setup.
	*/
	UAdvancedReplicatedObject(const FObjectInitializer& ObjectInitializer);
private:
	/**
	* The actor that owns this object. This property is replicated over the network.
	*
	* @property Owner
	* @see OnRep_Owner
	*/
	UPROPERTY(ReplicatedUsing=OnRep_Owner)
	AActor* Owner;

protected:

	/**
	 * Callback function called when the Owner property is updated on clients.
	 * Override this function to handle logic that should occur when the Owner property changes.
	 *
	 * @note This function is automatically called on clients when the Owner property is replicated.
	 */
	UFUNCTION()
	virtual void OnRep_Owner() { }

	/**
	* Called when the object is initialized and ready to be used.
	* Override this function to perform any necessary setup or initialization logic.
	*/
	UFUNCTION()
	virtual void ObjectBeginPlay() { }

	/**
	 * Called when the object is about to be destroyed.
	 * Override this function to handle cleanup or finalization before destruction.
	 */
	UFUNCTION()
	virtual void ObjectEndPlay() { }
public:
	/**
	 * Defines which properties are replicated and how.
	 * Override this function to specify additional properties that need replication.
	 *
	 * @param OutLifetimeProps The array of properties to be replicated.
	 */
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	/**
	* Determines whether this object supports networking.
	* Override this function to indicate that the object supports networking.
	*
	* @return true if the object supports networking; false otherwise.
	*/
	virtual bool IsSupportedForNetworking() const override;

	/**
	 * Called after properties have been initialized.
	 * Override this function to handle additional setup after properties have been initialized.
	 */
	virtual void PostInitProperties() override;
	
	/**
	* Called when the object is about to be destroyed.
	* Override this function to perform any necessary cleanup before destruction.
	*/
	virtual void BeginDestroy() override;

public:
	/**
	* Handles the replication of subobjects for this object.
	* Override this function to manage replication of subobjects.
	*
	* @param Channel The channel to use for replication.
	* @param Bunch The bunch of data to replicate.
	* @param RepFlags Flags for replication.
	* @return true if the subobjects were successfully replicated; false otherwise.
	*/
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags);
public:
	virtual bool CallRemoteFunction(UFunction* Function, void* Parms, struct FOutParmRec* OutParms, FFrame* Stack) override;
	virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;
	ENetRole GetOwnerRole() const;

public:
	UFUNCTION(BlueprintCallable, Category="UAdvancedReplicatedObject")
	virtual AActor* GetOwner() const;
	UFUNCTION(BlueprintCallable, Category="UAdvancedReplicatedObject")
	virtual void SetOwner(AActor* NewOwner);
	
	UFUNCTION(BlueprintCallable, Category="UAdvancedReplicatedObject")
	bool HasAuthority() const;
	
	UFUNCTION(BlueprintCallable, Category="UAdvancedReplicatedObject")
	virtual UWorld* GetWorld() const override;
};

