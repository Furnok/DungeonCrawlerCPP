// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONCRAWLER_API AMyHUD : public AHUD
{
	GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> MenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> LobbyWidgetClass;

private:
    UPROPERTY()
    UUserWidget* CurrentWidget;
};
