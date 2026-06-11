// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuGameMode.h"

void AMenuGameMode::StartPlay()
{
	Super::StartPlay();
	
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	FInputModeGameAndUI InputModeGameAndUI;
	PC->SetInputMode(InputModeGameAndUI);
	PC->bShowMouseCursor = true;
	PC->SetPause(false);
}
