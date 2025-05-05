// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "Blueprint/UserWidget.h"

void AMyHUD::ShowConstWidget(TSubclassOf<UUserWidget> WidgetClass)
{
    if (WidgetClass)
    {
        CurrentMainWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);

        if (CurrentMainWidget)
        {
            CurrentMainWidget->AddToViewport();
        }
    }
}

void AMyHUD::CloseWidget(TSubclassOf<UUserWidget> WidgetClass)
{
    if (WidgetClass)
    {
        for (UUserWidget* Widget : WidgetStack)
        {
            if (Widget && Widget->GetClass() == WidgetClass)
            {
                Widget->RemoveFromParent();
                WidgetStack.Remove(Widget);
                return;
            }
        }
    }
}

void AMyHUD::CloseAllWidget(TSubclassOf<UUserWidget> WidgetClass)
{
    if (WidgetClass)
    {
        for (UUserWidget* Widget : WidgetStack)
        {
            if (Widget && Widget->GetClass() != WidgetClass)
            {
                Widget->RemoveFromParent();
                WidgetStack.Remove(Widget);
            }
        }
    }
}

UUserWidget* AMyHUD::OpenWidget(TSubclassOf<UUserWidget> WidgetClass)
{
    if (WidgetClass)
    {
        for (UUserWidget* Widget : WidgetStack)
        {
            if (Widget && Widget->GetClass() == WidgetClass)
            {
                CloseWidget(WidgetClass);
                return nullptr;;
            }
        }

        UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);

        if (NewWidget)
        {
            NewWidget->AddToViewport();
            WidgetStack.Add(NewWidget);
            return NewWidget;
        }
    }

    return nullptr;
}

void AMyHUD::InputCloseWidget()
{
    if (WidgetStack.Num() == 0) return;

    UUserWidget* LastWidget = WidgetStack.Last();

    if (LastWidget && LastWidget->IsInViewport())
    {
        LastWidget->RemoveFromParent();
    }

    WidgetStack.Pop();
}

void AMyHUD::HandleMenu()
{
    if (WidgetStack.Num() > 0)
    {
        InputCloseWidget();
        return;
    }

    if (gamePause)
    {
        CloseMenu();
    }
    else
    {
        OpenMenu();
    }
}

void AMyHUD::OpenMenu()
{
    if (!MenuWidgetInstance && MenuWidgetClass)
    {
        MenuWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);

        if (MenuWidgetInstance)
        {
            MenuWidgetInstance->AddToViewport();

            gamePause = true;
        }
    }
}

void AMyHUD::CloseMenu()
{
    if (MenuWidgetInstance)
    {
        MenuWidgetInstance->RemoveFromParent();
        MenuWidgetInstance = nullptr;

        gamePause = false;
    }
}
