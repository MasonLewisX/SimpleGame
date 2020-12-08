// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnVolume.h"
#include <SimpleGame.h>
#include <Engine.h>
#include <Kismet/KismetMathLibrary.h>
#include <Pickup.h>

// Sets default values
ASpawnVolume::ASpawnVolume() // 이 여자는 이 부분이 'SpawnVolume 컨스트럭터' 라고 함.
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create the Box Component to represent the spawn volume
	WhereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawn"));
	RootComponent = WhereToSpawn;

	// 아이템 생성 최소/최대 초기값 설정하기.
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


void ASpawnVolume::SpawnPickup() // 아무것도 반환하지 않고 아이템을 생성하기만 함.
{
	// If we have set something to spawn:
	if (WhatToSpawn != NULL) // NULL이 아닐 때인지 확인을 하면 WhatToSpawn을 설정하였는지를 확인할 수 있음. 설정하지 않은 경우엔 예상치 못한 결과가 나올 수 있기 때문. 그런 경우엔 호출이 되지 않게 해야됨.
	{
		// Check for a valid World:
		UWorld* const World = GetWorld(); // UWorld*에 일반적인 Spawn 함수가 들어 있음. UWorld가 설정되어있는지 확인해야됨.
		if (World)
		{
			// Set the spawn parameters
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this; // 생성시 부모는 this가 됨. 보일러플레이트 코드임.(?)
			//SpawnParams.Instigator = Instigator; //누가 아이템을 만들었는지, 누가 만들게 하였는지를 명시해 줌.


			// 만드는 위치에 랜덤한 위치를 고름.
			FVector SpawnLocation = GetRandomPointInVolume();

			// 생성한 아이템에 랜덤한 로테이션 값을 넣어줌.
			FRotator SpawnRotation;
			SpawnRotation.Yaw = FMath::FRand() * 360.0f;
			SpawnRotation.Pitch = FMath::FRand() * 360.0f;
			SpawnRotation.Roll = FMath::FRand() * 360.0f;

			// Spawn the Pickup
			APickup* const SpawnedPickup = World->SpawnActor<APickup>(WhatToSpawn, SpawnLocation, SpawnRotation, SpawnParams); //혹시 나중을 위해서 이 값을 아이템의 포인터형으로 저장해두겠음. "="의 앞 부분임.

			SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
			GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnPickup, SpawnDelay, false);


		}

	}
}