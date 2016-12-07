// Erick Pombo Sonderblohm 2016

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT

//DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor::Red, false, 0.f, 0.f, 10.f);

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
	PlayerInput = GetOwner()->FindComponentByClass<UInputComponent>();
	if (PlayerInput)
	{
		PlayerInput->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		PlayerInput->BindAction("Grab", IE_Released, this, &UGrabber::Released);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerInput component NOT found!"));
	}
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	// Try and reach any actor with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	// If we hit something then attach a physics handle
	if (ActorHit)
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), ComponentToGrab->GetOwner()->GetActorRotation());
	}
}

void UGrabber::Released()
{
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// if the physics handle is attached 
	if (PhysicsHandle->GrabbedComponent)
	{
		// move object that we are holding
		PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
	}
		
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Line-trace (aka Ray-cast) out to reach distance
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(OUT HitResult, GetLineTraceStart(), GetLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);

	return HitResult;
}

const FVector UGrabber::GetLineTraceStart()
{
	/*UE_LOG(LogTemp, Warning, TEXT("location: %s :: rotation: %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString());*/
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	return PlayerViewPointLocation;
}

const FVector UGrabber::GetLineTraceEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}