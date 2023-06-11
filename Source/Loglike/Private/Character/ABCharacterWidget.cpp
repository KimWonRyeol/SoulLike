// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterWidget.h"
#include "Components/ProgressBar.h"
#include "Character/LoglikeCharacter.h"
#include "Components/TextBlock.h"
#include "GameFramework/Pawn.h"
#include "Animation/WidgetAnimation.h"


//ĳ���Ϳ� ������ �Լ�
void UABCharacterWidget::BindCharacter(ALoglikeCharacter* LoglikeCharacter)
{
	CurrentCharacter = LoglikeCharacter;
	//LoglikeCharacter->OnHPChanged.AddUObject(this, &UABCharacterWidget::UpdateHPWidget);
}

void UABCharacterWidget::PlayWidgetAnimationByName()
{
	PlayAnimation(Take_Damage_Red);
}

//AddToViewport �� ȣ��Ǵ� �Լ�
void UABCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//���� UI���� PB_HPBar �� �ش��ϴ� �̸��� ���α׷����� ��ü�� ����(���α׷����ٿ� ����)
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	//UpdateHPWidget();
}
