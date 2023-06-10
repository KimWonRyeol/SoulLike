// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DungeonClearWidget.h"

#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "UI/ClearStageNode.h"
#include "GameFramework/LoglikeGameInstance.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UDungeonClearWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ULoglikeGameInstance* GameInstance = Cast<ULoglikeGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	int32 TotalToken = 0;
	int32 Index = 1;
	TArray<TPair<int32,int32>> ClearPairArray = GameInstance->ClearStageArray;
	for(TPair<int32,int32> ClearPair : ClearPairArray)
	{
		UClearStageNode* StageClearWidget = CreateWidget<UClearStageNode>(GetWorld(),ClearStageNodeClass);
		switch (ClearPair.Key)
		{
		case 0:
			StageClearWidget->SetStageText(FText::FromString(FString::FromInt(Index)),FText::FromString("Artifact"),FText::FromString(FString::FromInt(ClearPair.Value)));
			break;
		case 1:
			StageClearWidget->SetStageText(FText::FromString(FString::FromInt(Index)),FText::FromString("Boss"),FText::FromString(FString::FromInt(ClearPair.Value)));
			break;
		case 2:
			StageClearWidget->SetStageText(FText::FromString(FString::FromInt(Index)),FText::FromString("Health"),FText::FromString(FString::FromInt(ClearPair.Value)));
			break;
		case 3:
			StageClearWidget->SetStageText(FText::FromString(FString::FromInt(Index)),FText::FromString("Stat"),FText::FromString(FString::FromInt(ClearPair.Value)));
			break;
		case 4:
			StageClearWidget->SetStageText(FText::FromString(FString::FromInt(Index)),FText::FromString("Weapon"),FText::FromString(FString::FromInt(ClearPair.Value)));
			break;
		}
		TotalToken += ClearPair.Value;
		if(ScrollBox)
		{
			ClearStageNodes.AddUnique(StageClearWidget);
			ScrollBox->AddChild(StageClearWidget);
			Index++;
		}
	}
	UpdateTotalText(FText::FromString(FString::FromInt(TotalToken)));
	LobbyButton->OnClicked.AddDynamic(this,&UDungeonClearWidget::MoveLobby);
}

void UDungeonClearWidget::UpdateTotalText(FText NewText)
{
	if(TotalTokenText)
	{
		FText CombinedText = FText::Format(NSLOCTEXT("YourNamespace", "TotalFormat", "Total Token: {0}"), NewText);
		TotalTokenText->SetText(CombinedText);
	}
}

void UDungeonClearWidget::MoveLobby()
{
	UGameplayStatics::OpenLevel(this, "Lobby");
}

void UDungeonClearWidget::PlayFadeIn()
{
	PlayAnimation(FadeIn);
}
