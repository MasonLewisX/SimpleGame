// Fill out your copyright notice in the Description page of Project Settings.

#include "SimpleGameAIController.h"
#include <SimpleGame.h>
#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <BehaviorTree/Blackboard/BlackboardKeyAllTypes.h>
#include <SimpleGameBot.h>
#include <SimpleGameCharacter.h>

ASimpleGameAIController::ASimpleGameAIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	BrainComponent = BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
}


/*void ASimpleGameAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	ASimpleGameBot* Bot = Cast<ASimpleGameBot>(InPawn);
	if (Bot && Bot->BotBehavior)
	{
		if (Bot->BotBehavior->BlackboardAsset)
		{
			Blackboard->InitializeBlackboard(*Bot->BotBehavior->BlackboardAsset);
		}
		EnemyKeyID = BlackboardComp->GetKeyID("Enemy");
		BehaviorComp->StartTree(*(Bot->BotBehavior));
	}

	
}*/

void ASimpleGameAIController::SetEnemy(class APawn* InPawn)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValue<UBlackboardKeyType_Object>(EnemyKeyID, InPawn);
		SetFocus(InPawn);
	
	}
	
}

void ASimpleGameAIController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
	FVector FocalPoint = GetFocalPoint();
	if (!FocalPoint.IsZero() && GetPawn())
	{
		FVector Direction = FocalPoint - GetPawn()->GetActorLocation();
		FRotator NewControlRotation = Direction.Rotation();

		NewControlRotation.Yaw = FRotator::ClampAxis(NewControlRotation.Yaw);

		SetControlRotation(NewControlRotation);

		APawn* const P = GetPawn();
		if (P && bUpdatePawn)
		{
			P->FaceRotation(NewControlRotation, DeltaTime);
		}

	}
}



void ASimpleGameAIController::FindClosestEnemy()
{
	APawn* MyBot = GetPawn();
	if (MyBot == NULL)
	{
		return;
	}

	const FVector MyLocal = MyBot->GetActorLocation();
	float BestDistSq = 3000.0f;
	ASimpleGameCharacter* BestPawn = NULL;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		ASimpleGameCharacter* TestPawn = Cast<ASimpleGameCharacter>(*It);
		if (TestPawn)
		{
			const float DistSq = (TestPawn->GetActorLocation() - MyLocal).SizeSquared();
			if (DistSq < BestDistSq)
			{
				BestDistSq = DistSq;
				BestPawn = TestPawn;


			}
		}
	}
	if (BestPawn)
	{
		SetEnemy(BestPawn);
	}

}