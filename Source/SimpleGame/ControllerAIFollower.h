// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include <Engine.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/BehaviorTree.h>
#include "ControllerAIFollower.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEGAME_API AControllerAIFollower : public AAIController
{
	GENERATED_BODY()


public:

		AControllerAIFollower();

//		void Possess(APawn* Pawn) ;

//		void SetPlayerToFollow(APawn* Pawn);


protected:

	UBehaviorTreeComponent* BehaviorComp;
	UBlackboardComponent* BlackboardComp;



};
