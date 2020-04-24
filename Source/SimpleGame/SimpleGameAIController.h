// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include <SimpleGame.h>
#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <BehaviorTree/Blackboard/BlackboardKeyAllTypes.h>
#include <SimpleGameBot.h>
#include <SimpleGameCharacter.h>
#include "SimpleGameAIController.generated.h"

/**
 * 
 */

// class UBehaviorTreeComponent;
// class UBlackboardComponent;

UCLASS(config = Game)
class SIMPLEGAME_API ASimpleGameAIController : public AAIController
{
	GENERATED_BODY()

public:
	ASimpleGameAIController(const class FObjectInitializer & ObjectInitializer);

//	virtual void Possess(APawn* InPawn) override;

	void SetEnemy(class APawn* InPawn);

	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn = true) override;

	UFUNCTION(BlueprintCallable, Category = Behavior)
		void FindClosestEnemy();




private:
	UPROPERTY(transient) // transient 는 휘발성. 모두 0으로 채워짐.
		UBlackboardComponent* BlackboardComp;

	UPROPERTY(transient)
		UBehaviorTreeComponent* BehaviorComp;

protected:
	int32 EnemyKeyID;

	
};
