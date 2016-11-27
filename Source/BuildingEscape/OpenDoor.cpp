// Erick Pombo Sonderblohm 2016

#include "BuildingEscape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

void UOpenDoor::OpenDoor()
{
	AActor* Owner = GetOwner();
	FQuat Rotation = Owner->GetTransform().GetRotation();
	FRotator NewRotation = FRotator(0.f, 60.f, 0.f);

	UE_LOG(LogTemp, Warning, TEXT("rotation before change is %s"), *Owner->GetTransform().GetRotation().ToString());
	Owner->SetActorRotation(NewRotation);
	UE_LOG(LogTemp, Warning, TEXT("new rotation is %s"), *Owner->GetTransform().GetRotation().ToString());
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll the Trigger Volume
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor();
	}
}

