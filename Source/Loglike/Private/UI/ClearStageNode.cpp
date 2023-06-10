// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ClearStageNode.h"
#include "UI/ClearStageNode.h"
#include "UI/StageNodeWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/LoglikeGameInstance.h"

void UClearStageNode::NativeConstruct()
{
	Super::NativeConstruct();
	
	
}

void UClearStageNode::SetStageText(FText NumTxt, FText StageTxt, FText TokenTxt)
{
	if(StageText&&TokenText&&NumberText)
	{
		NumberText->SetText(NumTxt);
		StageText->SetText(StageTxt);
		TokenText->SetText(TokenTxt);
	}
	
}
