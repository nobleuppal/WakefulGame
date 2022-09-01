// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GreenPlayerController.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTGREEN_API AGreenPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	// Refernce to UMG asset in the editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> HUDOverlayAsset;

	// Variable to hold the widget after creating it
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* HUDOverlay;

protected:

	virtual void BeginPlay() override;

};
