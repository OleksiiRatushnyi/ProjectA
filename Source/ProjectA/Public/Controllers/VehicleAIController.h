// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Vehicle/VehiclePawn.h"
#include "VehicleAIController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAIController, Warning, All);

class USplineComponent;

UCLASS()
class PROJECTA_API AVehicleAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* InPawn) override;
	void InitializeControlledVehicle();
	void InitializePath();
	
private:
	TWeakObjectPtr<AVehiclePawn> ControlledVehicle;

	UPROPERTY(EditDefaultsOnly, Category = "Path")
	TSubclassOf<AActor> Spline;

	TObjectPtr<USplineComponent> Path;
};
