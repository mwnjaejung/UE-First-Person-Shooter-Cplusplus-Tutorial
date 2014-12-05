// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProject.h"
#include "FPSGameMode.h"
#include "FPSCharacter.h"
#include "FPSHUD.h"



AFPSGameMode::AFPSGameMode(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	//DefaultPawnClass = AFPSCharacter::StaticClass();
	static ConstructorHelpers::FObjectFinder<UBlueprint> PlayerPawnObject(
		TEXT("BluePrint'/Game/BluePrints/BP_FPSCharacter.BP_FPSCharacter'"));

	// 기본 캐릭터 설정.
	if (PlayerPawnObject.Object != nullptr)
	{
		DefaultPawnClass = (UClass*)PlayerPawnObject.Object->GeneratedClass;
	}

	// 기본 HUD 설정.
	HUDClass = AFPSHUD::StaticClass();

}


void AFPSGameMode::StartPlay()
{
	Super::StartPlay();

	StartMatch();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Hello World"));
	}

}
