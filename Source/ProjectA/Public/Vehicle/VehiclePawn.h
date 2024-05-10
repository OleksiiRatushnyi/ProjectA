// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "VehiclePawn.generated.h"

class USpringArmComponent;
class UCameraComponent;

class UChaosWheeledVehicleMovementComponent;

class UInputAction;
struct FInputActionValue;

/*
 * The FInputActions structure contains a set of properties that define input actions
 * for the player to control a vehicle
 */
USTRUCT(BlueprintType)
struct FInputActions
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> ThrottleAction;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> BrakeAction;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> HandbrakeAction;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> SteeringAction;
};

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

	// Player Input Handling
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void OnThrottleAction(const FInputActionValue& Value);
	void OnBrakeAction(const FInputActionValue& Value);
	void OnHandbrakeAction(const FInputActionValue& Value);
	void OnSteeringAction(const FInputActionValue& Value);

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> Camera;

private:
	TObjectPtr<UChaosWheeledVehicleMovementComponent> MovementComponent;

	UPROPERTY(EditDefaultsOnly)
	FInputActions InputActions;
};
