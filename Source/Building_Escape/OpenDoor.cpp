// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"

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
	InitialYaw = GetOwner()->GetActorRotation().Yaw; //0
	CurrentYaw = InitialYaw; // 0
	TargetYaw += CurrentYaw; // 30+0
	if(!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s tiene el componente OpenDoor pero no tiene un PressurePlate establecido!"), *GetOwner()->GetName());
	}
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	if(!ActorThatOpens)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s tiene el componente OpenDoor pero no tiene un ActorThatOpens establecido"), *GetOwner()->GetName());
	}	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//UE_LOG(LogTemp, Warning, TEXT("%s's Yaw: %f"), *GetOwner()->GetName(), GetOwner()->GetActorRotation().Yaw);
	if(PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor(DeltaTime);
	}
	else
	{
		CloseDoor(DeltaTime);
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, TimeMultiplier);
	UpdateDoorsYaw(CurrentYaw);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{	
	CurrentYaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, TimeMultiplier);
	UpdateDoorsYaw(CurrentYaw);
}

void UOpenDoor::UpdateDoorsYaw(float CurrentDoorsYaw)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s's Yaw: %f"), *GetOwner()->GetName(), GetOwner()->GetActorRotation().Yaw);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentDoorsYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}
