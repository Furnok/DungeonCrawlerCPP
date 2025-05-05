// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "MyHUD.h"

void AMyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    FInputModeGameOnly InputModeGame;
    SetInputMode(InputModeGame);

    FInputModeGameAndUI InputModeGameUI;
    SetInputMode(InputModeGameUI);

    bShowMouseCursor = false;

    InputComponent->BindAction("Menu", IE_Pressed, this, &AMyPlayerController::HandleCloseMenu);
}

void AMyPlayerController::HandleCloseMenu()
{
    FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);

    AMyHUD* HUD = Cast<AMyHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());

    if (!HUD) return;

    if (CurrentLevelName == "L_Game")
    {
        HUD->HandleMenu();
    }
    else
    {
        HUD->InputCloseWidget();
    }
}
