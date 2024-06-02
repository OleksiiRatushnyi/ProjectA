// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VehicleHUD.generated.h"

UCLASS()
class PROJECTA_API UVehicleHUD : public UUserWidget
{
	GENERATED_BODY()

public:

	// Called to update the speed display 
	void UpdateSpeed(float NewSpeed);

protected:

	// Implemented in Blueprint to display the new speed 
	UFUNCTION(BlueprintImplementableEvent, Category = "Vehicle")
	void OnSpeedUpdate(float NewSpeed);
};
