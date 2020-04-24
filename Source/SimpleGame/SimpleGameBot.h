// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PublicCharacter.h"
#include "SimpleGameBot.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEGAME_API ASimpleGameBot : public APublicCharacter
{
	GENERATED_BODY()
	
public:

	ASimpleGameBot();

	UPROPERTY(VisibleAnywhere, Category = "Collision")
		class USphereComponent* CollisionSphere;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void FaceRotation(FRotator NewRotation, float Deltatime = 0.f) override;

	void PlayMeleeAnim();
	void StopMeleeAnim();

	UPROPERTY(EditAnywhere, Category = "Behavior")
		class UBehaviorTree* BotBehavior;
	

	UPROPERTY(EditDefaultsOnly, Category = "Pawn")
		UAnimMontage* MeleeAnim;


protected:



private:
	FTimerHandle TimerHandle_EnemyStopAttack;

	bool DuringAttack;


};
