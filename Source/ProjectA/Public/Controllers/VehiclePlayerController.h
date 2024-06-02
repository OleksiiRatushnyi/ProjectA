// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VehiclePlayerController.generated.h"

class UInputMappingContext;
class AVehiclePawn;
class UVehicleHUD;

UCLASS()
class PROJECTA_API AVehiclePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* InPawn) override;
	void AddInputMappingContext();

	virtual void Tick(float Delta) override;
	
	TObjectPtr<AVehiclePawn> VehiclePawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<UVehicleHUD> VehicleUIClass;
	
	TObjectPtr<UVehicleHUD> VehicleUI;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> InputMappingContext;
};
