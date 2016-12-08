// Erick Pombo Sonderblohm 2016

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:

	UPROPERTY(EditAnywhere)
	float Reach = 100.f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* PlayerInput = nullptr;

	// Ray-Cast and grab what is in reach
	void Grab();
	void Released();

	// Find (assumed) attached physics handle component
	void FindPhysicsHandleComponent();

	// Find (assumed) attached input component
	void SetupInputComponent();

	// Return hit for first physics body  in reach
	FHitResult GetFirstPhysicsBodyInReach() const;

	// Returns current start of reach line
	FVector GetLineTraceStart() const;

	// Returns current end of reach line
	FVector GetLineTraceEnd() const;
};
