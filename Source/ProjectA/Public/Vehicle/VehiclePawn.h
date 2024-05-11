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
 * The FAISettings structure contains settings for controlling AI
 */
USTRUCT()
struct FAISettings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Steering")
	FVector2D AngleRangeForSteering = FVector2D(-10.0f, 10.0f);

	UPROPERTY(VisibleDefaultsOnly, Category = "Steering")
	FVector2D SteeringInputRange = FVector2D(-1.0f, 1.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Steering ")
	FVector2D SpeedRangeForSteering = FVector2D(50.0f, 150.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Steering")
	FVector2D DistanceRangeForSteering = FVector2D(1000.0f, 3000.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Speed")
	FVector2D AngleRangeForTopSpeed = FVector2D(0.0f, 40.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Speed")
	FVector2D TopSpeedRange = FVector2D(150.0f, 50.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Speed")
	FVector2D SpeedRangeForTopSpeed = FVector2D(50.0f, 150.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Speed")
	FVector2D DistanceRangeForTopSpeed = FVector2D(3000.0f, 9000.0f);
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

	// Action processing for AI
	FAISettings GetAISettings() { return AISettings; }
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> Camera;

private:
	TObjectPtr<UChaosWheeledVehicleMovementComponent> MovementComponent;

	UPROPERTY(EditDefaultsOnly)
	FInputActions InputActions;

	UPROPERTY(EditDefaultsOnly)
	FAISettings AISettings;
};
