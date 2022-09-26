// Fill out your copyright notice in the Description page of Project Settings.


#include "FinishTrigger.h"
#include "Engine/Engine.h"


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
		
		GEngine->AddOnScreenDebugMessage(0, 9.0f, FColor::Emerald, FString(TEXT("FINISH!!!!!!")));
	
		//UE_LOG(LogTemp, Warning, TEXT("Finished!!!!"));
	}
} 