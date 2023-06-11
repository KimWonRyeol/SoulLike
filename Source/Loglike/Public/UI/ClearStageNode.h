// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClearStageNode.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API UClearStageNode : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	void SetStageText(FText NumTxt,FText StageTxt, FText TokenTxt);
public:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Text",Meta = (BindWidget))
	class UTextBlock* NumberText;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Text",Meta = (BindWidget))
	class UTextBlock* StageText;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Text",Meta = (BindWidget))
	class UTextBlock* TokenText;
	
};
