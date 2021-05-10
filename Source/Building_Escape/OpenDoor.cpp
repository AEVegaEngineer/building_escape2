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
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw = InitialYaw-90.f;
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UE_LOG(LogTemp, Warning, TEXT("%s's Yaw: %f"), *GetOwner()->GetName(), GetOwner()->GetActorRotation().Yaw);
	
	CurrentYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, 1);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	/*
	float CurrentYaw = GetOwner()->GetActorRotation().Yaw;	
	UE_LOG(LogTemp, Warning, TEXT("Current Yaw: %f, TargetYaw: %f"), CurrentYaw, TargetYaw);	
	FRotator OpenDoor = {0.f, 0.f, 0.f};
	OpenDoor.Yaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, 1);
	GetOwner()->SetActorRotation(OpenDoor);
	*/
}