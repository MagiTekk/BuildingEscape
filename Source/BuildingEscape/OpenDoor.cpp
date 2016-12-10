// Erick Pombo Sonderblohm 2016

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT

//UE_LOG(LogTemp, Warning, TEXT("new rotation is %s"), *Owner->GetTransform().GetRotation().ToString());

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
	if (!PressurePlate) { UE_LOG(LogTemp, Error, TEXT("%s PressurePlate is not assigned!"), *GetOwner()->GetName()); return; }
	Owner = GetOwner();
}

void UOpenDoor::OpenDoor()
{
	isDoorOpen = true;
	//Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
	OnOpenRequest.Broadcast();
}

void UOpenDoor::CloseDoor()
{
	isDoorOpen = false;
	Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll the Trigger Volume
	if (GetTotalMassOfActorsOnPlate() > 30.f) // TODO make into a parameter
	{
		if (!isDoorOpen)
		{
			OpenDoor();
		}		
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	// Check if it's time to cose the door
	if (isDoorOpen && GetWorld()->GetTimeSeconds() - LastDoorOpenTime >= DoorCloseDelay)
	{
		CloseDoor();
	}
}

const float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;

	// Find all the overlapping actors
	if (!PressurePlate) { return 0; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Iterate through them adding their masses
	for (const auto& Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		
		//another solution getting all prmitive components...
		/*TArray<UPrimitiveComponent*> comps;
		Actor->GetComponents(OUT comps);

		for (const auto& PrimitiveComponent : comps)
		{
			TotalMass += PrimitiveComponent->GetMass();
		}*/
	}

	return TotalMass;
}

