// Fill out your copyright notice in the Description page of Project Settings.
// By Noblejit Uppal

#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/Engine.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Walls.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "TargetArm.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "Engine/Engine.h"
// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Camera Boom (pulls towards player if there's collision)
	TargetBoom = CreateDefaultSubobject<UTargetArm>(TEXT("TargetBoom"));
	TargetBoom->SetupAttachment(GetRootComponent());
	TargetBoom->TargetArmLength = 200.f; // camera follows at this distance
	TargetBoom->bUsePawnControlRotation = true; // rotate arm based on controller
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	Camera->SetupAttachment(TargetBoom, UTargetArm::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match
	// the controller oreintation
	Camera->bUsePawnControlRotation = false;


	BaseTurnRate = 65.f;
	BaseLookUpAtRate = 65.f;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // character moves in direction of input
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f); // at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->AirControl = 0.5f;

	bAttacking = false;
	CountDown = false;

	PunchStatus = EPunchStatus::EPS_Normal;

	ArmCollisionR = CreateDefaultSubobject<UBoxComponent>(TEXT("ArmCollisionR"));
	ArmCollisionL = CreateDefaultSubobject<UBoxComponent>(TEXT("ArmCollisionL"));

	ArmCollisionR->SetupAttachment(GetMesh(), FName("RightHandSocket"));
	ArmCollisionL->SetupAttachment(GetMesh(), FName("LeftHandSocket"));

	RadialR = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialR"));
	RadialL = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialL"));
	RadialB = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialB"));

	RadialR->SetupAttachment(GetMesh(), FName("RightHandSocket"));
	RadialL->SetupAttachment(GetMesh(), FName("LeftHandSocket"));

	Beam = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Beam"));

	isHit = false;
	hasTarget = false;
	bCamRot = true;
	bLockedOn = false;
	
	MaxTargetDist = 7000.f;
	Wallnumber= 0;

	LockCursor = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockCursor"));


	

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	
	ArmCollisionR->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	ArmCollisionR->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	ArmCollisionR->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	ArmCollisionL->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	ArmCollisionL->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	ArmCollisionL->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	for (TActorIterator<AWalls> ActorItr (GetWorld()); ActorItr; ++ActorItr)
	{
		WallArr.Add(*ActorItr);
	} 

	


}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetVelocity().X != 0.0f && GetVelocity().Y != 0.0f)
	{
		CalcClose();
	}

	if (GetActorLocation().Z < -1700.0f)
	{
		GEngine->AddOnScreenDebugMessage(0, 9.0f, FColor::Emerald, FString(TEXT("You Lose!!!!!!")));
		Destroy();
	}

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

	PlayerInputComponent->BindAxis("TurnRate", this, &AMainCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMainCharacter::LookUpRate);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Punch", IE_Pressed, this, &AMainCharacter::ShiftDown);

	PlayerInputComponent->BindAction("Kick", IE_Pressed, this, &AMainCharacter::EnterDown);

	PlayerInputComponent->BindAction("DeathBeam", IE_Pressed, this, &AMainCharacter::LMBDown);
	PlayerInputComponent->BindAction("DeathBeam", IE_Released, this, &AMainCharacter::LMBUp);

	PlayerInputComponent->BindAction("LockOn", IE_Pressed, this, &AMainCharacter::LDown);
	PlayerInputComponent->BindAction("LockOn", IE_Released, this, &AMainCharacter::LUp);


}



void AMainCharacter::MoveForward(float Value)
{
	if (!bAttacking)
	{
		
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMainCharacter::MoveRight(float Value)
{
	if (!bAttacking)
	{
		
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AMainCharacter::TurnAtRate(float Rate)
{
	if (bCamRot && !bLockedOn)
	{
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}

}

void AMainCharacter::LookUpRate(float Rate)
{
	if (bCamRot && !bLockedOn)
	{
		AddControllerPitchInput(Rate * BaseLookUpAtRate * GetWorld()->GetDeltaSeconds());
	}
}


void AMainCharacter::Punch1()
{

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 2.0f);
		AnimInstance->Montage_JumpToSection(FName("Punch"), CombatMontage);

	
	}
}

void AMainCharacter::Punch2()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 2.0f);
		AnimInstance->Montage_JumpToSection(FName("Punch2"), CombatMontage);
	}
}

void AMainCharacter::Punch3()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 2.0f);
		AnimInstance->Montage_JumpToSection(FName("Punch3"), CombatMontage);
	}
}

void AMainCharacter::Punch4()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 2.0f);
		AnimInstance->Montage_JumpToSection(FName("Punch4"), CombatMontage);
	}
}

void AMainCharacter::Punch5()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 2.0f);
		AnimInstance->Montage_JumpToSection(FName("Punch5"), CombatMontage);
	}
}



void AMainCharacter::Kick1()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 2.0f);
		AnimInstance->Montage_JumpToSection(FName("MMA Kick"), CombatMontage);
	}
}

void AMainCharacter::CannonAnim()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 4.0f);
		AnimInstance->Montage_JumpToSection(FName("EraserCannon"), CombatMontage);
	}
} 

void AMainCharacter::EraserCannon()
{
	FVector NewLocation, Adder = FVector(0, 0, 70);
	NewLocation = GetMesh()->GetSocketLocation(TEXT("HeadSocket"));
	Beam->SetBeamSourcePoint(0, NewLocation, 0);
	NewLocation += Camera->GetForwardVector() * 5000.0f;

	Beam->SetVisibility(true);
	isHit = GetWorld()->LineTraceSingleByChannel(OutHit, GetCapsuleComponent()->GetComponentLocation(), NewLocation, ECollisionChannel::ECC_Visibility);

	if (isHit)
	{
		Beam->SetBeamTargetPoint(0, OutHit.Location, 0);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "Hit!");


		AWalls* WallShooter = Cast<AWalls>(OutHit.GetActor());


		if (WallShooter)
		{
			RadialB->SetWorldLocation(OutHit.Location);
			RadialB->FireImpulse();
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Miss!");
		Beam->SetBeamTargetPoint(0, NewLocation, 0);
	}
}

void AMainCharacter::ShiftDown()
{
	bAttacking = true;
	float Combo = .5f;
	RadialL->FireImpulse();
	RadialR->FireImpulse();

	if (!CountDown)
	{
		PunchStatus = EPunchStatus::EPS_Punch1;
		Punch1();
		
		GetWorldTimerManager().SetTimer(NextAttack, this, &AMainCharacter::ShiftUp, Combo);
		CountDown = true;

	}
	else if (CountDown && (PunchStatus == EPunchStatus::EPS_Punch1))
	{
		GetWorldTimerManager().ClearTimer(NextAttack);
		PunchStatus = EPunchStatus::EPS_Punch2;
		Punch2();
		GetWorldTimerManager().SetTimer(NextAttack, this, &AMainCharacter::ShiftUp, Combo);
		CountDown = true;
	}
	else if (CountDown && (PunchStatus == EPunchStatus::EPS_Punch2))
	{
		GetWorldTimerManager().ClearTimer(NextAttack);
		PunchStatus = EPunchStatus::EPS_Punch3;
		Punch3();
		GetWorldTimerManager().SetTimer(NextAttack, this, &AMainCharacter::ShiftUp, Combo);
		CountDown = true;
	}
	else if (CountDown && (PunchStatus == EPunchStatus::EPS_Punch3))
	{
		GetWorldTimerManager().ClearTimer(NextAttack);
		PunchStatus = EPunchStatus::EPS_Punch5;
		Punch5();
		GetWorldTimerManager().SetTimer(NextAttack, this, &AMainCharacter::ShiftUp, Combo);
		CountDown = true;
	}
}

void AMainCharacter::ShiftUp()
{	
	bAttacking = false;
	CountDown = false;
	PunchStatus = EPunchStatus::EPS_Normal;
}

void AMainCharacter::EnterDown()
{
	bAttacking = true;
	float Combo = .5f;

	if (!CountDown)
	{
		PunchStatus = EPunchStatus::EPS_Kick1;
		Kick1();
		GetWorldTimerManager().SetTimer(NextAttack, this, &AMainCharacter::EnterUp, Combo);
		CountDown = true;
	}
}

void AMainCharacter::EnterUp()
{
	ShiftUp();
}

void AMainCharacter::LMBDown()
{	
	bAttacking = true;
	CannonAnim();
	
	GetWorldTimerManager().SetTimer(NextAttack, this, &AMainCharacter::EraserCannon, .25f);
}

void AMainCharacter::LMBUp()
{
	bAttacking = false;
	Beam->SetVisibility(false);
}

void AMainCharacter::SetToNearestTarget(TArray<AWalls*> WallsinRange, int32 Wallnumber)
{
	FRotator NewRotation;
	
	//UE_LOG(LogTemp, Warning, TEXT("Location: %s"), *GetActorLocation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("Wallnumber: %d"), Wallnumber);

	if (WallsinRange.Num() != 0)
	{
		NewRotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), WallsinRange.Last(Wallnumber)->GetActorLocation());
		Controller->SetControlRotation(NewRotation);
	}
	//UE_LOG(LogTemp, Warning, TEXT("Rotation: %s"), *NewRotation.Vector().ToString());

	
}


void AMainCharacter::LDown()
{
	SetToNearestTarget(WallArrClose, Wallnumber);	

	if (Wallnumber < WallArrClose.Num() - 1)
	{
		Wallnumber++;
	}
	else
	{
		Wallnumber = 0;
	}
}

void AMainCharacter::LUp()
{
	
	//UE_LOG(LogTemp, Warning, TEXT("Wallnumber: %d"), Wallnumber);

}

void AMainCharacter::CalcClose()
{
	Wallnumber = 0;
	WallArrClose.Empty();
	float NewClosest = MaxTargetDist;
	//UE_LOG(LogTemp, Warning, TEXT("Array Size: %d"), WallArr.Num());
	for (int32 i = 0; i < WallArr.Num(); i++)
	{
		Wall = WallArr.Last(i);

		if (GetDistanceTo(Wall) < MaxTargetDist)
		{
			WallArrClose.Add(Wall);
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("Array Size: %d"), WallArrClose.Num());

}









