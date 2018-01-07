// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "Engine/World.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankTrack::BeginPlay()
{
	//Super::BeginPlay();
	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

void UTankTrack::ApplySidewaysForce()
{
	auto DeltaTime = GetWorld()->GetDeltaSeconds();

	/** NOTES ==================================================================== */
	// FIND THE SLIPPAGE SPEED
	// - The component of speed in the tank right direction
	// - If there is no slippage, then it should be defaulted to 0
	// - If sliding sideways, the slippage speed should be exactly the same as the
	//	 tank.
	// - Use cos of the angle between the velocity of the tank (whichever
	//   direction it is actually going in) and the sideways right vector. So, if
	//   it is going perfectly sideways, the cos between the two angles would be 1.
	//   If it is going forward then the cos would be 0.
	// - Use FVector::DotProduct() between velocity of tank component
	//   (GetComponentVelocity()) and the tank right vector (GetRightVector).
	/** ========================================================================== */

	auto SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());

	// work-out the required acceleration this frame to correct (applied in the
	// opposite direction)
	auto CorrectionAcceleration = -SlippageSpeed / DeltaTime * GetRightVector();

	// calculate and apply sideways force (Force = Mass * Acceleration)
	// get mass of tank via the TankRoot (need static mesh component to
	// get the mass)
	auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());

	// divided by two because there are two tracks
	auto CorrectionForce = (TankRoot->GetMass() * CorrectionAcceleration) / 2;
	TankRoot->AddForce(CorrectionForce);
}

void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, FVector NormalImpulse,
	const FHitResult& Hit)
{
	DriveTrack();
	ApplySidewaysForce();
	CurrentThrottle = 0;
}

void UTankTrack::SetThrottle(float Throttle)
{
	CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + Throttle, -1, 1);
}

void UTankTrack::DriveTrack()
{
	auto ForceApplied = GetForwardVector() * CurrentThrottle * TrackMaxDrivingForce;
	auto ForceLocation = GetComponentLocation();
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}
