// Fill out your copyright notice in the Description page of Project Settings.


#include "FinishTrigger.h"
#include "Engine/Engine.h"
#include "GreenPlayerController.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"



AFinishTrigger::AFinishTrigger()
{
	OnActorBeginOverlap.AddDynamic(this, &AFinishTrigger::OnOverlapBegin);
	
}

void AFinishTrigger::BeginPlay()
{
	Super::BeginPlay();

}

void AFinishTrigger::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && (OtherActor != this))
	{
		PlayerPtr = Cast<AGreenPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

		if (PlayerPtr) {
			PlayerPtr->SetFinishHUD();
		}
	}
} 