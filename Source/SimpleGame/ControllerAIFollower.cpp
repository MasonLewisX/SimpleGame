// Fill out your copyright notice in the Description page of Project Settings.

#include "ControllerAIFollower.h"
#include <BehaviorTree/BlackboardComponent.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/BehaviorTree.h>
#include <CharacterFollower.h>

AControllerAIFollower::AControllerAIFollower()
{
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(FName("BehaviorComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(FName("BlackboardComp")); 
}

/*void AControllerAIFollower::Possess(APawn * Pawn)
{

	//Super::Possess(Pawn);

	ACharacterFollower* AICharacter = Cast<ACharacterFollower>(Pawn);

	if (AICharacter)
	{
		if (AICharacter->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*(AICharacter->BehaviorTree->BlackboardAsset));
			BehaviorComp->StartTree(*AICharacter->BehaviorTree);
		}
	}

}*/



/*void AControllerAIFollower::SetPlayerToFollow(APawn* Pawn)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(FName("PlayerToFollower"), Pawn);
	}
}*/