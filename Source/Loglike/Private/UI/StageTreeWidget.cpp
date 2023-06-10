// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StageTreeWidget.h"
#include "UI/StageNodeWidget.h"

#include "Character/LoglikeCharacter.h"
#include "Character/ABCharacterStatComponent.h"

#include "GameFramework/LoglikeGameInstance.h"
#include "DungeonGameMode.h"


#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"


UStageTreeWidget::UStageTreeWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//연결 입력
	TreeConnection.Emplace(0, 1);
	TreeConnection.Emplace(0, 2);

	TreeConnection.Emplace(1, 3);
	TreeConnection.Emplace(2, 4);
	TreeConnection.Emplace(2, 5);

	TreeConnection.Emplace(3, 6);
	TreeConnection.Emplace(4, 7);
	TreeConnection.Emplace(5, 8);

	TreeConnection.Emplace(6, 9);
	TreeConnection.Emplace(6, 10);
	TreeConnection.Emplace(7, 10);
	TreeConnection.Emplace(8, 10);

	TreeConnection.Emplace(9, 11);
	TreeConnection.Emplace(10, 12);
	TreeConnection.Emplace(10, 13);

	TreeConnection.Emplace(11, 14);
	TreeConnection.Emplace(12, 15);
	TreeConnection.Emplace(13, 15);

	TreeConnection.Emplace(14, 16);
	TreeConnection.Emplace(15, 16);

	NodeNum = 17;
	IsSelect = false;
}

void UStageTreeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//GetNodes
	for (int i = 0; i < NodeNum; i++)
	{
		FString NodeName = FString::Printf(TEXT("StageNode%d"), i);
		UStageNodeWidget* Node = Cast<UStageNodeWidget>(GetWidgetFromName(FName(*NodeName)));
		if (Node != nullptr)
		{
			StageTree.Add(Node);
			NodeNumMap.Add(Node, i);
			Node->ParentWidget = this;
		}
	}

	//Set Connection info
	for (auto tuple : TreeConnection)
	{
		StageTree[tuple.Key]->NextStageNodes.Add(tuple.Value);
	}
	
	//Show Nodes
	UpdateTree();
	
	//Back Btn
	BackButton = Cast<UButton>(GetWidgetFromName(TEXT("BackBtn")));
	BackButton->OnClicked.AddDynamic(this, &UStageTreeWidget::RemoveStageTreeWidget);
}

void UStageTreeWidget::UpdateTree()
{
	TArray<int32> SelectedArr = Cast<ULoglikeGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetSelectedNodeArr();
	//Tree Load Update
	for (auto StageNode : StageTree)
	{
		StageNode->SetNodeState(EStageNodeState::E_Disable);
	}
	for (auto SelectNodeNum : SelectedArr)
	{
		StageTree[SelectNodeNum]->SetNodeState(EStageNodeState::E_Select);
	}
	if (SelectedArr.Num() == 0)
	{
		StageTree[0]->SetNodeState(EStageNodeState::E_Enable);
	}
	else
	{
		//Next Node UI Change
		for (auto j : StageTree[SelectedArr[SelectedArr.Num() - 1]]->NextStageNodes)
		{
			StageTree[j]->SetNodeState(EStageNodeState::E_Enable);
		}
	}
	
}

void UStageTreeWidget::SelectNode(UStageNodeWidget* NodeWidget)
{
	if(IsSelect) return;
	IsSelect = true;
	ULoglikeGameInstance* GameInstance = Cast<ULoglikeGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ALoglikeCharacter* PlayerCharacter = Cast<ALoglikeCharacter>(UGameplayStatics::GetPlayerPawn(this,0));
	
	
	if (GameInstance->CurrentStageType != EStageType::E_None) //Normal Stage -> Token 1~4
	{
		int32 RandomToken = FMath::RandRange(1, 3);
		GameInstance->SetAddToken(RandomToken);
		GameInstance->SetTotalAddToken(GameInstance->GetAddToken());
		PlayerCharacter->CharacterToken = PlayerCharacter->CharacterStat->GetToken() + GameInstance->GetAddToken();
		PlayerCharacter->SaveCharacter();

		//Clear Array
		TPair<int32, int32> ClearPair;
		ClearPair.Key = (int32)GameInstance->CurrentStageType;
		ClearPair.Value = RandomToken;
		GameInstance->ClearStageArray.Add(ClearPair);
	}

	//Sound
	//USoundBase* MySound = LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/RPG_Skill_SFX/Wav/Additional_SFX/UI_Confirm_02_Impact_Heavy_Wooden_Metal_01.UI_Confirm_02_Impact_Heavy_Wooden_Metal_01'"));
	if (MySound)
	{
		//UUserWidget::PlaySound(MySound);
		UUserWidget::PlaySound(MySound);
	}
	
	//SelectNode
	int32 Num = (int32)NodeNumMap[NodeWidget];
	GameInstance->AddSelectedNode(Num);	//Add Node
	GameInstance->CurrentStageType = NodeWidget->NodeStateType;

	//Transform Stage
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%d"), (int8)(GameInstance->CurrentStageType)));
	TransformStage(GameInstance->CurrentStageType == EStageType::E_Boss);
}


void UStageTreeWidget::FadeAnimationPlay(bool IsIn)
{
	if (IsIn)
	{
		PlayAnimation(FadeIn);
	}
	else
	{
		PlayAnimation(FadeOut);
	}
}

void UStageTreeWidget::TransformStage(bool IsBossStage)
{
	
	//Fade
	Cast<ADungeonGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->FadeIn();
	FadeAnimationPlay(false);
	
	FTimerHandle fadeOutTimerHandle;
	//Enter Boss Stage 
	if (IsBossStage)
	{
		GetWorld()->GetTimerManager().SetTimer(fadeOutTimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				UGameplayStatics::OpenLevel(this, FName("Dungeon-Boss"));
			}), 0.6f, false);
		return;
	}

	//Enter Normally Stage
	GetWorld()->GetTimerManager().SetTimer(fadeOutTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			int8 RandomStageNum = FMath::RandRange(1, 3);
			FName DungeonName = FName(*FString::Printf(TEXT("Dungeon-%d"), RandomStageNum));
			UGameplayStatics::OpenLevel(this, DungeonName);
		}), 0.6f, false);
}

void UStageTreeWidget::RemoveStageTreeWidget()
{
	FadeAnimationPlay(false);
	FTimerHandle WidgetDeleteTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(WidgetDeleteTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			Cast<ADungeonGameMode>(GetWorld()->GetAuthGameMode())->DisableNextStageWidget();
		}), 0.5f, false);
	
}
