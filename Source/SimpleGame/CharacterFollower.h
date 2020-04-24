// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <BehaviorTree/BlackboardComponent.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/BehaviorTree.h>
#include "GameFramework/Character.h"
#include "CharacterFollower.generated.h"

UCLASS()
class SIMPLEGAME_API ACharacterFollower : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterFollower();


	
	UPROPERTY(EditAnywhere, Category = "Behavior")
		class UBehaviorTree* BehaviorTree;
























protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;









	/*
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

*/
	
};
