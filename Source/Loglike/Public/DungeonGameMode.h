// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LoglikeGameMode.h"
#include "DungeonGameMode.generated.h"

enum class EStageType : uint8;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStageClearDelegate);
/**
 * 
 */
UCLASS()
class LOGLIKE_API ADungeonGameMode : public ALoglikeGameMode
{
	GENERATED_BODY()
public:
	ADungeonGameMode();
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:
	/**If Stage is clear, NextStageWidget can be added to viewport*/
	void EnableNextStageWidget();
	/**Delete NextStageWidget*/
	void DisableNextStageWidget();
	/**When Monster dead, MonsterNum Update*/
	void UpdateMonsterNum();
	/**Generate Monster in Dungeon*/
	void GenerateMonster(EStageType GenMonStage);

	void EnableDungeonClearWidget();

	bool GetIsEnableNextStageWidget();
	bool GetIsEnableDungeonClearWidget();
	

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GateUI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UStageTreeWidget> NextStageWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GateUI", meta = (AllowPrivateAccess = "true"))
	class UStageTreeWidget* NextStageWidgetObject;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterGen", meta = (AllowPrivateAccess = "true"))
	TArray<class AMonsterGenerator*> MonsterGenerator;
	TArray<TTuple<int8, int8>> MonsterGroupPairs;
	TTuple<int8, int8> MonsterGroupPair;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterGen", meta = (AllowPrivateAccess = "true"))
	int32 MonsterNum;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterGen", meta = (AllowPrivateAccess = "true"))
	int32 Wave;
public:
	UPROPERTY(BlueprintAssignable, Category = "MonsterGen")
	FOnStageClearDelegate OnStageClear;	//BroadCast that Stage is cleared.
public:
	/**FadeIn/Out*/
	void FadeIn();
	void FadeOut();

/// <summary>
/// FadeOut
/// </summary>
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GateUI", meta = (AllowPrivateAccess = "true"))
	class UFadeInOutWidget* FadeInOutWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GateUI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> FadeInOutWidgetClass;
	
	//UI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GateUI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UDungeonClearWidget> ClearWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GateUI", meta = (AllowPrivateAccess = "true"))
	class UDungeonClearWidget* DungeonClearWidget;
};
