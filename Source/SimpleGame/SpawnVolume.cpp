// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnVolume.h"
#include <SimpleGame.h>
#include <Engine.h>
#include <Kismet/KismetMathLibrary.h>
#include <Pickup.h>

// Sets default values
ASpawnVolume::ASpawnVolume() // �� ���ڴ� �� �κ��� 'SpawnVolume ����Ʈ����' ��� ��.
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create the Box Component to represent the spawn volume
	WhereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawn"));
	RootComponent = WhereToSpawn;

	// ������ ���� �ּ�/�ִ� �ʱⰪ �����ϱ�.
	SpawnDelayRangeLow = 1.0f;
	SpawnDelayRangeHigh = 4.5f;
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();


	
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnVolume::GetRandomPointInVolume()
{
	FVector SpawnOrigine = WhereToSpawn->Bounds.Origin;
	FVector SpawnExtent = WhereToSpawn->Bounds.BoxExtent;

	return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigine, SpawnExtent);
}


void ASpawnVolume::SetSpawningActive(bool bShouldSpawn)
{
	if (bShouldSpawn)
	{
		// Set the timer on Spawn Pickup.
		SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
		GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnPickup, SpawnDelay, false);
	}
	else
	{
		// Clear the timer on Spawn Pkckup.
		GetWorldTimerManager().ClearTimer(SpawnTimer);
	}
}


void ASpawnVolume::SpawnPickup() // �ƹ��͵� ��ȯ���� �ʰ� �������� �����ϱ⸸ ��.
{
	// If we have set something to spawn:
	if (WhatToSpawn != NULL) // NULL�� �ƴ� ������ Ȯ���� �ϸ� WhatToSpawn�� �����Ͽ������� Ȯ���� �� ����. �������� ���� ��쿣 ����ġ ���� ����� ���� �� �ֱ� ����. �׷� ��쿣 ȣ���� ���� �ʰ� �ؾߵ�.
	{
		// Check for a valid World:
		UWorld* const World = GetWorld(); // UWorld*�� �Ϲ����� Spawn �Լ��� ��� ����. UWorld�� �����Ǿ��ִ��� Ȯ���ؾߵ�.
		if (World)
		{
			// Set the spawn parameters
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this; // ������ �θ�� this�� ��. ���Ϸ��÷���Ʈ �ڵ���.(?)
			//SpawnParams.Instigator = Instigator; //���� �������� ���������, ���� ����� �Ͽ������� ����� ��.


			// ����� ��ġ�� ������ ��ġ�� ��.
			FVector SpawnLocation = GetRandomPointInVolume();

			// ������ �����ۿ� ������ �����̼� ���� �־���.
			FRotator SpawnRotation;
			SpawnRotation.Yaw = FMath::FRand() * 360.0f;
			SpawnRotation.Pitch = FMath::FRand() * 360.0f;
			SpawnRotation.Roll = FMath::FRand() * 360.0f;

			// Spawn the Pickup
			APickup* const SpawnedPickup = World->SpawnActor<APickup>(WhatToSpawn, SpawnLocation, SpawnRotation, SpawnParams); //Ȥ�� ������ ���ؼ� �� ���� �������� ������������ �����صΰ���. "="�� �� �κ���.

			SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
			GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnPickup, SpawnDelay, false);


		}

	}
}