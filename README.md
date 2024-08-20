# Replicated UObject for Unreal Engine 5.2
As you may know Unreal Engine does not replicate UObjects. But in some solutions, such as inventory, it can come in handy very often.

This plugin provides a simple solution to replicate UObjects.

# How do I?
To make replication work there are some steps that need to be followed.

- Create a class that will inherit from UAdvancedReplicatedObject
```C++
class YOUR_API UMyReplicatedObject : public UAdvancedReplicatedObject 
{
  GENERATED_BODY()
public:
  UMyReplicatedObject(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
  virtual bool IsSupportedForNetworking() const override { return true; }
}
```
- Create a UPROPERTY variable with a pointer to your object. Create an object of your class.
```C++
class YOUR_API AMyActor : public AActor {
public:
  UPROPERTY(Replicated)
  class UMyReplicatedObject* MyObjectPtr;
}
```
- Override next functions in your actor.
```C++
class YOUR_API AMyActor : public AActor {
public:
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
    virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
}
```
- It is necessary to perform variable replication as well as object data replication as follows.
```C++
void AMyActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);
  DOREPLIFETIME(AMyActor, MyObjectPtr);
}

bool AMyActor::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
  bool sup = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
  if (IsValid(MyObjectPtr))
  {
    sup |= Channel->ReplicateSubobject(MyObjectPtr, *Bunch, *RepFlags);
    sup |= MyObjectPtr->ReplicateSubobjects(Channel, Bunch, RepFlags);
  }
  return sup;
}
```
- The object must be created on the server.
> Note that it is desirable to make the Outer of the object an Actor.
Otherwise RPC will not work, and replication will work only if the Outer of your Outer is an Actor.
```C++
void AReplicatedActorExample::BeginPlay()
{
  Super::BeginPlay();
  if (HasAuthority())
  {
    MyObjectPtr = NewObject<UAdvancedReplicatedObject>(this, UAdvancedReplicatedObject::StaticClass());
  }
}
```
## Tips and tricks
- If you need to make an array, then create a array of pointers and replicate the data through the FOR.
```C++
class REPLICATEDOBJECT_API AReplicatedActorExample : public AActor {
	GENERATED_BODY()
protected:
	UPROPERTY(Replicated)
	TArray<UAdvancedReplicatedObject*> MyArray;
}
```
```C++
void AReplicatedActorExample::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		MyArray.Add(MyObjectPtr = NewObject<UAdvancedReplicatedObject>(this, UAdvancedReplicatedObject::StaticClass()));
		MyArray.Add(MyObjectPtr = NewObject<UAdvancedReplicatedObject>(this, UAdvancedReplicatedObject::StaticClass()));
	}
}
void AReplicatedActorExample::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AReplicatedActorExample, MyArray);
}
bool AReplicatedActorExample::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool sup = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
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
```
