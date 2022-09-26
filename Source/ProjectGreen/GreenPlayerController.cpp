  // Fill out your copyright notice in the Description page of Project Settings.

#include "GreenPlayerController.h"
#include "Blueprint/UserWidget.h"

void AGreenPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDOverlayAsset)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}

	HUDOverlay->AddToViewport();	
}