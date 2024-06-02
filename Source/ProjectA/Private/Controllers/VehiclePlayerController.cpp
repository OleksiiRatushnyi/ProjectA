// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/VehiclePlayerController.h"

#include "EnhancedInputSubsystems.h"

#include "Vehicle/VehiclePawn.h"
#include "UI/VehicleHUD.h"
#include "ChaosWheeledVehicleMovementComponent.h"

void AVehiclePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AddInputMappingContext();

	VehiclePawn = CastChecked<AVehiclePawn>(InPawn);
	VehicleUI = CreateWidget<UVehicleHUD>(this, VehicleUIClass);
	VehicleUI->AddToViewport();
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

void AVehiclePlayerController::Tick(float Delta)
{
	Super::Tick(Delta);

	if (IsValid(VehiclePawn) && IsValid(VehicleUI))
	{
		VehicleUI->UpdateSpeed(VehiclePawn->GetChaosVehicleMovement()->GetForwardSpeed());
	}
}
