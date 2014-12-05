// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProject.h"
#include "FPSProjectile.h"


AFPSProjectile::AFPSProjectile(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	CollisionComp = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(15.f);
	// �Ѿ� �ɼ� ���������� �����Ѵ�.
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	
	// �Ѿ��� �ε�ĥ �� ȣ���� �Լ��� ����Ѵ�.
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
		// �Ѿ��� ������Ʈ�� �ε����� ��, �浹�� ������Ʈ�� ����� ���Ѵ�.
		otherComp->AddImpulseAtLocation(ProjectileMovement->Velocity * 100.f, hit.ImpactPoint);
	}
}



