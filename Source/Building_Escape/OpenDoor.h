// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	void UpdateDoorsYaw(float CurrentDoorsYaw);
	float GetTotalMassOfActors() const;
	void FindAudioComponent();
	void FindPressurePlate();

private:
	float InitialYaw;
	float CurrentYaw;

	UPROPERTY(EditAnywhere);
	float TiempoParaAbrir = 0.5f;

	UPROPERTY(EditAnywhere);
	float AnguloParaAbrir = 90.0f;	

	UPROPERTY(EditAnywhere);
	float PesoParaAbrir = 50.0f;	

	float DoorLastOpened = 0.0f;

	UPROPERTY(EditAnywhere);
	float RetardoParaCerrar = 2.f;

	UPROPERTY(EditAnywhere);
	ATriggerVolume* Accionador = nullptr;

	//AActor* ActorThatOpens = nullptr;
	UPROPERTY();
	UAudioComponent* AudioComponent = nullptr;
	UPROPERTY();
	UInputComponent* Input = nullptr;

	bool DoorOpen = false;
};
