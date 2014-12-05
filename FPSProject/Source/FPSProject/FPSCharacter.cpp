// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProject.h"
#include "FPSCharacter.h"


AFPSCharacter::AFPSCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	FirstPersonCameraComponent = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachParent = CapsuleComponent;
	FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 50 + BaseEyeHeight);
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

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
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "We ar using FPSCharacter!");
	}

}


void AFPSCharacter::SetupPlayerInputComponent(class UInputComponent *input)
{
	input->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	input->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);
	input->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	input->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);
	input->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::OnStartJump);
	input->BindAction("Jump", IE_Released, this, &AFPSCharacter::OnStopJump);
	input->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::OnFire);
}


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


void AFPSCharacter::MoveRight(float value)
{
	if (Controller && (value != 0))
	{
		const FRotator rotation = Controller->GetControlRotation();
		const FVector direction = FRotationMatrix(rotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(direction, value);
	}
}


void AFPSCharacter::OnStartJump()
{
	bPressedJump = true;
}


void AFPSCharacter::OnStopJump()
{
	bPressedJump = false;
}


void AFPSCharacter::OnFire()
{

}