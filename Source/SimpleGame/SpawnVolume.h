// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Engine.h>
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class SIMPLEGAME_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Returns the WhereToSpawn Subobject
	FORCEINLINE class UBoxComponent* GetWhereToSpawn() const { return WhereToSpawn; }

	// Find a random point within the BoxComponent
	UFUNCTION(BlueprintPure, Category = "Spawning")
	FVector GetRandomPointInVolume();


	// Volume Spawn�� Pickup Spawn �۵��� �����ϴ� �Լ�. This function toggles whether or not the spawn volume spawns pickups.
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SetSpawningActive(bool bShouldSpawn);






protected:
	// The Pickup to Spawn //
	UPROPERTY(EditAnywhere, Category = "Spawning") // ������ �гο��� ���� ������ �� �ֵ��� �ϱ� ���� "EditAnywhere" �ɼ��� ����(�̷��� �ϸ� �������Ʈ������ ����).
	TSubclassOf<class APickup> WhatToSpawn;

	FTimerHandle SpawnTimer; // FTimerHandle ���� "� ��"�� �ƴϱ� ������ UPROPERTY ��ũ�θ� ���� �ʿ䰡 ����.


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	// ������ �ּ� ���� �ð�
	float SpawnDelayRangeLow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	// ������ �ִ� ���� �ð�
	float SpawnDelayRangeHigh;



private:
	// Box Component to specify where pickups should be spawned
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* WhereToSpawn;

	// �� ������ ������ �ٷ�.
	void SpawnPickup();


	// ���� ������ ���� ������ ��.
	float SpawnDelay;


};
