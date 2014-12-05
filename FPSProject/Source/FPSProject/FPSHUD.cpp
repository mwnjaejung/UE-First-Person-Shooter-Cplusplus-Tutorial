// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProject.h"
#include "FPSHUD.h"


AFPSHUD::AFPSHUD(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(
		TEXT("Texture2D'/Game/Texture/crosshair.crosshair'"));
		//TEXT("Texture2D'/Game/Texture/crosshair2.crosshair2'"));
	CrosshairTex = CrosshairTexObj.Object;
}


void AFPSHUD::DrawHUD()
{
	Super::DrawHUD();

	// 텍스쳐 출력.
	const FVector2D center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	const FVector2D crosshairDrawPosition( 
		center.X - (CrosshairTex->GetSurfaceWidth() * 0.5f),
		center.Y - (CrosshairTex->GetSurfaceHeight() * 0.5f));
	FCanvasTileItem tileItem(crosshairDrawPosition, CrosshairTex->Resource,
		FLinearColor::White);
	tileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(tileItem);
}
