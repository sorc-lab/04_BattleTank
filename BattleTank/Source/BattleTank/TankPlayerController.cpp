// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Tank.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "BattleTank.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto ControlledTank = GetControlledTank();
	if (!ControlledTank) {
		UE_LOG(LogTemp, Error, TEXT("PlayerController not possesing a tank"));
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Player Controlled Tank possesed by %s"),
			*(ControlledTank->GetName()));
	}
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetControlledTank()) { return; }

	FVector HitLocation; // out parameter
	if (GetSightRayHitLocation(HitLocation)) // has side-effect, is going to line trace
		GetControlledTank()->AimAt(HitLocation);
}

// get world location of linetrace through crosshair, true if it hits landscape
bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{
	// find the crosshair position in pixel coordinates
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	auto ScreenLocation = FVector2D(ViewportSizeX * CrosshairXLocation,
		ViewportSizeY * CrosshairYLocation);
	
	// de-project the screen position of the crosshair to a world direction
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection)) {
		// Log LookDirection (world direction = direction from camera going
		// through crosshair into the world).
		//UE_LOG(LogTemp, Warning, TEXT("Look direction: %s"), *LookDirection.ToString());

		// line-trace along LookDirection and see what we hit (up to max range)
		GetLookVectorHitLocation(LookDirection, HitLocation);
	}
	
	return true;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation,
	FVector& LookDirection) const
{
	FVector CameraWorldLocation; // to be discarded
	DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y,
		CameraWorldLocation, LookDirection);
	return true;
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection,
	FVector& HitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation,
		ECollisionChannel::ECC_Visibility))
	{
		HitLocation = HitResult.Location;
		return true;
	} else {
		HitLocation = FVector(0);
		return false;
	}
}

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}
