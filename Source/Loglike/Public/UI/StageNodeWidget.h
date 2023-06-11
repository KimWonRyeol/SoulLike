// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageNodeWidget.generated.h"


/**
 * 
 */

UENUM(BlueprintType)
enum class EStageNodeState : uint8
{
	E_Enable = 0 UMETA(DisplayName = "Enable"),
	E_Disable = 1 UMETA(DisplayName = "Disable"),
	E_Select = 2 UMETA(DisplayName = "Select"),
};

UENUM(BlueprintType)
enum class EStageType : uint8
{
	E_Artificat = 0 UMETA(DisplayName = "Artificat"),
	E_Boss = 1 UMETA(DisplayName = "Boss"),
	E_Health = 2 UMETA(DisplayName = "Health"),
	E_Stat = 3 UMETA(DisplayName = "Stat"),
	E_Weapon = 4 UMETA(DisplayName = "Weapon"), 
	E_None = 254 UMETA(DisplayName = "None"),
};

USTRUCT(Atomic, BlueprintType)
struct FStageNodeVisual
{
    GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool EnableBtn;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float EnableStageBorderOpacity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SelectedStageBorderOpacity;
};
UCLASS()
class LOGLIKE_API UStageNodeWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UStageNodeWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
public:
	void SetStageImage();

	void SetNodeState(EStageNodeState);
	UFUNCTION(BlueprintCallable)
		void NodeButtonCallback();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UImage* StageImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UImage* StageBorder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UImage* SelectedStageBorder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UImage* EnableStageBorder;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStageType NodeStateType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UTexture2D*> NodeImageArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UButton* StageBtn;
public:
	UPROPERTY()
		TArray<int32> NextStageNodes;
	UPROPERTY()
		class UStageTreeWidget* ParentWidget;
};
