// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StageNodeWidget.h"
#include "UI/StageTreeWidget.h"
#include "Components/Image.h" 
#include "Components/Button.h"
#include "Math/Color.h"

UStageNodeWidget::UStageNodeWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UStageNodeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StageImage = Cast<UImage>(GetWidgetFromName(TEXT("StageImg")));
	StageBorder = Cast<UImage>(GetWidgetFromName(TEXT("border")));
	StageBtn = Cast<UButton>(GetWidgetFromName(TEXT("Btn")));
	SelectedStageBorder = Cast<UImage>(GetWidgetFromName(TEXT("Selectedborder")));
	EnableStageBorder = Cast<UImage>(GetWidgetFromName(TEXT("Enableborder")));
	SetStageImage();
	StageBtn->OnClicked.AddDynamic(this, &UStageNodeWidget::NodeButtonCallback);
}


void UStageNodeWidget::SetStageImage()
{
	uint8 TextureNum = (uint8)NodeStateType;
	StageImage->SetBrushFromTexture(NodeImageArr[TextureNum], true);

}

void UStageNodeWidget::SetNodeState(EStageNodeState CurrentState)
{
	FStageNodeVisual NodeVisual;
	switch (CurrentState)
	{
	case EStageNodeState::E_Enable:
		NodeVisual.EnableBtn = true;
		NodeVisual.EnableStageBorderOpacity = 1.f;
		NodeVisual.SelectedStageBorderOpacity = 0.f;
		
		break;
	case EStageNodeState::E_Disable:
		NodeVisual.EnableBtn = false;
		NodeVisual.EnableStageBorderOpacity = 0.f;
		NodeVisual.SelectedStageBorderOpacity = 0.f;
		break;
	case EStageNodeState::E_Select:
		NodeVisual.EnableBtn = false;
		NodeVisual.EnableStageBorderOpacity = 0.f;
		NodeVisual.SelectedStageBorderOpacity = 1.f;
		break;
	}

	StageBtn->SetIsEnabled(NodeVisual.EnableBtn);
	EnableStageBorder->SetOpacity(NodeVisual.EnableStageBorderOpacity);
	SelectedStageBorder->SetOpacity(NodeVisual.SelectedStageBorderOpacity);
}

void UStageNodeWidget::NodeButtonCallback()
{
	if(ParentWidget == nullptr) return;

	ParentWidget->SelectNode(this);
	//ParentWidget->UpdateTree();
}
