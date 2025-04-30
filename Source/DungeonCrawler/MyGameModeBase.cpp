// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "MyCharacter.h"
#include "MyHUD.h"
#include "Blueprint/UserWidget.h"

AMyGameModeBase::AMyGameModeBase()
{
	// Set the default pawn class to your custom character
	DefaultPawnClass = AMyCharacter::StaticClass();

	// Set the default hud class to your custom character
	HUDClass = AMyHUD::StaticClass();
}
