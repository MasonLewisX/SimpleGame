// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Engine.h>
#include "GameFramework/Actor.h"
#include "SpawnSkill.generated.h"

UCLASS()
class SIMPLEGAME_API ASpawnSkill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnSkill();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;




public:


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Storm")
		float EnergyConsumed;


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TargetedActors")
		FVector TargetedLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TargetedActors")
		FVector CenterOfStorm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TargetedActors")
		FVector VelocityOfStorm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TargetedActors")
		float StormDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TargetedActors")
		float RandomFloat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TargetedActors")
		FString DamageDisplay;

	UFUNCTION(BlueprintCallable, Category = "Critical")
		void SetCriticalHit(bool IsCritical);

	UFUNCTION(BlueprintPure, Category = "Critical")
		bool CriticalHit();

	bool bCritical;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Critical")
		float CriticalProbability;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Critical")
		float CriticalTimes;


	

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TargetedActors")
	//	void MakingRandomFlaot();


	// ������ Ž���� ��ü �ݸ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* StormCollision;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sphere, meta = (AllowPrivateAccess = "true"))
	//	class UStaticMeshComponent* Center;

	//�� �Լ��� ��ȭ�Ǹ� ������. CollisionCapsule �ӿ� ����ִ� ���͵��� ACharacter Ŭ�������� üũ��.
	//UFUNCTION(BlueprintCallable, Category = "CheckActors")
	//	void CheckActors();


private:


};
