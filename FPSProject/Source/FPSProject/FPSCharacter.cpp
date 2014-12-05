// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProject.h"
#include "FPSCharacter.h"
#include "FPSProjectile.h"


AFPSCharacter::AFPSCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// ĳ���� ���� �ʱ�ȭ
	FirstPersonCameraComponent = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachParent = CapsuleComponent;
	FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 50 + BaseEyeHeight);
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// ����Ī ���� ĳ���� �� �ʱ�ȭ.
	FirstPersonMesh = PCIP.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->AttachParent = FirstPersonCameraComponent;
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = false;

}


void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine)
	{
		// ����� �޼����� ȭ�鿡 ����Ѵ�.
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "We ar using FPSCharacter!");
	}

}


void AFPSCharacter::SetupPlayerInputComponent(class UInputComponent *input)
{
	// �Է� ���ε�.
	input->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	input->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);
	input->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	input->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);
	input->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::OnStartJump);
	input->BindAction("Jump", IE_Released, this, &AFPSCharacter::OnStopJump);
	input->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::OnFire);
}


// �յڷ� �̵�
void AFPSCharacter::MoveForward(float value)
{
	if ((Controller != NULL) && (value != 0.0f))
	{
		FRotator rotation = Controller->GetControlRotation();
		if (CharacterMovement->IsMovingOnGround() || CharacterMovement->IsFalling())
		{
			rotation.Pitch = 0;
		}

		const FVector direction = FRotationMatrix(rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(direction, value);
	}
}


// �¿� �̵�
void AFPSCharacter::MoveRight(float value)
{
	if (Controller && (value != 0))
	{
		const FRotator rotation = Controller->GetControlRotation();
		const FVector direction = FRotationMatrix(rotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(direction, value);
	}
}


// ���� ����.
void AFPSCharacter::OnStartJump()
{
	bPressedJump = true;
}


void AFPSCharacter::OnStopJump()
{
	bPressedJump = false;
}


// �Ѿ��� ���.
void AFPSCharacter::OnFire()
{
	if (ProjectileClass)
	{
		FVector cameraLoc;
		FRotator cameraRot;
		GetActorEyesViewPoint(cameraLoc, cameraRot);
		
		// �Ѿ� ��ġ ��� (ī�޶󿡼� Ư�� �� ��ŭ �̵� ��Ų��.)
		FVector const muzzelLocation = cameraLoc + FTransform(cameraRot).TransformVector(MuzzleOffset);

		// �Ѿ��� �߻�� ������ ����Ѵ�.  (ī�޶� �ٶ󺸴� ���⺸�� ���� �� �� �������� ȸ��)
		FRotator muzzleRotation = cameraRot;
		muzzleRotation.Pitch += 10.f;

		// �Ѿ� ����.
		UWorld *const world = GetWorld();
		if (world)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			spawnParams.Instigator = Instigator;
			AFPSProjectile * const projectile = world->SpawnActor<AFPSProjectile>(ProjectileClass,
				muzzelLocation, muzzleRotation, spawnParams);
			if (projectile)
			{
				// �Ѿ��� ���� �� ���� ����.
				FVector const launchDir = muzzleRotation.Vector();
				projectile->InitVelocity(launchDir);
			}
		}
	}
}
