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


	// Volume Spawn의 Pickup Spawn 작동을 조정하는 함수. This function toggles whether or not the spawn volume spawns pickups.
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SetSpawningActive(bool bShouldSpawn);






protected:
	// The Pickup to Spawn //
	UPROPERTY(EditAnywhere, Category = "Spawning") // 디테일 패널에서 직접 설정할 수 있도록 하기 위해 "EditAnywhere" 옵션을 넣음(이렇게 하면 블루프린트에서도 가능).
	TSubclassOf<class APickup> WhatToSpawn;

	FTimerHandle SpawnTimer; // FTimerHandle 형은 "어떤 값"이 아니기 때문에 UPROPERTY 매크로를 써줄 필요가 없음.


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	// 생성시 최소 지연 시간
	float SpawnDelayRangeLow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	// 생성시 최대 지연 시간
	float SpawnDelayRangeHigh;



private:
	// Box Component to specify where pickups should be spawned
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* WhereToSpawn;

	// 새 아이템 생성을 다룸.
	void SpawnPickup();


	// 현재 아이템 생성 딜레이 값.
	float SpawnDelay;


};
