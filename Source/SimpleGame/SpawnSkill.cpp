// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnSkill.h"
#include <Engine.h>
#include <SimpleGameCharacter.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetStringLibrary.h>

// Sets default values
ASpawnSkill::ASpawnSkill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StormCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	RootComponent = StormCollision;

	//Center = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CenterOfTheStorm"));
	
}

// Called when the game starts or when spawned
void ASpawnSkill::BeginPlay()
{
	Super::BeginPlay();
	
	ASimpleGameCharacter* MyCharacter = Cast<ASimpleGameCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	EnergyConsumed = MyCharacter->GetCurrentPower() * 0.7f;

	MyCharacter->SetCurrentPower(-EnergyConsumed);

	

	CriticalTimes = 62.f;

	SetLifeSpan(12);
	

}



// 언리얼 내 데미지 공식 적용 방법
/*
float UGameplayStatics::ApplyDamage(AActor* DamagedActor, float Damage, AController* EventInstigator, AActor* DamageCauser, TSubclassOf<UDamageType> DamageTypeClass)
{
	if (DamagedActor && (Damage != 0.f))
	{
		// make sure we have a good damage type
		TSubclassOf<UDamageType> const ValidDamageTypeClass = DamageTypeClass ? DamageTypeClass : TSubclassOf<UDamageType>(UDamageType::StaticClass());
		FDamageEvent DamageEvent(ValidDamageTypeClass);

		return DamagedActor->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	}

	return 0.f;
}
*/


/*
void ASpawnSkill::MakingRandomFloat()
{

}

*/


void ASpawnSkill::SetCriticalHit(bool IsCritical)
{
	bCritical = IsCritical;
}

bool ASpawnSkill::CriticalHit()
{
	return bCritical;
}





// Called every frame
void ASpawnSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// CollectionSphere와 오버랩 되는 모든 액터를 배열로 받아옴.
	TArray<AActor*> OverrapedActors;
	StormCollision->GetOverlappingActors(OverrapedActors); // 'GetOverlappingActors()' 함수는 Primitive Component 클래스에 기본적으로 있는 함수.


	// 우리가 수집한 각각의 액터에서
	for (int32 iOverraped = 0; iOverraped < OverrapedActors.Num(); ++iOverraped)
	{
		// 액터를 AChracter로 형변환
		ACharacter* const TargetedCharacter = Cast<ACharacter>(OverrapedActors[iOverraped]);

		// 형 변환이 성공하였을때, 그리고 아이템이 유효하고 활성화되어있을때
		if (TargetedCharacter && !TargetedCharacter->IsPendingKill()  /*&& TargetedActor->IsActive()*/)
		{
			TargetedLocation = TargetedCharacter->GetActorLocation();
			CenterOfStorm = StormCollision->GetComponentLocation() + FVector::FVector(0.f, 0.f, 750.f); // GetTargetLocation() 써야되나?

			VelocityOfStorm = 5.f*(CenterOfStorm - TargetedLocation);
			


			//ASimpleGameCharacter* MyCharacter = Cast<ASimpleGameCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

			//EnergyConsumed = MyCharacter->GetCurrentPower() * 0.7f;
			
			RandomFloat = UKismetMathLibrary::RandomFloatInRange(80, 100);
			StormDamage = 3.f * RandomFloat * DeltaTime * EnergyConsumed;


			CriticalProbability = UKismetMathLibrary::RandomFloatInRange(0, 100);

			if (CriticalProbability <= 3)
			{
				SetCriticalHit(true);

			}
			else
			{
				SetCriticalHit(false);

			}
			
				if (CriticalHit() == true)
				{
					StormDamage = CriticalTimes * StormDamage;

					StormDamage = UGameplayStatics::ApplyDamage(TargetedCharacter, StormDamage, NULL, this, UDamageType::StaticClass());


					DamageDisplay = UKismetStringLibrary::Conv_FloatToString(StormDamage);

					// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "치명타!!!");
					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, DamageDisplay);

				}

				else
				{
					DamageDisplay = UKismetStringLibrary::Conv_FloatToString(StormDamage);



					// 이거 이전에 캐릭터들 레그돌 시켜야됨.
					StormDamage = UGameplayStatics::ApplyDamage(TargetedCharacter, StormDamage, NULL, this, UDamageType::StaticClass());
					TargetedCharacter->LaunchCharacter(VelocityOfStorm * DeltaTime, false, false);


					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, DamageDisplay);


				}
			}


			


			/*// TargetedActor가 적군인지 아군인지 확인하기 위해 확인 함. (번역에서는 아이템이 배터리인지 형변환으로 확인한다고 함.)
			ASimpleGameCharacter* const OurCharacter = Cast<ASimpleGameCharacter>(TargetedCharacter);
			if (!OurCharacter) // 아군 캐릭터가 아니라면
			{
				// 이하 데미지 적용. 모든 적 캐릭터들에게

				// 아이템 속 파워값을 캐릭터에 더해주기.
				CollectedPower += TestBattery->GetPower(); // 이렇게 해서 속에 든 파워값을 가져옴.

			}
			// 아이템을 비활성화 시킴
			TestPickup->SetActive(false);
			*/

		}

	}






/*
void ASpawnSkill::CheckActors()
{




}

*/








