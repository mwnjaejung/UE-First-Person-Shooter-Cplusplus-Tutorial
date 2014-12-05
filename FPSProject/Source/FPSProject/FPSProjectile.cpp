// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProject.h"
#include "FPSProjectile.h"


AFPSProjectile::AFPSProjectile(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	CollisionComp = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(15.f);
	// 총알 옵션 프로파일을 설정한다.
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	
	// 총알이 부딪칠 때 호출할 함수를 등록한다.
	CollisionComp->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);

	RootComponent = CollisionComp;

	ProjectileMovement = PCIP.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.3f;

}


void AFPSProjectile::InitVelocity(const FVector &ShootDirection)
{
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
	}
}


void AFPSProjectile::OnHit(class AActor *otherActor, class UPrimitiveComponent *otherComp,
	FVector normalImpulse, const FHitResult &hit)
{
	if (otherActor && (otherActor != this) && otherComp)
	{
		// 총알이 오브젝트에 부딪쳤을 때, 충돌한 오브젝트에 충격을 가한다.
		otherComp->AddImpulseAtLocation(ProjectileMovement->Velocity * 100.f, hit.ImpactPoint);
	}
}



