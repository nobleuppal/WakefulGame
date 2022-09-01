// Fill out your copyright notice in the Description page of Project Settings.


#include "Walls.h"
#include "Components/BoxComponent.h"
#include "Engine.h"
#include "MainCharacter.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AWalls::AWalls()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	DMesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DMesh"));

	
	
}



// Called when the game starts or when spawned
void AWalls::BeginPlay()
{
	Super::BeginPlay();


	/* for (TActorIterator<AMainCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		Main = Cast<AMainCharacter>(*ActorItr);
	} */
	
}



// Called every frame
void AWalls::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*FRotator NewRotation;
	
	//UE_LOG(LogTemp, Warning, TEXT("Location: %s"), *GetActorLocation().ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Location: %s"), *Main->GetActorLocation().ToString());

	if (Main)
	{
		NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Main->GetActorLocation());
	}
	//UE_LOG(LogTemp, Warning, TEXT("Rotation: %s"), *NewRotation.Vector().ToString());

	SetActorRotation(NewRotation); */
}

