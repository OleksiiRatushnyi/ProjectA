// Fill out your copyright notice in the Description page of Project Settings.

#include "Vehicle/VehiclePawn.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "ChaosWheeledVehicleMovementComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

/*
 * Common Vehicle Functionality
 */
AVehiclePawn::AVehiclePawn()
{
	InitializeMesh();
	InitializeSpringArm();
	InitializeCamera();
	InitializeMovementComponent();
	InitializeFrontPoint();
}

void AVehiclePawn::InitializeMesh() const
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(FName("Vehicle"));
}

void AVehiclePawn::InitializeSpringArm()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetMesh());
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->SocketOffset.Z = 150.0f;
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 130.0f));
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 2.0f;
	SpringArm->CameraLagMaxDistance = 50.0f;
}

void AVehiclePawn::InitializeCamera()
{
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform);
}

void AVehiclePawn::InitializeMovementComponent()
{
	MovementComponent = CastChecked<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());
}

void AVehiclePawn::InitializeFrontPoint()
{
	FrontPoint = CreateDefaultSubobject<USceneComponent>(TEXT("FrontPoint"));
	FrontPoint->SetupAttachment(RootComponent);
}

FVector AVehiclePawn::GetFrontOfVehicle()
{
	return FrontPoint->GetComponentLocation();
}

void AVehiclePawn::Tick(float Delta)
{
	Super::Tick(Delta);
	
	ApplyAngularDamping();
	CameraUpdate(Delta);
}

void AVehiclePawn::ApplyAngularDamping()
{
	// add some angular damping if the vehicle is in midair
	bool bMovingOnGround = MovementComponent->IsMovingOnGround();
	GetMesh()->SetAngularDamping(bMovingOnGround ? 0.0f : 3.0f);
}

void AVehiclePawn::CameraUpdate(float Delta)
{
	float CameraYaw = SpringArm->GetRelativeRotation().Yaw;
	static float TargetYaw = 0.0f;
	CameraYaw = FMath::FInterpTo(CameraYaw, TargetYaw, Delta, 4.0f);
	SpringArm->SetRelativeRotation(FRotator(0.0f, CameraYaw, 0.0f));
}

float AVehiclePawn::GetCurrentSpeed()
{
	return MovementComponent->GetForwardSpeed() * 0.036;
}

/*
 * Player Input Handling
 */
void AVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(InputActions.ThrottleAction, ETriggerEvent::Triggered, this, &AVehiclePawn::OnThrottleAction);
		EnhancedInputComponent->BindAction(InputActions.ThrottleAction, ETriggerEvent::Completed, this, &AVehiclePawn::OnThrottleAction);

		EnhancedInputComponent->BindAction(InputActions.BrakeAction, ETriggerEvent::Triggered, this, &AVehiclePawn::OnBrakeAction);
		EnhancedInputComponent->BindAction(InputActions.BrakeAction, ETriggerEvent::Completed, this, &AVehiclePawn::OnBrakeAction);

		EnhancedInputComponent->BindAction(InputActions.HandbrakeAction, ETriggerEvent::Triggered, this, &AVehiclePawn::OnHandbrakeAction);
		EnhancedInputComponent->BindAction(InputActions.HandbrakeAction, ETriggerEvent::Completed, this, &AVehiclePawn::OnHandbrakeAction);
	
		EnhancedInputComponent->BindAction(InputActions.SteeringAction, ETriggerEvent::Triggered, this, &AVehiclePawn::OnSteeringAction);
		EnhancedInputComponent->BindAction(InputActions.SteeringAction, ETriggerEvent::Completed, this, &AVehiclePawn::OnSteeringAction);
	}
}

void AVehiclePawn::OnThrottleAction(const FInputActionValue& Value)
{
	if(bIsMovemenetEnabled)
	{
		MovementComponent->SetThrottleInput(Value.Get<float>());
	}
}

void AVehiclePawn::OnBrakeAction(const FInputActionValue& Value)
{
	if(bIsMovemenetEnabled)
	{
		MovementComponent->SetBrakeInput(Value.Get<float>());
	}
}

void AVehiclePawn::OnHandbrakeAction(const FInputActionValue& Value)
{
	MovementComponent->SetHandbrakeInput(Value.Get<float>() >= 0.5f);
}

void AVehiclePawn::OnSteeringAction(const FInputActionValue& Value)
{
	if(bIsMovemenetEnabled)
	{
		MovementComponent->SetSteeringInput(Value.Get<float>());
	}
}

/*
 * Action processing for AI
 */
void AVehiclePawn::OnThrottleAction(float Value)
{
	if(bIsMovemenetEnabled)
	{
		MovementComponent->SetThrottleInput(Value);
	}
}

void AVehiclePawn::OnBrakeAction(float Value)
{
	MovementComponent->SetBrakeInput(Value);
}

void AVehiclePawn::OnSteeringAction(float Value)
{
	if(bIsMovemenetEnabled)
	{
		MovementComponent->SetSteeringInput(Value);
	}
}
