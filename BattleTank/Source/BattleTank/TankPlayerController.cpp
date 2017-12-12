// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
//#include "Math/Vector2D.h"
#include "BattleTank.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto ControlledTank = GetControlledTank();
	if (!ControlledTank) {
		UE_LOG(LogTemp, Error, TEXT("PlayerController not possesing a tank"));
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Player Controlled Tank possesed by %s"), *(ControlledTank->GetName()));
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
	if (GetSightRayHitLocation(HitLocation)) { // has side-effect, is going to line trace
		//UE_LOG(LogTemp, Warning, TEXT("Look direction: %s"), *HitLocation.ToString());
		// TODO: Tell controlled tank to aim at this point
	}
}

// get world location of linetrace through crosshair, true if it hits landscape
bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
	// find the crosshair position
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	auto ScreenLocation = FVector2D(ViewportSizeX * CrosshairXLocation,
		ViewportSizeY * CrosshairYLocation);
	
	FVector CameraWorldLocation;
	FVector WorldDirection;
	if (DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y,
		CameraWorldLocation, WorldDirection))
		UE_LOG(LogTemp, Warning, TEXT("Look direction: %s"), *WorldDirection.ToString());
	
	// line-trace along look direction and see what we hit (up to max range)
	return true;
}

ATank* ATankPlayerController::GetControlledTank() const { return Cast<ATank>(GetPawn()); }
