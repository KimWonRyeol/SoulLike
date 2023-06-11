// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ABCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API UABCharacterWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//캐릭터와 연결할 함수
	void BindCharacter(class ALoglikeCharacter* LoglikeCharacter);

	//캐릭터 피격 효과 패널 출력 함수
	void PlayWidgetAnimationByName();


protected:
	//AddToViewport 시 호출되는 함수
	virtual void NativeConstruct() override;
	

public:
	//HUD 상의 실제 데미지 텍스트 블럭
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* DamageText;

	//HUD 상의 실제 행운 텍스트 블럭
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* LuckText;

	//HUD 피격 효과 출력
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* Take_Damage_Red;


private:
	//캐릭터 클래스 객체
	UPROPERTY()
	ALoglikeCharacter* CurrentCharacter;

	//프로그래스바 클리스 객체
	UPROPERTY()
	class UProgressBar* HPProgressBar;
};
