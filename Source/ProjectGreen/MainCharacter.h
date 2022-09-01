// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UENUM(BlueprintType)
enum class EPunchStatus : uint8
{
	EPS_Normal UMETA(DisplayName = "Normal"),
	EPS_Punch1 UMETA(DisplayName = "Punch1"),
	EPS_Punch2 UMETA(DisplayName = "Punch1"),
	EPS_Punch3 UMETA(DisplayName = "Punch1"),
	EPS_Punch4 UMETA(DisplayName = "Punch1"),
	EPS_Punch5 UMETA(DisplayName = "Punch1"),
	EPS_Punch6 UMETA(DisplayName = "Punch1"),
	EPS_Kick1 UMETA(DisplayName = "Kick1"),

	EPS_Default UMETA(DisplayName = "Default")

};
UCLASS()
class PROJECTGREEN_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UTargetArm* TargetBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	//class AWalls* Wall;

	// turns the camera at this rate
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpAtRate;

	// called for forwards/backwards input
	void MoveForward(float Value);

	//called for side to side input
	void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void LookUpRate(float Rate);


	FORCEINLINE class UTargetArm* GetTargetBoom() const { return TargetBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return Camera; }
	


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anims")
	bool bAttacking;

	void Punch1();
	void Punch2();
	void Punch3();
	void Punch4();
	void Punch5();
	void Punch6();

	void Kick1();

	void CannonAnim();

	bool CountDown;

	FTimerHandle NextAttack;
	
	class AWalls *Wall;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* CombatMontage;

	void ShiftDown();
	void ShiftUp();

	void EnterDown();
	void EnterUp();

	void LMBDown();
	void EraserCannon();
	void LMBUp();

	void LDown();
	void LUp();
	
	void CalcClose();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EPunchStatus PunchStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	class UBoxComponent* ArmCollisionR;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	UBoxComponent* ArmCollisionL;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	class UParticleSystemComponent* Beam;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	class URadialForceComponent* RadialR;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	URadialForceComponent* RadialL;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	URadialForceComponent* RadialB;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Targeting")
	TArray<AWalls*> WallArr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Targeting")
	TArray<AWalls*> WallArrClose;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Targeting")
	class UWidgetComponent* LockCursor;

	

	FHitResult OutHit;
	int32 Wallnumber;
	

	void SetToNearestTarget(TArray<AWalls*> WallsinRange, int32 Wallnumber);
	

	UPROPERTY(EditAnywhere)
	bool hasTarget;
	bool bCamRot;
	bool bLockedOn;
	bool isHit;
	bool bMoving;
	UPROPERTY(EditAnywhere ,Category = "LockOn")
	float MaxTargetDist;

};
