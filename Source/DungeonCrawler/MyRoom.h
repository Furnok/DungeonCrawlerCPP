// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyRoom.generated.h"

UCLASS()
class DUNGEONCRAWLER_API AMyRoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyRoom();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* NorthDoor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* SouthDoor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* EastDoor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* WestDoor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	TSubclassOf<AActor> ChestClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	bool IsImportantRoom = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	bool IsExit = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	bool HaveChest = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	float SpawnChance = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	FVector ChestSpawn = FVector(0.f, 0.f, 50.f);

	UPROPERTY()
	AActor* SpawnedChest;

	UFUNCTION()
	void RemoveDoor(FVector2D Direction);

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};
