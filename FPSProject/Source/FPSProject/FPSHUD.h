// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "FPSHUD.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API AFPSHUD : public AHUD
{
	GENERATED_UCLASS_BODY()

	virtual void DrawHUD() override;


private:
	UTexture2D *CrosshairTex;	
};
