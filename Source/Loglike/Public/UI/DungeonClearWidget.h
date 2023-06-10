// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DungeonClearWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API UDungeonClearWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	void UpdateTotalText(FText NewText);
	UFUNCTION()
	void MoveLobby();
	void PlayFadeIn();
public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "TotalToken",Meta = (BindWidget))
	class UTextBlock* TotalTokenText;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "TotalStage",Meta = (BindWidget))
	class UScrollBox* ScrollBox;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TotalStage")
	TSubclassOf<class UClearStageNode> ClearStageNodeClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button",Meta = (BindWidget))
	class UButton* LobbyButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ClearStage")
	TArray<class UClearStageNode*> ClearStageNodes;
	//Animation
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* FadeIn;
	
};
