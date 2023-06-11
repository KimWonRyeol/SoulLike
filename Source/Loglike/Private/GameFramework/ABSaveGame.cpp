// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/ABSaveGame.h"

UABSaveGame::UABSaveGame()
{
	Name = 1;
	MaxHealth = 100.0f;
	DefaultDamage = 0.0f;
	DefaultLuck = 0.0f;
	DefaultAttackSpeed = 1.0;
	Token = 0;
	WeaponKey = false;
	ArtifactKey = false;
	WeaponKeyRemain = 1;
	ArtifactKeyRemain = 1;
	HpStatRemain = 10;
	DamageStatRemain = 10;
	LuckStatRemain = 10;
}
