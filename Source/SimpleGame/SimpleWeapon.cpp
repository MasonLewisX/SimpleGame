// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleWeapon.h"
#include <Engine.h>
#include "components/SkeletalMeshComponent.h"


// Sets default values
ASimpleWeapon::ASimpleWeapon(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer) // "부모에 있는 작동 방식도 표현해주기 위해서 Super ObjectInitializer를 해주겠음."

{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true; // 매 프레임마다 틱 함수를 호출할지를 설정함. 필요없으면 false해서 실행속도를 향상시켜도 됨. 여기서는 없앰.

	WeaponMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("WeaponMesh"));
	WeaponMesh->CastShadow = true; // 그림자를 만들어줌
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌이 되지 않게 함.
	RootComponent = WeaponMesh;  //루트 컴포넌트로 만듦.


	WeaponCollision = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponCollision"));
	WeaponCollision->InitSphereRadius(30.0f);;
	WeaponCollision->AttachTo(WeaponMesh, "DamageSocket");

	
	DuringAttack = false;   // 이 부분이 공격하지 않았을때 데미지 충돌 일어나지 않게 하는 부분 (헤더에 39번줄 참조)
}

void ASimpleWeapon::SetPawnOwner(APublicCharacter* NewOwner)  // NewOwner 부분은 PublicCharacter 클래스에서 함수로 쓸때 플레이어냐 적군이냐에 따라 알아서 써주면 연결됨
{
	if (MyPawn != NewOwner) // MyPawn이 NewOwner와 같지 않다면, MyPawn을 NewOwner로 설정함.
	{
		MyPawn = NewOwner;
	}
}

void ASimpleWeapon::AttachMeshToPawn() // 소스파일에서, 함수 설정에 앞서서, 웨폰 메쉬를 붙이기 위해서, 
// 씬 컴포넌트에 있는 AttachTo() 함수(하나의 소켓으로 만드는 함수)를 사용하게 될텐데요, 이 함수의 인수로 앞에서 언급했던 '캐릭터 메쉬'와  '소켓 이름'이 인수로 사용됩니다. 
// 구현해보면,
{
	if (MyPawn) // MyPawn이 있다면
	{
		USkeletalMeshComponent* PawnMesh/*(그냥 어떤 스켈레탈 메쉬. *스켈레탈 메쉬가 하나의 씬 컴포넌트임.*)*/ = MyPawn/*(PublicCharacter)*/->GetSpecificPawnMesh(); // PawnMesh로 변수를 만들고, PublicCharacter에서 Mesh를 가져오겠음.
		FName AttachPoint = MyPawn->GetWeaponAttachPoint(); // 왜 이렇게 빙빙 돌리는질 모르겠음. 그냥 PublicCharacter.h에 있는 FName WeaponAatachPoint를 아래에 바로 쓰면 될것 같은데. return 하는 과정 때문인가? 그게 뭐지?
		WeaponMesh->AttachTo(PawnMesh, AttachPoint); // "웨폰 메쉬가 캐릭터 본에서 만들어진 소켓에 붙여질 것입니다."
	}

}


void ASimpleWeapon::OnEquip(const ASimpleWeapon* SelectedWeapon)
{
	AttachMeshToPawn();
}

void ASimpleWeapon::OnUnEquip()
{
	WeaponMesh->DetachFromParent(); // "DetachFromParent(); 함수도 씬 컴포넌트에 있는 함수로, 그냥 가져다 쓰면 됨."

}



void ASimpleWeapon::StartAttack()
{
	float AnimDuration = MyPawn->PlayAnimMontage(AttackAnim);

	DuringAttack = true;     // 이 부분이 공격하지 않았을때 데미지 충돌 일어나지 않게 하는 부분.

	FTimerHandle TimerHandle_StopAttack;
	GetWorldTimerManager().SetTimer(TimerHandle_StopAttack, this, &ASimpleWeapon::StopAttack, AnimDuration, false);
}

void ASimpleWeapon::StopAttack()
{
	MyPawn->StopAnimMontage(AttackAnim);
	DuringAttack = false;

}

void ASimpleWeapon::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor->IsA(AActor::StaticClass()) && DuringAttack == true)			// 이 부분이 공격하지 않았을때 데미지 충돌 일어나지 않게 하는 부분.   &&DuringAttack == true 부분
	{
		UGameplayStatics::ApplyDamage(OtherActor, 50.0f, NULL, this, UDamageType::StaticClass());

		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, "ApplyDamage!!!");
	}
}

/* 언리얼 내 데미지 공식 적용 방법

float UGameplayStatics::ApplyDamage(AActor* DamagedActor, float BaseDamage, AController* EventInstigator, AActor* DamageCauser, TSubclassOf<UDamageType> DamageTypeClass)
{
	if ( DamagedActor && (BaseDamage != 0.f) )
	{
		// make sure we have a good damage type
		TSubclassOf<UDamageType> const ValidDamageTypeClass = DamageTypeClass ? DamageTypeClass : TSubclassOf<UDamageType>(UDamageType::StaticClass());
		FDamageEvent DamageEvent(ValidDamageTypeClass);

		return DamagedActor->TakeDamage(BaseDamage, DamageEvent, EventInstigator, DamageCauser);
	}

	return 0.f;
}

*/