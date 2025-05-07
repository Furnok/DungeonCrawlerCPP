// Fill out your copyright notice in the Description page of Project Settings.


#include "MyChest.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter.h"

// Sets default values
AMyChest::AMyChest()
{
    PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    ChestMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestMesh"));
    ChestMesh->SetupAttachment(Root);

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetupAttachment(Root);

    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMyChest::OnOverlapBegin);
    TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AMyChest::OnOverlapEnd);
}

void AMyChest::BeginPlay()
{
    Super::BeginPlay();
}

void AMyChest::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bPlayerInRange && !bIsOpen)
    {
        if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
        {
            if (PC->WasInputKeyJustPressed(EKeys::F))
            {
                OpenChest();
            }
        }
    }
}

void AMyChest::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this && OtherActor->IsA(AMyCharacter::StaticClass()))
    {
        bPlayerInRange = true;
    }
}

void AMyChest::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor != this && OtherActor->IsA(AMyCharacter::StaticClass()))
    {
        bPlayerInRange = false;
    }
}

void AMyChest::OpenChest()
{
    bIsOpen = true;
    Destroy();
}

