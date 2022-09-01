// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "FinishTrigger.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGREEN_API AFinishTrigger : public ATriggerVolume
{
	GENERATED_BODY()

public:

	AFinishTrigger();

protected:

	virtual void BeginPlay() override;

private:

	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	
};
