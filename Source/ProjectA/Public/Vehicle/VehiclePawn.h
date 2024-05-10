// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "VehiclePawn.generated.h"

class USpringArmComponent;
class UCameraComponent;

class UChaosWheeledVehicleMovementComponent;

/*
 * Base class for a vehicle
 */
UCLASS()
class PROJECTA_API AVehiclePawn : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:
	// Common Vehicle Functionality
	AVehiclePawn();
	void InitializeMesh() const;
	void InitializeSpringArm();
	void InitializeCamera();
	void InitializeMovementComponent();

	virtual void Tick(float Delta) override;
	void ApplyAngularDamping();
	void CameraUpdate(float Delta);

	float GetCurrentSpeed();

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> Camera;

private:
	TObjectPtr<UChaosWheeledVehicleMovementComponent> MovementComponent;
};
