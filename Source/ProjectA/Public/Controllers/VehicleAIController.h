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
	int CalculateStartingSideOfRoad();

	FVector GetClosestLocationToPath(const FVector& AILocation, float AdditionalDistance, int32 SideOfRoadParam) const;

	virtual void Tick(float DeltaSeconds) override;
	float CalculateSteering() const;
	float GetAdditionalDistanceForSteering(FVector2D SpeedRangeForSteering, FVector2D DistanceRangeForSteering) const;
	FRotator GetClosestPointToTheSpline(float AdditionalDistance) const;
	FRotator GetAngleOfSteering(const FRotator& ClosestPointToTheSpline) const;
	static float GetSteeringInput(FVector2D AngleRangeForSteering, FVector2D SteeringInputRange, const FRotator& AngleOfSteering);
	std::pair<float, float> CalculateThrottleAndBrake(float TopSpeed) const;
	float CalculateBrakeIntensity(float TopSpeed) const;
	float CalculateTopSpeed() const;
	float GetAdditionalDistanceForTopSpeed(FVector2D SpeedRangeForTopSpeed, FVector2D DistanceRangeForTopSpeed) const;
	static float GetTopSpeed(FVector2D AngleRangeForTopSpeed, FVector2D TopSpeedRange, const FRotator& AngleOfSteering);
	
private:
	TWeakObjectPtr<AVehiclePawn> ControlledVehicle;

	UPROPERTY(EditDefaultsOnly, Category = "Path")
	TSubclassOf<AActor> Spline;

	TObjectPtr<USplineComponent> Path;
	
	int32 SideOfRoad;
};
