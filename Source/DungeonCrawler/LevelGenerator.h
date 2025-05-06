// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyRoom.h"
#include "MyCorridor.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelGenerator.generated.h"

UCLASS()
class DUNGEONCRAWLER_API ALevelGenerator : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ALevelGenerator();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

protected:
    UPROPERTY(EditAnywhere)
    TSubclassOf<class AMyRoom> RoomClass;

    UPROPERTY(EditAnywhere)
    TSubclassOf<class AMyCorridor> CorridorClass;

    UPROPERTY(EditAnywhere)
    int32 NumberOfRooms = 10;

    UPROPERTY(EditAnywhere)
    float TileSize = 1000.0f;

    UPROPERTY()
    TMap<FVector2D, AMyRoom*> SpawnedRooms;

private:
    TSet<FVector2D> OccupiedTiles;

    void GenerateLevel();
    void ResetLevel();
    bool IsOccupied(FVector2D Pos);
    void MarkOccupied(FVector2D Pos);
    void SpawnRoomAt(FVector2D GridPos);
    void SpawnCorridorBetween(FVector2D From, FVector2D To);
    TArray<FVector2D> GetUnoccupiedNeighbors(FVector2D From);
};
