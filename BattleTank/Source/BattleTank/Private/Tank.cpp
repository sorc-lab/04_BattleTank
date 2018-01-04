// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "Projectile.h"
#include "Components/SceneComponent.h"
#include "TankBarrel.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "BattleTank.h"

void ATank::Fire()
{
	if (!ensure(Barrel)) { return; }
	bool bIsReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;
	if (bIsReloaded) {
		// spawn a projectile at the socket location on barrel
		// 'T *UWorld::SpawnActor<AProjectile>(UClass*,const FVector&,const FRotator&,const FActorSpawnParameters&)'
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile")));

		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
	}
}

// Sets default values (CONSTRUCTOR)
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ATank::BeginPlay()
{
	Super::BeginPlay(); // needed for BP Begin Play to run
	TankAimingComponent = FindComponentByClass<UTankAimingComponent>();
}

void ATank::AimAt(FVector HitLocation)
{
	if (!ensure(TankAimingComponent)) { return; }
	TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
}
