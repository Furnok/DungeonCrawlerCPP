// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"
#include "MyRoom.h"
#include "MyCorridor.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ALevelGenerator::ALevelGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();
	GenerateLevel();
}

void ALevelGenerator::GenerateLevel()
{
    FVector2D StartPos(0, 0);
    SpawnRoomAt(StartPos, RoomEnterClass);
    MarkOccupied(StartPos);

    TArray<FVector2D> RoomPositions = { StartPos };

    for (int32 i = 1; i < NumberOfRooms; ++i)
    {
        FVector2D From = RoomPositions[FMath::RandRange(0, RoomPositions.Num() - 1)];
        TArray<FVector2D> Neighbors = GetUnoccupiedNeighbors(From);

        if (Neighbors.Num() > 0)
        {
            FVector2D To = Neighbors[FMath::RandRange(0, Neighbors.Num() - 1)];

            int32 Index = FMath::RandRange(0, RoomClass.Num() - 1);
            TSubclassOf<AMyRoom> SelectedClass = RoomClass[Index];

            SpawnRoomAt(To, SelectedClass);
            RoomPositions.Add(To);
            MarkOccupied(To);
            SpawnCorridorBetween(From, To);
        }
    }

    if (RoomPositions.Num() > 1)
    {
        FVector2D ExitPos;
        do
        {
            ExitPos = RoomPositions[FMath::RandRange(1, RoomPositions.Num() - 1)];
        } while (ExitPos == StartPos); // Ensure not placing at start

        // Remove existing room at ExitPos
        if (AMyRoom* OldRoom = SpawnedRooms.FindRef(ExitPos))
        {
            SpawnedRooms.Remove(ExitPos);
            OldRoom->Destroy();
        }

        // Spawn the exit room
        SpawnRoomAt(ExitPos, RoomExitClass);

        if (AMyRoom* ExitRoom = SpawnedRooms.FindRef(ExitPos))
        {
            TArray<FVector2D> Directions = {
                FVector2D(1, 0),   // Right
                FVector2D(-1, 0),  // Left
                FVector2D(0, 1),   // Up
                FVector2D(0, -1)   // Down
            };

            for (FVector2D Dir : Directions)
            {
                FVector2D NeighborPos = ExitPos + Dir;

                if (AMyRoom* Neighbor = SpawnedRooms.FindRef(NeighborPos))
                {
                    // Check if a corridor exists between ExitPos and NeighborPos
                    bool bCorridorExists = false;
                    for (AMyCorridor* Corridor : SpawnedCorridors)
                    {
                        if (!Corridor) continue;

                        FVector CorridorLoc = Corridor->GetActorLocation();
                        FVector ExpectedLoc = FVector((ExitPos.X + NeighborPos.X) * 0.5f * TileSize,
                            (ExitPos.Y + NeighborPos.Y) * 0.5f * TileSize,
                            0.0f);

                        if (CorridorLoc.Equals(ExpectedLoc, 1.0f)) // Tolerance of 1.0f
                        {
                            bCorridorExists = true;
                            break;
                        }
                    }

                    if (bCorridorExists)
                    {
                        FVector2D Dir2 = NeighborPos - ExitPos;

                        if (Dir2.X < 0)
                        {
                            ExitRoom->RemoveDoor(FVector2D(0, -1)); // South
                        }
                        else if (Dir2.X > 0)
                        {
                            ExitRoom->RemoveDoor(FVector2D(0, 1)); // North
                        }
                        else if (Dir2.Y > 0)
                        {
                            ExitRoom->RemoveDoor(FVector2D(1, 0)); // West
                        }
                        else if (Dir2.Y < 0)
                        {
                            ExitRoom->RemoveDoor(FVector2D(-1, 0)); // East
                        }
                    }
                }
            }
        }
    }
}

void ALevelGenerator::ResetLevel()
{
    // Destroy all currently spawned rooms and corridors
    for (auto& RoomEntry : SpawnedRooms)
    {
        AMyRoom* Room = RoomEntry.Value;
        if (Room)
        {
            Room->Destroy();
        }
    }

    // Destroy all spawned corridors
    for (AMyCorridor* Corridor : SpawnedCorridors)
    {
        if (Corridor)
        {
            Corridor->Destroy();
        }
    }

    SpawnedCorridors.Empty();

    // Clear the list of spawned rooms
    SpawnedRooms.Empty();

    // Clear the occupied tiles list
    OccupiedTiles.Empty();

    // Trigger the level generation again
    GenerateLevel();
}

bool ALevelGenerator::IsOccupied(FVector2D Pos)
{
    return OccupiedTiles.Contains(Pos);
}

void ALevelGenerator::MarkOccupied(FVector2D Pos)
{
    OccupiedTiles.Add(Pos);
}

void ALevelGenerator::SpawnRoomAt(FVector2D GridPos, TSubclassOf<AMyRoom> RoomToSpawn)
{
    FVector WorldLocation(GridPos.X * TileSize, GridPos.Y * TileSize, 0.0f);
    AMyRoom* NewRoom = GetWorld()->SpawnActor<AMyRoom>(RoomToSpawn, WorldLocation, FRotator::ZeroRotator);
    if (NewRoom)
    {
        SpawnedRooms.Add(GridPos, NewRoom);
    }
}

void ALevelGenerator::SpawnCorridorBetween(FVector2D From, FVector2D To)
{
    FVector WorldPos = FVector((From.X + To.X) * 0.5f * TileSize, (From.Y + To.Y) * 0.5f * TileSize, 0.0f);
    FVector2D Dir = To - From;
    FRotator Rotation;

    // Handle Direction for Rotation
    if (FMath::Abs(Dir.X) > FMath::Abs(Dir.Y))  // Horizontal (Left or Right)
    {
        Rotation = (Dir.X > 0) ? FRotator(0, 0, 0) : FRotator(0, 180, 0);
    }
    else  // Vertical (Up or Down)
    {
        Rotation = (Dir.Y > 0) ? FRotator(0, 90, 0) : FRotator(0, 270, 0);
    }

    // Spawn the Corridor Actor
    AMyCorridor* Corridor = GetWorld()->SpawnActor<AMyCorridor>(CorridorClass, WorldPos, Rotation);

    if (Corridor)
    {
        SpawnedCorridors.Add(Corridor);
    }

    // Ensure the "From" room is spawned and added to SpawnedRooms
    if (AMyRoom* FromRoom = SpawnedRooms.FindRef(From))
    {
        // Remove the wall from the "From" room
        if (Dir.X < 0)  // Left (X negative direction)
        {
            FromRoom->RemoveDoor(FVector2D(0, -1)); // Remove bottom (South) wall
        }
        else if (Dir.X > 0)  // Right (X positive direction)
        {
            FromRoom->RemoveDoor(FVector2D(0, 1)); // Remove top (North) wall
        }
        else if (Dir.Y > 0)  // Up (Y positive direction)
        {
            FromRoom->RemoveDoor(FVector2D(1, 0)); // Remove west (West) wall
        }
        else if (Dir.Y < 0)  // Down (Y negative direction)
        {
            FromRoom->RemoveDoor(FVector2D(-1, 0)); // Remove east (East) wall
        }
    }

    // Ensure the "To" room is spawned if it doesn't exist in SpawnedRooms
    if (AMyRoom* ToRoom = SpawnedRooms.FindRef(To))
    {
        // Remove the wall from the "To" room
        if (Dir.X < 0)  // Left (X negative direction)
        {
            ToRoom->RemoveDoor(FVector2D(0, 1)); // Remove top (North) wall
        }
        else if (Dir.X > 0)  // Right (X positive direction)
        {
            ToRoom->RemoveDoor(FVector2D(0, -1)); // Remove bottom (South) wall
        }
        else if (Dir.Y > 0)  // Up (Y positive direction)
        {
            ToRoom->RemoveDoor(FVector2D(-1, 0)); // Remove east (East) wall
        }
        else if (Dir.Y < 0)  // Down (Y negative direction)
        {
            ToRoom->RemoveDoor(FVector2D(1, 0)); // Remove west (West) wall
        }
    }
}

TArray<FVector2D> ALevelGenerator::GetUnoccupiedNeighbors(FVector2D From)
{
    TArray<FVector2D> Neighbors;

    TArray<FVector2D> Directions = {
        FVector2D(1, 0),
        FVector2D(-1, 0),
        FVector2D(0, 1),
        FVector2D(0, -1)
    };

    for (FVector2D Dir : Directions)
    {
        FVector2D Neighbor = From + Dir;
        if (!IsOccupied(Neighbor))
        {
            Neighbors.Add(Neighbor);
        }
    }

    return Neighbors;
}