// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGameMode.h"
#include "GameFramework/LoglikeGameInstance.h"
#include "UI/StageTreeWidget.h"
#include "UI/StageNodeWidget.h"
#include "UI/FadeInOutWidget.h"
#include "UI/DungeonClearWidget.h"
#include "Enemy/MonsterGenerator.h"
#include "Kismet/GameplayStatics.h"
#include "Character/LoglikeCharacter.h"
#include "Character/ABCharacterWidget.h"


ADungeonGameMode::ADungeonGameMode()
{
/*	static ConstructorHelpers::FClassFinder<UStageTreeWidget> SelectWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/UI_SelectStage.UI_SelectStage_C'"));
	if (SelectWidget.Succeeded())
	{
		NextStageWidgetClass = SelectWidget.Class;
	}*/

	//Monster Gen
	MonsterGroupPairs.Emplace(3, 2);
	MonsterGroupPairs.Emplace(2, 4);
	MonsterGroupPairs.Emplace(4, 5);
	MonsterGroupPairs.Emplace(5, 3);
	MonsterGroupPairs.Emplace(2, 5);
	MonsterGroupPairs.Emplace(4, 1);

	int8 RandNum = FMath::RandRange(0, 5);
	MonsterGroupPair = MonsterGroupPairs[RandNum];
	Wave = 1;

	//FadeInOut
	/*static ConstructorHelpers::FClassFinder<UFadeInOutWidget> FadeWidget(TEXT("/ Script / UMGEditor.WidgetBlueprint'/Game/UI/UI_FadeInOut.UI_FadeInOut_C'"));
	if (FadeWidget.Succeeded())
	{
		FadeInOutWidgetClass = FadeWidget.Class;
	}*/
	
}
void ADungeonGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//Lobby Initialize
	if (GetWorld()->GetMapName().Contains("Lobby"))
	{
		Cast<ULoglikeGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->InitDungeonMode();	//Init in Instance 
		OnStageClear.Broadcast();	//Door Init in GameMode 
	}

}
void ADungeonGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	//UI
	UGameplayStatics::GetPlayerController(this, 0)->SetInputMode(FInputModeGameOnly());

	//SetCurrentStage
	EStageType CurrentStage = Cast<ULoglikeGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->CurrentStageType;

	/**Debugger*/
	//const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EStageType"), true);
	//if (!EnumPtr) return;
	//UE_LOG(LogTemp, Warning, TEXT("%d"), (uint8)CurrentStage);
	 

	//Generate Enemy
	GenerateMonster(CurrentStage);

	//FadeOut
	FadeOut();
}

void ADungeonGameMode::EnableNextStageWidget()
{
	if (!::IsValid(NextStageWidgetClass)) { UE_LOG(LogTemp, Warning, TEXT("Can not CreateWidget")); return; }

	//UE_LOG(LogTemp, Warning, TEXT("CreateWidget"));
	
	if (NextStageWidgetObject == nullptr)
	{
		NextStageWidgetObject = CreateWidget<UStageTreeWidget>(GetWorld(), NextStageWidgetClass);
		NextStageWidgetObject->AddToViewport();
		NextStageWidgetObject->FadeAnimationPlay(true);
	}
	
	Cast<ALoglikeCharacter>(UGameplayStatics::GetPlayerPawn(this, 0))->CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
	UGameplayStatics::GetPlayerController(this, 0)->SetShowMouseCursor(true);
	UGameplayStatics::GetPlayerController(this, 0)->SetInputMode(FInputModeUIOnly());
}
void ADungeonGameMode::DisableNextStageWidget()
{
	if (NextStageWidgetObject)
	{
		NextStageWidgetObject->RemoveFromParent();
		NextStageWidgetObject = nullptr;
	}
	Cast<ALoglikeCharacter>(UGameplayStatics::GetPlayerPawn(this, 0))->CurrentWidget->SetVisibility(ESlateVisibility::Visible);
	UGameplayStatics::GetPlayerController(this, 0)->SetShowMouseCursor(false);
	UGameplayStatics::GetPlayerController(this, 0)->SetInputMode(FInputModeGameOnly());
}
void ADungeonGameMode::UpdateMonsterNum()
{
	//보스던전일 땐 한마리만 죽여도 Stage Clear
	if (Cast<ULoglikeGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->CurrentStageType == EStageType::E_Boss)
	{
		OnStageClear.Broadcast();
	}
	//일반 던전일 때
	if (MonsterGenerator.Num() < 2) return;
	MonsterNum--;
	//UE_LOG(LogTemp, Warning, TEXT("Wave: %d, Remain Monster: %d"), Wave, MonsterNum);
	if (MonsterNum <= 0 && Wave < 2)	// 첫번째 Wave 클리어
	{
		MonsterNum = MonsterGenerator[1]->GenMonsters(MonsterGroupPair.Value);	// 두번째 Wave 시작
		Wave++;
	}
	else if (MonsterNum <= 0 && Wave == 2)
	{
		OnStageClear.Broadcast();
	}
}
void ADungeonGameMode::GenerateMonster(EStageType GenMonStage)
{
	if (GenMonStage == EStageType::E_Health)
	{
		// Health Stage do not Generate Monster 
	}
	else  // Other Stages Generate Monster
	{
		//Monster Generation
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMonsterGenerator::StaticClass(), Actors);
		for (auto actor : Actors)
		{
			if (Cast<AMonsterGenerator>(actor) != nullptr)
			{
				MonsterGenerator.Add(Cast<AMonsterGenerator>(actor));
			}

		}

		if (MonsterGenerator.Num() > 1)
		{
			if (MonsterGenerator[1]->GetDistanceTo(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)) < MonsterGenerator[0]->GetDistanceTo(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
			{
				AMonsterGenerator* tmp = MonsterGenerator[1];
				MonsterGenerator[1] = MonsterGenerator[0];
				MonsterGenerator[0] = tmp;
			}
			MonsterNum = MonsterGenerator[0]->GenMonsters(MonsterGroupPair.Key);
		}
	}
}

void ADungeonGameMode::EnableDungeonClearWidget()
{
	if (!::IsValid(ClearWidgetClass)) { UE_LOG(LogTemp, Warning, TEXT("Can not CreateWidget")); return; }

	//UE_LOG(LogTemp, Warning, TEXT("CreateWidget"));
	
	if (DungeonClearWidget == nullptr)
	{
		DungeonClearWidget = CreateWidget<UDungeonClearWidget>(GetWorld(), ClearWidgetClass);
		DungeonClearWidget->AddToViewport();
		DungeonClearWidget->PlayFadeIn();
	}
	
	Cast<ALoglikeCharacter>(UGameplayStatics::GetPlayerPawn(this, 0))->CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
	UGameplayStatics::GetPlayerController(this, 0)->SetShowMouseCursor(true);
	UGameplayStatics::GetPlayerController(this, 0)->SetInputMode(FInputModeUIOnly());
}

bool ADungeonGameMode::GetIsEnableNextStageWidget()
{
	return (NextStageWidgetObject != nullptr);
}

bool ADungeonGameMode::GetIsEnableDungeonClearWidget()
{
	return (DungeonClearWidget != nullptr);
}

void ADungeonGameMode::FadeIn()
{
	if (FadeInOutWidget==nullptr)
	{
		FadeInOutWidget = CreateWidget<UFadeInOutWidget>(GetWorld(), FadeInOutWidgetClass);
		
	}
	if (FadeInOutWidget)
	{
		FadeInOutWidget->AddToViewport();
		FadeInOutWidget->PlayFadeIn();
	}
}
void ADungeonGameMode::FadeOut()
{
	if (FadeInOutWidget == nullptr)
	{
		FadeInOutWidget = CreateWidget<UFadeInOutWidget>(GetWorld(), FadeInOutWidgetClass);
	}
	if (FadeInOutWidget)
	{
		FadeInOutWidget->AddToViewport();
		FadeInOutWidget->PlayFadeOut();
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("FadeInOutWidget is null"));
	}
}