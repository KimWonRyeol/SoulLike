// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/UIpawn.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"


// Sets default values
AUIpawn::AUIpawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("rootComponent"));
}

// Called when the game starts or when spawned
void AUIpawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUIpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUIpawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("AnyKey", IE_Pressed, this, &AUIpawn::OnAnyKeyPress);
}

void AUIpawn::OnAnyKeyPress()
{
	UE_LOG(LogTemp, Log, TEXT("Log Message"));
	// 다음 레벨로 이동하는 코드
	FString nextLevel = FString("Lobby");
	UGameplayStatics::OpenLevel(GetWorld(), FName(*nextLevel));
}
