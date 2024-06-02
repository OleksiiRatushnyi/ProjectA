// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/VehicleHUD.h"

void UVehicleHUD::UpdateSpeed(float NewSpeed)
{
	const float FloatSpeed = FMath::Abs(NewSpeed) *  0.036f;
	const int Speed = static_cast<int>(FloatSpeed);
	OnSpeedUpdate(Speed);
}