// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/Door.h"
#include "DungeonGameMode.h"
#include "Character/LoglikeCharacter.h"
#include "Character/ABCharacterStatComponent.h"
#include "GameFramework/LoglikeGameInstance.h"
#include "UI/DungeonClearWidget.h"
#include "UI/StageNodeWidget.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	DoorL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Door"));
	DoorR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Door"));
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CharacterCollider"));
	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Detail Text"));

	SetRootComponent(SphereComponent);
	DoorL->SetupAttachment(RootComponent);
	DoorR->SetupAttachment(RootComponent);
	TextComponent->SetupAttachment(RootComponent);

	IsStageClear = false;
	IsEnableWidget = false;
}

void ADoor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	TextComponent->SetVisibility(false);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ADoor::CharacterOverlapBegin);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ADoor::CharacterOverlapEnd);

}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<ADungeonGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->OnStageClear.AddDynamic(this, &ADoor::OnStageClear);

	EStageType CurrentStage = Cast<ULoglikeGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->CurrentStageType;
	if (CurrentStage == EStageType::E_None || CurrentStage == EStageType::E_Health)
	{
		IsStageClear = true;
	}
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ADoor::CharacterOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsStageClear) return;
	TextComponent->SetVisibility(true);
}
void ADoor::CharacterOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsStageClear) return;
	TextComponent -> SetVisibility(false);
}

void ADoor::ShowNextStageWidget()
{
	if (!IsStageClear) return;
	
	ULoglikeGameInstance* GameInstance = Cast<ULoglikeGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//현재 스테이지가 보스 스테이지라면 다음 스테이지의 종류를 None으로 변경하고 선택된 스테이지를 초기화한 뒤 로비로 이동
	if (GameInstance->CurrentStageType == EStageType::E_Boss)
	{
		if(IsEnableWidget) return;
		IsEnableWidget = true;
		ALoglikeCharacter* PlayerCharacter = Cast<ALoglikeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
		//Cast<ULoglikeGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->InitDungeonMode();
		//Token
		GameInstance->SetAddToken(10);
		GameInstance->SetTotalAddToken(GameInstance->GetAddToken());
		PlayerCharacter->CharacterToken = PlayerCharacter->CharacterStat->GetToken() + GameInstance->GetAddToken();
		PlayerCharacter->SaveCharacter();
		
		//Clear Array
		TPair<int32, int32> ClearPair;
		ClearPair.Key = (int32)GameInstance->CurrentStageType;
		ClearPair.Value = 10;
		GameInstance->ClearStageArray.Add(ClearPair);
		GameMode->EnableDungeonClearWidget();
		
		return;
		//UGameplayStatics::OpenLevel(this, "Lobby");
	}
	//현재 스테이지가 보스 스테이지가 아니라면 다음 스테이지 선택을 위한 Widget을 보여줌
	if (GameMode == nullptr || GameMode->GetIsEnableNextStageWidget()) { return; }
	GameMode->EnableNextStageWidget();
}

void ADoor::OnStageClear()
{
	UE_LOG(LogTemp, Warning, TEXT("------Stage Clear-----"));
	IsStageClear = true;
}
