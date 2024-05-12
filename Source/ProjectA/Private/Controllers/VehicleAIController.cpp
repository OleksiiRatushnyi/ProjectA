// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/VehicleAIController.h"

#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(LogAIController);

void AVehicleAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	InitializeControlledVehicle();
	InitializePath();
	CalculateStartingSideOfRoad();
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

int AVehicleAIController::CalculateStartingSideOfRoad()
{
	const float LeftSideDistance = FVector::Distance(GetClosestLocationToPath(ControlledVehicle->GetActorLocation(),
		0.0f, 0), ControlledVehicle->GetActorLocation());
	const float RightSideDistance = FVector::Distance(GetClosestLocationToPath(ControlledVehicle->GetActorLocation(),
		0.0f, 1), ControlledVehicle->GetActorLocation());
	return LeftSideDistance > RightSideDistance ? SideOfRoad = 1 : SideOfRoad = 0;
}

FVector AVehicleAIController::GetClosestLocationToPath(const FVector& AILocation, float AdditionalDistance,
                                                       int32 SideOfRoadParam) const
{
	// Checking for the presence of the USplineComponent component
	if (Path)
	{
		const int SideOffset = SideOfRoadParam ? 500 : -500;
		const float Distance = Path->GetDistanceAlongSplineAtLocation(AILocation, ESplineCoordinateSpace::World);
		const FVector RightVector = Path->GetRightVectorAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
		return Path->GetLocationAtDistanceAlongSpline(Distance + AdditionalDistance, ESplineCoordinateSpace::World) + RightVector * SideOffset;
	}
	
	UE_LOG(LogAIController, Warning, TEXT("Failed to find USplineComponent"));
	return FVector::ZeroVector; // Return a default value (zero vector) if Path is null
}

void AVehicleAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ControlledVehicle->OnSteeringAction(CalculateSteering());
	auto [Throttle, Brake] = CalculateThrottleAndBrake(CalculateTopSpeed());
	ControlledVehicle->OnThrottleAction(Throttle);
	ControlledVehicle->OnBrakeAction(Brake);
	CheckForOvertakes();
}

float AVehicleAIController::CalculateSteering() const
{
	const float AdditionalDistance = GetAdditionalDistanceForSteering(ControlledVehicle->GetAISettings().SpeedRangeForSteering,
		ControlledVehicle->GetAISettings().DistanceRangeForSteering);
	const FRotator ClosestPointToTheSpline = GetClosestPointToTheSpline(AdditionalDistance);
	const FRotator AngleOfSteering = GetAngleOfSteering(ClosestPointToTheSpline);
	return GetSteeringInput(ControlledVehicle->GetAISettings().AngleRangeForSteering, ControlledVehicle->GetAISettings().SteeringInputRange,
		AngleOfSteering);
}

float AVehicleAIController::GetAdditionalDistanceForSteering(FVector2D SpeedRangeForSteering,
	FVector2D DistanceRangeForSteering) const
{
	return FMath::GetMappedRangeValueClamped(SpeedRangeForSteering, DistanceRangeForSteering,
		ControlledVehicle->GetCurrentSpeed());
}

FRotator AVehicleAIController::GetClosestPointToTheSpline(float AdditionalDistance) const
{
	return UKismetMathLibrary::FindLookAtRotation(ControlledVehicle->GetFrontOfVehicle(),
	GetClosestLocationToPath(ControlledVehicle->GetFrontOfVehicle(), AdditionalDistance, SideOfRoad));
}

FRotator AVehicleAIController::GetAngleOfSteering(const FRotator& ClosestPointToTheSpline) const
{
	return UKismetMathLibrary::NormalizedDeltaRotator(ClosestPointToTheSpline, ControlledVehicle->GetActorRotation());
}

float AVehicleAIController::GetSteeringInput(FVector2D AngleRangeForSteering, FVector2D SteeringInputRange,
	const FRotator& AngleOfSteering)
{
	return FMath::GetMappedRangeValueClamped(AngleRangeForSteering, SteeringInputRange, AngleOfSteering.Yaw);
}

std::pair<float, float> AVehicleAIController::CalculateThrottleAndBrake(float TopSpeed) const
{
	return (ControlledVehicle->GetCurrentSpeed() >= TopSpeed) ? std::make_pair(0.0f, CalculateBrakeIntensity(CalculateTopSpeed())) : std::make_pair(1.0f, 0.0f);
}

float AVehicleAIController::CalculateBrakeIntensity(float TopSpeed) const
{
	const FVector2D SpeedRange(TopSpeed + 1.0f, TopSpeed + 5.0f);
	const FVector2D IntensityRange(0.0f, 1.0f);
	return FMath::GetMappedRangeValueClamped(SpeedRange, IntensityRange, ControlledVehicle->GetCurrentSpeed());
}

float AVehicleAIController::CalculateTopSpeed() const
{
	if(IsOverrideTopSpeed)
	{
		return OverrideTopSpeed;
	}
	
	const float AdditionalDistance = GetAdditionalDistanceForTopSpeed(ControlledVehicle->GetAISettings().SpeedRangeForTopSpeed,
		ControlledVehicle->GetAISettings().DistanceRangeForTopSpeed);
	const FRotator ClosestPointToTheSpline = GetClosestPointToTheSpline(AdditionalDistance);
	const FRotator AngleOfSteering = GetAngleOfSteering(ClosestPointToTheSpline);
	return GetTopSpeed(ControlledVehicle->GetAISettings().AngleRangeForTopSpeed, ControlledVehicle->GetAISettings().TopSpeedRange, AngleOfSteering);
}

float AVehicleAIController::GetAdditionalDistanceForTopSpeed(FVector2D SpeedRangeForTopSpeed,
	FVector2D DistanceRangeForTopSpeed) const
{
	return FMath::GetMappedRangeValueClamped(SpeedRangeForTopSpeed, DistanceRangeForTopSpeed,
		ControlledVehicle->GetCurrentSpeed());
}

float AVehicleAIController::GetTopSpeed(FVector2D AngleRangeForTopSpeed, FVector2D TopSpeedRange,
	const FRotator& AngleOfSteering)
{
	return FMath::GetMappedRangeValueClamped(AngleRangeForTopSpeed, TopSpeedRange,
		FMath::Abs(AngleOfSteering.Yaw));
}

void AVehicleAIController::CheckForOvertakes() 
{
	const FHitResult FrontHitResult = GetFrontHitResult();
	ProcessFrontHit(FrontHitResult);
}

FHitResult AVehicleAIController::GetFrontHitResult()
{
	FHitResult FrontHitResult;
	FVector StartLocation = ControlledVehicle->GetFrontOfVehicle();
	FVector EndLocation = StartLocation + ControlledVehicle->GetActorForwardVector() * DetectionDistance;

	FCollisionShape CollisionShape = FCollisionShape::MakeBox(FVector(400.0f, 180.0f, 180.0f));
	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(ControlledVehicle.Get());

	GetWorld()->SweepSingleByObjectType(FrontHitResult, StartLocation, EndLocation, FQuat::Identity,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_Vehicle), CollisionShape, CollisionParams);
	
	return FrontHitResult;
}

void AVehicleAIController::ProcessFrontHit(const FHitResult& FrontHitResult)
{
	if (FrontHitResult.bBlockingHit && FrontHitResult.GetActor()->IsA<AVehiclePawn>())
	{
		AVehiclePawn* VehicleInFront = Cast<AVehiclePawn>(FrontHitResult.GetActor());
		OverrideTopSpeed = VehicleInFront->GetCurrentSpeed();
		IsOverrideTopSpeed = true;
	}
	else
	{
		IsOverrideTopSpeed = false;
	}
}

