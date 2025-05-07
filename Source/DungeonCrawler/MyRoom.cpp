// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRoom.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "MyCharacter.h"
#include "LevelGenerator.h"
#include "MyHUD.h"

// Sets default values
AMyRoom::AMyRoom()
{
    PrimaryActorTick.bCanEverTick = false;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    NorthDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NorthDoor"));
    NorthDoor->SetupAttachment(Root);

    SouthDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SouthDoor"));
    SouthDoor->SetupAttachment(Root);

    EastDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EastDoor"));
    EastDoor->SetupAttachment(Root);

    WestDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WestDoor"));
    WestDoor->SetupAttachment(Root);

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetupAttachment(Root);

    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMyRoom::OnOverlapBegin);
}

void AMyRoom::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this && IsImportantRoom)
    {
        AMyCharacter* PlayerCharacter = Cast<AMyCharacter>(OtherActor);

        if (PlayerCharacter)
        {
            if (IsExit)
            {
                ALevelGenerator* LevelGen = Cast<ALevelGenerator>(
                    UGameplayStatics::GetActorOfClass(GetWorld(), ALevelGenerator::StaticClass()));

                if (LevelGen)
                {
                    AMyHUD* HUD = Cast<AMyHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());

                    if (HUD)
                    {
                        HUD->OpenWidget(HUD->PopupExitWidgetClass);

                        //PlayerCharacter->TPSpawn();

                        //LevelGen->ResetLevel();
                    }
                }
            }
            else
            {
                AMyHUD* HUD = Cast<AMyHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());

                if (HUD)
                {
                    HUD->OpenWidget(HUD->PopupEnterWidgetClass);
                }
            }
        }
    }
}

void AMyRoom::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
}

void AMyRoom::RemoveDoor(FVector2D Direction)
{
    if (Direction == FVector2D(0, 1) && NorthDoor)
    {
        NorthDoor->SetVisibility(false);
        NorthDoor->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
    else if (Direction == FVector2D(0, -1) && SouthDoor)
    {
        SouthDoor->SetVisibility(false);
        SouthDoor->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
    else if (Direction == FVector2D(1, 0) && EastDoor)
    {
        EastDoor->SetVisibility(false);
        EastDoor->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
    else if (Direction == FVector2D(-1, 0) && WestDoor)
    {
        WestDoor->SetVisibility(false);
        WestDoor->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}
