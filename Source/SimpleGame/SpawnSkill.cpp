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



// �𸮾� �� ������ ���� ���� ���
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

	// CollectionSphere�� ������ �Ǵ� ��� ���͸� �迭�� �޾ƿ�.
	TArray<AActor*> OverrapedActors;
	StormCollision->GetOverlappingActors(OverrapedActors); // 'GetOverlappingActors()' �Լ��� Primitive Component Ŭ������ �⺻������ �ִ� �Լ�.


	// �츮�� ������ ������ ���Ϳ���
	for (int32 iOverraped = 0; iOverraped < OverrapedActors.Num(); ++iOverraped)
	{
		// ���͸� AChracter�� ����ȯ
		ACharacter* const TargetedCharacter = Cast<ACharacter>(OverrapedActors[iOverraped]);

		// �� ��ȯ�� �����Ͽ�����, �׸��� �������� ��ȿ�ϰ� Ȱ��ȭ�Ǿ�������
		if (TargetedCharacter && !TargetedCharacter->IsPendingKill()  /*&& TargetedActor->IsActive()*/)
		{
			TargetedLocation = TargetedCharacter->GetActorLocation();
			CenterOfStorm = StormCollision->GetComponentLocation() + FVector::FVector(0.f, 0.f, 750.f); // GetTargetLocation() ��ߵǳ�?

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

					// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "ġ��Ÿ!!!");
					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, DamageDisplay);

				}

				else
				{
					DamageDisplay = UKismetStringLibrary::Conv_FloatToString(StormDamage);



					// �̰� ������ ĳ���͵� ���׵� ���Ѿߵ�.
					StormDamage = UGameplayStatics::ApplyDamage(TargetedCharacter, StormDamage, NULL, this, UDamageType::StaticClass());
					TargetedCharacter->LaunchCharacter(VelocityOfStorm * DeltaTime, false, false);


					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, DamageDisplay);


				}
			}


			


			/*// TargetedActor�� �������� �Ʊ����� Ȯ���ϱ� ���� Ȯ�� ��. (���������� �������� ���͸����� ����ȯ���� Ȯ���Ѵٰ� ��.)
			ASimpleGameCharacter* const OurCharacter = Cast<ASimpleGameCharacter>(TargetedCharacter);
			if (!OurCharacter) // �Ʊ� ĳ���Ͱ� �ƴ϶��
			{
				// ���� ������ ����. ��� �� ĳ���͵鿡��

				// ������ �� �Ŀ����� ĳ���Ϳ� �����ֱ�.
				CollectedPower += TestBattery->GetPower(); // �̷��� �ؼ� �ӿ� �� �Ŀ����� ������.

			}
			// �������� ��Ȱ��ȭ ��Ŵ
			TestPickup->SetActive(false);
			*/

		}

	}






/*
void ASpawnSkill::CheckActors()
{




}

*/








