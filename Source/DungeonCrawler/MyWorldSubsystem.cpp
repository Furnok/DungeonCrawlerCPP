// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWorldSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "MyHUD.h"

void UMyWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    UWorld* World = GetWorld();

    if (IsValid(World) && GetWorld()->IsGameWorld())
    {
        GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
        {
            ShowUIForCurrentLevel();
        });
    }
}

void UMyWorldSubsystem::ShowUIForCurrentLevel()
{
    FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);

    AMyHUD* HUD = Cast<AMyHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());

    if (!HUD) return;

    if (CurrentLevelName == "L_Menu")
    {
        HUD->ShowWidget(HUD->MenuWidgetClass);
    }
    else if (CurrentLevelName == "L_Lobby")
    {
        HUD->ShowWidget(HUD->LobbyWidgetClass);
    }
}