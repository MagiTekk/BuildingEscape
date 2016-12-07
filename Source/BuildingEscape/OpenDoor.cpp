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
	Owner = GetOwner();
}

void UOpenDoor::OpenDoor()
{
	isDoorOpen = true;
	Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
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
	if (GetTotalMassOfActorsOnPlate() > 50.f) // TODO make into a parameter
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
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Iterate through them adding their masses
	for (AActor* Actor : OverlappingActors)
	{
		// TODO in next video
		UStaticMeshComponent* StaticMesh = Actor->FindComponentByClass<UStaticMeshComponent>();
		if (StaticMesh)
		{
			TotalMass += StaticMesh->GetMass();
		}

		/*
		TArray<UPrimitiveComponent*> comps;
		Actor->GetComponents(OUT comps);

		for (UPrimitiveComponent* comp: comps)
		{
			if (comp)
			{
				TotalMass += comp->GetMass();
			}
		}
		*/
	}

	return TotalMass;
}

