// Fill out your copyright notice in the Description page of Project Settings.

#include "Vehicle/VehiclePawn.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "ChaosWheeledVehicleMovementComponent.h"

AVehiclePawn::AVehiclePawn()
{
	InitializeMesh();
	InitializeSpringArm();
	InitializeCamera();
	InitializeMovementComponent();
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
