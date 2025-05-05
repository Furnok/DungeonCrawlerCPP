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

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowConstWidget(TSubclassOf<UUserWidget> WidgetClass);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void CloseWidget(TSubclassOf<UUserWidget> WidgetClass);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void CloseAllWidget(TSubclassOf<UUserWidget> WidgetClass);

    UFUNCTION(BlueprintCallable, Category = "UI")
    UUserWidget* OpenWidget(TSubclassOf<UUserWidget> WidgetClass);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void InputCloseWidget();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void HandleMenu();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void OpenMenu();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void CloseMenu();

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> MainMenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> LobbyWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> MenuWidgetClass;

private:
    UPROPERTY()
    UUserWidget* CurrentMainWidget;

    UPROPERTY()
    UUserWidget* MenuWidgetInstance;

    UPROPERTY()
    TArray<UUserWidget*> WidgetStack;

    UPROPERTY()
    bool gamePause = false;
};
