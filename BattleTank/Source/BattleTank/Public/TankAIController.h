// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

protected:
	// how close the AI can get to the player when moving
	UPROPERTY(EditAnywhere, Category = "Setup") // consider EditDefaultsOnly
	float AcceptanceRadius = 3000;

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
