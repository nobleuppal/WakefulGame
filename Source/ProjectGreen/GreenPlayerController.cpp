  // Fill out your copyright notice in the Description page of Project Settings.

#include "GreenPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"


void AGreenPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetHUD();
}

void AGreenPlayerController::SetHUD()
{
	if (HUDOverlayAsset)
	{
		
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}

	HUDOverlay->AddToViewport();
	HUDOverlay->WidgetTree->FindWidget("Finish")->SetVisibility(ESlateVisibility::Hidden);
	HUDOverlay->WidgetTree->FindWidget("Restart_Button")->SetVisibility(ESlateVisibility::Hidden);
}

void AGreenPlayerController::SetFinishHUD()
{
	HUDOverlay->WidgetTree->FindWidget("Crosshair")->SetVisibility(ESlateVisibility::Hidden);
	HUDOverlay->WidgetTree->FindWidget("Finish")->SetVisibility(ESlateVisibility::Visible);


	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}
