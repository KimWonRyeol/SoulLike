// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BTService_Detect.h"
#include "Character/LoglikeCharacter.h"
#include "Enemy/MonsterAIControllerBase.h"
#include "Enemy/MonsterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
	DetectRadius = 600.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	

	if (nullptr == World) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_Pawn,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			ALoglikeCharacter* Player = Cast<ALoglikeCharacter>(OverlapResult.GetActor());
			if (Player && Player->GetController()->IsPlayerController())
			{
				FVector StartLoc = ControllingPawn->GetActorLocation();
				FVector EndLoc = Player->GetActorLocation();
				FHitResult HitResult;
				TArray<AActor*> IgnoreActors;
				IgnoreActors.Add(ControllingPawn);
				IgnoreActors.Add(Player);
				bool Result = UKismetSystemLibrary::LineTraceSingle(
					GetWorld(),
					StartLoc,
					EndLoc,
					UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_WorldStatic),
					false,
					IgnoreActors,
					EDrawDebugTrace::None,
					HitResult,
					true
					, FLinearColor::Red
					, FLinearColor::Blue
					, 5.0f
				);

				if (Result) { return; }
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMonsterAIControllerBase::TargetKey, Player);
				//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				//DrawDebugPoint(World, Player->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				//DrawDebugLine(World, ControllingPawn->GetActorLocation(), Player->GetActorLocation(), FColor::Blue, false, 0.27f);
				return;
			}
		}
	}

	//OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMonsterAIControllerBase::TargetKey, nullptr);
	//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}