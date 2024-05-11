// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/VehicleAIController.h"

#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogAIController);

void AVehicleAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	InitializeControlledVehicle();
	InitializePath();
}

void AVehicleAIController::InitializeControlledVehicle()
{
	ControlledVehicle = Cast<AVehiclePawn>(GetPawn());
}

void AVehicleAIController::InitializePath()
{
	if (const AActor* SplineActor = UGameplayStatics::GetActorOfClass(GetWorld(), Spline))
	{
		Path = SplineActor->FindComponentByClass<USplineComponent>();

		// Checking for the presence of the USplineComponent component
		if (!Path)
		{
			UE_LOG(LogAIController, Warning, TEXT("Failed to find USplineComponent"));
		}
	}
	else
	{
		UE_LOG(LogAIController, Warning, TEXT("Failed to find BP_AIPath actor"));
	}
}
