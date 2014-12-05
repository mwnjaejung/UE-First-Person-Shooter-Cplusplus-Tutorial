// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProject.h"
#include "FPSCharacter.h"
#include "FPSProjectile.h"


AFPSCharacter::AFPSCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// 캐릭터 몸통 초기화
	FirstPersonCameraComponent = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachParent = CapsuleComponent;
	FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 50 + BaseEyeHeight);
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// 일인칭 시점 캐릭터 팔 초기화.
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
		// 디버그 메세지를 화면에 출력한다.
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "We ar using FPSCharacter!");
	}

}


void AFPSCharacter::SetupPlayerInputComponent(class UInputComponent *input)
{
	// 입력 바인딩.
	input->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	input->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);
	input->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	input->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);
	input->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::OnStartJump);
	input->BindAction("Jump", IE_Released, this, &AFPSCharacter::OnStopJump);
	input->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::OnFire);
}


// 앞뒤로 이동
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


// 좌우 이동
void AFPSCharacter::MoveRight(float value)
{
	if (Controller && (value != 0))
	{
		const FRotator rotation = Controller->GetControlRotation();
		const FVector direction = FRotationMatrix(rotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(direction, value);
	}
}


// 점프 시작.
void AFPSCharacter::OnStartJump()
{
	bPressedJump = true;
}


void AFPSCharacter::OnStopJump()
{
	bPressedJump = false;
}


// 총알을 쏜다.
void AFPSCharacter::OnFire()
{
	if (ProjectileClass)
	{
		FVector cameraLoc;
		FRotator cameraRot;
		GetActorEyesViewPoint(cameraLoc, cameraRot);
		
		// 총알 위치 계산 (카메라에서 특정 값 만큼 이동 시킨다.)
		FVector const muzzelLocation = cameraLoc + FTransform(cameraRot).TransformVector(MuzzleOffset);

		// 총알이 발사될 각도를 계산한다.  (카메라가 바라보는 방향보다 조금 더 윗 방향으로 회전)
		FRotator muzzleRotation = cameraRot;
		muzzleRotation.Pitch += 10.f;

		// 총알 생성.
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
				// 총알이 날아 갈 방향 설정.
				FVector const launchDir = muzzleRotation.Vector();
				projectile->InitVelocity(launchDir);
			}
		}
	}
}
