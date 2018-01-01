// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"
#include "Engine/World.h"

void UTankMovementComponent::Initialize(UTankTrack* LeftTrackToSet,
	UTankTrack* RightTrackToSet)
{
	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;
}


void UTankMovementComponent::IntendMoveForward(float Throw)
{
	if (!LeftTrack || !RightTrack) { return; }
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);
}

void UTankMovementComponent::IntendTurnRight(float Throw)
{
	if (!LeftTrack || !RightTrack) { return; }
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(-Throw);
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	// No need to call Super since we're replacing the functionality

	/** NOTES */
	// AI Tank will start w/ X axis facing the player tank
	// It will move forward @ +1 until it reaches a 90 degree angle w/ player tank
	// AI Tank will stop moving forward because it will be facing the player tank
	// If AI is at a 180 degree angle w/ player tank it will move @ -1 (backwards)
	// 360 degrees goes back to +1 (forward)
	//
	// Going to use The Vector Dot Product ( A * B = ||A|| ||B|| cos(0),
	// where 0 is the angle between A and B.
	// In particular, if A and B are orthogonal, then the angle between them is 90
	// degrees and A * B = 0.
	// At the other extreme, if they are codirectional, then the angle between
	// them is 0 degrees and A * B = ||A|| ||B||
	// Magnitude of A (TankForward) * magnitude of B (AIForwardIntention) * cos(0) of the angle between them
	//
	// Type Dot | Input 2x FVector | Output Float
	// Projection of one vector onto the other, "parallelness"
	// Maximum when vectors are parallel, and zero when they are perpendicular.  Cos 0.

	auto TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
	auto AIForwardIntention = MoveVelocity.GetSafeNormal();

	auto ForwardThrow = FVector::DotProduct(TankForward, AIForwardIntention);
	IntendMoveForward(ForwardThrow);

	auto RightThrow = FVector::CrossProduct(TankForward, AIForwardIntention).Z;
	IntendTurnRight(RightThrow);
}
