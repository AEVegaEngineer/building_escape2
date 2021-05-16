// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

#define OUT

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
	FindPhysicsHandle();
	SetupInputComponent();
}

//check for the physics handle component
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(PhysicsHandle == nullptr)
	{	
		UE_LOG(LogTemp,Error, TEXT("Error!: el objeto %s tiene la clase Grabber pero le falta el componente PhysicsHandle!"),*GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	Input = GetOwner()->FindComponentByClass<UInputComponent>();
	if(Input)
	{
		//UE_LOG(LogTemp,Warning, TEXT("Error!: el objeto %s tiene la clase Grabber y ha encontrado un InputComponent"),*GetOwner()->GetName());
		Input->BindAction("Grab",IE_Pressed, this, &UGrabber::Grab);
		Input->BindAction("Grab",IE_Released, this, &UGrabber::Release);
	} 
	else
	{
		UE_LOG(LogTemp,Error, TEXT("Error!: el objeto %s tiene la clase Grabber pero le falta el InputComponent!"),*GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	if(HitResult.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation
		(
			ComponentToGrab,
			NAME_None,
			GetLineTraceEnd()
		);
	}
}
void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}

FVector UGrabber::GetLineTraceEnd() const
{
	// Get Players Viewpoint
	FVector PlayerViewPointLocation = {0.f,0.f,0.f};
	FRotator PlayerViewPointRotation = {0.f,0.f,0.f};
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint( OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	//UE_LOG(LogTemp, Warning, TEXT("PlayerViewPointLocation: %s, PlayerViewPointRotation: %s"),*PlayerViewPointLocation.ToString(),*PlayerViewPointRotation.ToString());
	// Draw a line from player showing the reach	
	return PlayerViewPointLocation + ( PlayerViewPointRotation.Vector() * Alcance) ;	
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	//If the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		//move the object we are holding
		PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const 
{
	/*
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(0,255,0),
		false,
		0.f,
		0,
		5.f // grueso de la linea
	);
	*/
	//Ray-cast out to a certain distance (reach)
	
	FCollisionQueryParams TraceParams(FName(TEXT("")),false,GetOwner());// lo que no queremos intersectar
	FHitResult Hit;
	bool FoundSomething = GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetOwner()->GetActorLocation(),
		GetLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), // lo que queremos intersectar
		TraceParams // lo que no queremos intersectar
	);
	if(FoundSomething){
		AActor* PtrActorFound = Hit.GetActor();	
		//See what it hits	
		UE_LOG(LogTemp, Warning, TEXT("Encontrado: %s"), *PtrActorFound->GetName());
	}
	return Hit;
}