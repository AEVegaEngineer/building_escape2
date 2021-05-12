// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
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
	/*
	FRotator DoorRotator = GetOwner()->GetActorRotation();
	DoorRotator.Yaw += YawValue;
	GetOwner()->SetActorRotation(DoorRotator);
	*/
	
	InitialYaw = GetOwner()->GetActorRotation().Yaw; //0
	CurrentYaw = InitialYaw; // 0
	TargetYaw += CurrentYaw; // 30+0
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//UE_LOG(LogTemp, Warning, TEXT("%s's Yaw: %f"), *GetOwner()->GetName(), GetOwner()->GetActorRotation().Yaw);
	
	CurrentYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, TimeMultiplier);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}