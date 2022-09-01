// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleComponent.h"
#include "Walls.generated.h"

UCLASS()
class PROJECTGREEN_API AWalls : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWalls();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collide")
	class UDestructibleComponent* DMesh;

	class AMainCharacter* Main;
	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

};
