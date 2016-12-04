// Erick Pombo Sonderblohm 2016

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));

	/// Look for attached Physics Handle
	//PhysicsHandle = Cast<UPhysicsHandleComponent>( GetOwner()->GetComponentByClass(UPhysicsHandleComponent::StaticClass()) );
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle)
	{
		// Physics Handle has been found!
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Physics Handle Component not found, UGrabber::BeginPlay at line 36 - OnPawn: %s"), *GetOwner()->GetName());
	}
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	/// Get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint( OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	/*UE_LOG(LogTemp, Warning, TEXT("location: %s :: rotation: %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString());*/

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	/// Draw a debug line
	DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor::Red, false, 0.f, 0.f, 10.f);

	/// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Line-trace (aka Ray-cast) out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, PlayerViewPointLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);

	/// See what we hit
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor Hit!: %s"), *ActorHit->GetName());
	}
}

