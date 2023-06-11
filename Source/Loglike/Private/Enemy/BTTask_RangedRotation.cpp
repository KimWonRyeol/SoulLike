// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BTTask_RangedRotation.h"
#include "Character/LoglikeCharacter.h"
#include "Enemy/MonsterAIControllerBase.h"
#include "Enemy/MonsterBase.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_RangedRotation::UBTTask_RangedRotation()
{
	NodeName = TEXT("ReadyToSpell");
}

void UBTTask_RangedRotation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

}

EBTNodeResult::Type UBTTask_RangedRotation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Monster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Monster) 
		return EBTNodeResult::Failed;

	auto Target = Cast<ALoglikeCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIControllerBase::TargetKey));
	if (nullptr == Target) 
		return EBTNodeResult::Failed;

	LookVector = Target->GetActorLocation() - Monster->GetActorLocation();
	LookVector.Z = 0.0f;

	TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	Monster->SetActorRotation(FMath::RInterpTo(Monster->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;
}