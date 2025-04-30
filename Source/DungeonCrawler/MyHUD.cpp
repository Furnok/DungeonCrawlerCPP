// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

void AMyHUD::BeginPlay()
{
    Super::BeginPlay();

    FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);

    if (CurrentLevelName == "L_Menu")
    {
        if (MenuWidgetClass)
        {
            CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
        }
    }
    else if (CurrentLevelName == "L_Lobby")
    {
        if (LobbyWidgetClass)
        {
            CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), LobbyWidgetClass);
        }
    }

    if (CurrentWidget)
    {
        CurrentWidget->AddToViewport();
    }
}