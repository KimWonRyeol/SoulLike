// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageTreeWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API UStageTreeWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UStageTreeWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	void UpdateTree();
	void SelectNode(class UStageNodeWidget* NodeWidget);
	void FadeAnimationPlay(bool IsIn);
	void TransformStage(bool IsBossStage);

	UFUNCTION()
	void RemoveStageTreeWidget();

public:
	//Nodes
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "NextStage")
	TArray<class UStageNodeWidget*> StageTree;
	TArray<TTuple<int32, int32>> TreeConnection;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "NextStage")
	TMap<class UStageNodeWidget*, int32> NodeNumMap;

	//Back Button
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "BackButton")
	class UButton* BackButton;

	//Animation
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* FadeIn;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* FadeOut;
	int32 NodeNum = 7;

	//Sound
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	class USoundBase* MySound;
	//SelectNode
	bool IsSelect;
};
