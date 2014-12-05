// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "FPSProjectile.generated.h"

/**
 * 
 */
class UProjectileMovementComponent;

UCLASS()
class FPSPROJECT_API AFPSProjectile : public AActor
{
	GENERATED_UCLASS_BODY()


	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		TSubobjectPtr<USphereComponent> CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
		TSubobjectPtr<UProjectileMovementComponent> ProjectileMovement;

		void InitVelocity(const FVector &ShootDirection);

		UFUNCTION()
			void OnHit(class AActor *otherActor, class UPrimitiveComponent *otherComp,
			FVector normalImpulse, const FHitResult &hit);


};
