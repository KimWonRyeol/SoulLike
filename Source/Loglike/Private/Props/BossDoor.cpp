// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/BossDoor.h"
#include "Enemy/BossAIController.h"
#include "Enemy/Boss.h"

#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"

// Sets default values
ABossDoor::ABossDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BossDoorL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BossDoorL"));
	BossDoorR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BossDoorR"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	PlayerStop = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayerStop"));
	CloseDoor = CreateDefaultSubobject<UBoxComponent>(TEXT("CloseDoor"));

	SetRootComponent(BoxComponent);
	BossDoorL->SetupAttachment(RootComponent);
	BossDoorR->SetupAttachment(RootComponent);
	PlayerStop->SetupAttachment(RootComponent);
	CloseDoor->SetupAttachment(RootComponent);

	RotateSpeed = 30.f;
	OpenDoor = false;
	IsClose = true;
}

void ABossDoor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABossDoor::BoxCompBegineOverlap);
	CloseDoor->OnComponentBeginOverlap.AddDynamic(this, &ABossDoor::CloseDoorCompBegineOverlap);
	IsEnterance(false);
}

void ABossDoor::BeginPlay()
{
	Super::BeginPlay();	
}

void ABossDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OpenDoor)
	{
		float RotationSpeedL = -30.0f; 
		float RotationSpeedR = 30.0f;

		FRotator RotationAmountL(0.0f, DeltaTime * RotationSpeedL, 0.0f);
		FRotator RotationAmountR(0.0f, DeltaTime * RotationSpeedR, 0.0f);


		FRotator CurrentRotationL = BossDoorL->GetRelativeRotation();
		FRotator CurrentRotationR = BossDoorR->GetRelativeRotation();

		FRotator RotationL = CurrentRotationL + RotationAmountL;
		FRotator RotationR = CurrentRotationR + RotationAmountR;

		if (RotationL.Yaw <= 0)
		{
			OpenDoor = false;
			IsClose = false;
			IsEnterance(true);
		}

		BossDoorL->SetRelativeRotation(RotationL);
		BossDoorR->SetRelativeRotation(RotationR);
	}
	if (IsClose)
	{
		float RotationSpeedL = 30.0f;
		float RotationSpeedR = -30.0f;

		FRotator RotationAmountL(0.0f, DeltaTime * RotationSpeedL, 0.0f);
		FRotator RotationAmountR(0.0f, DeltaTime * RotationSpeedR, 0.0f);


		FRotator CurrentRotationL = BossDoorL->GetRelativeRotation();
		FRotator CurrentRotationR = BossDoorR->GetRelativeRotation();

		FRotator RotationL = CurrentRotationL + RotationAmountL;
		FRotator RotationR = CurrentRotationR + RotationAmountR;

		if (RotationL.Yaw >= 90)
		{
			OpenDoor = false;
			IsClose = false;
			IsEnterance(false);

			ABoss* Boss = Cast<ABoss>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoss::StaticClass()));
			if (Boss)
			{
				ABossAIController* BossAI = Cast<ABossAIController>(Boss->GetController());
				if (BossAI)
				{
					BossAI->SetEntranceKey(false);
				}
			}
			
		}

		BossDoorL->SetRelativeRotation(RotationL);
		BossDoorR->SetRelativeRotation(RotationR);
	}
}

void ABossDoor::BoxCompBegineOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OpenDoor)
	{
		OpenDoor = true;
	}
}

void ABossDoor::CloseDoorCompBegineOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsClose)
	{
		IsClose = true;

		ABoss* Boss = Cast<ABoss>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoss::StaticClass()));

		if (Boss)
		{
			ABossAIController* BossAI = Cast<ABossAIController>(Boss->GetController());
			if (BossAI)
			{
				Boss->SetHPbar();
				BossAI->SetEntranceKey(true);
				CloseDoor->SetCollisionProfileName(TEXT("NoCollision"));
			}
		}
	}

}


void ABossDoor::IsEnterance(bool IgnoreCollision)
{
	if (PlayerStop)
	{
		if (IgnoreCollision)
		{
			PlayerStop->SetCollisionProfileName(TEXT("NoCollision"));
		}
		else
		{
			PlayerStop->SetCollisionProfileName(TEXT("BlockAll"));
		}
	}
}