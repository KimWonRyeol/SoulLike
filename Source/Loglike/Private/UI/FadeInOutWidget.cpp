// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FadeInOutWidget.h"

void UFadeInOutWidget::PlayFadeIn()
{
	PlayAnimation(FadeIn);
}

void UFadeInOutWidget::PlayFadeOut()
{
	PlayAnimation(FadeOut);
}