// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/VehiclePlayerController.h"

#include "EnhancedInputSubsystems.h"

void AVehiclePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AddInputMappingContext();
}

void AVehiclePlayerController::AddInputMappingContext()
{
	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}
