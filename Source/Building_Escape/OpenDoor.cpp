// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"
#include "Components/PrimitiveComponent.h"

#define OUT

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
	AnguloParaAbrir += CurrentYaw; // 30+0
	FindPressurePlate();	
	/*
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	if(!ActorThatOpens)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s tiene el componente OpenDoor pero no tiene un ActorThatOpens establecido"), *GetOwner()->GetName());
	}	
	*/
	FindAudioComponent();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//UE_LOG(LogTemp, Warning, TEXT("%s's Yaw: %f"), *GetOwner()->GetName(), GetOwner()->GetActorRotation().Yaw);
	
	//if(Accionador && Accionador->IsOverlappingActor(ActorThatOpens))
	if(GetTotalMassOfActors() > PesoParaAbrir)
	{
		OpenDoor(DeltaTime);
		//when the door was opened
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		// dandole un delay al jugador para que escape
		float CurrentTime = GetWorld()->GetTimeSeconds();
		//UE_LOG(LogTemp, Warning, TEXT("CurrentTime: %f, DoorLastOpened+RetardoParaCerrar: %f"), CurrentTime, DoorLastOpened+RetardoParaCerrar);
		if(CurrentTime >= DoorLastOpened+RetardoParaCerrar)
		{
			CloseDoor(DeltaTime);
		}
	}
	
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
		
	CurrentYaw = FMath::FInterpTo(CurrentYaw, AnguloParaAbrir, DeltaTime, TiempoParaAbrir);
	UpdateDoorsYaw(CurrentYaw);
	if(!DoorOpen && AudioComponent)
	{
		AudioComponent->Play();
		DoorOpen = true;
	}			
}

void UOpenDoor::CloseDoor(float DeltaTime)
{	
	CurrentYaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, TiempoParaAbrir);
	UpdateDoorsYaw(CurrentYaw);
	if(DoorOpen && AudioComponent)
	{
		AudioComponent->Play();
		DoorOpen = false;
	}	
}

void UOpenDoor::UpdateDoorsYaw(float CurrentDoorsYaw)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s's Yaw: %f"), *GetOwner()->GetName(), GetOwner()->GetActorRotation().Yaw);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentDoorsYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}

float UOpenDoor::GetTotalMassOfActors() const
{
	float TotalMass = 0.f;
	//Find all overlaping actors
	TArray<AActor*> OverlapingActors;
	if(Accionador)
	{
		Accionador->GetOverlappingActors(OUT OverlapingActors);
	}		
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s tiene el componente OpenDoor pero no tiene un Accionador establecido!"), *GetOwner()->GetName());
		return TotalMass;
	}
	
	for(AActor* Actor: OverlapingActors)
	{		
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}	
	return TotalMass;
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if(!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s No tiene un Audio Component con el sonido establecido!!"), *GetOwner()->GetName());	
	}		
}
void UOpenDoor::FindPressurePlate()
{
	if(!Accionador)
	{
		UE_LOG(LogTemp, Error, TEXT("%s tiene el componente OpenDoor pero no tiene un Accionador establecido!"), *GetOwner()->GetName());
	}
}