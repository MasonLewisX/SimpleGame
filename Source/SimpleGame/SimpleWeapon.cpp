// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleWeapon.h"
#include <Engine.h>
#include "components/SkeletalMeshComponent.h"


// Sets default values
ASimpleWeapon::ASimpleWeapon(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer) // "�θ� �ִ� �۵� ��ĵ� ǥ�����ֱ� ���ؼ� Super ObjectInitializer�� ���ְ���."

{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true; // �� �����Ӹ��� ƽ �Լ��� ȣ�������� ������. �ʿ������ false�ؼ� ����ӵ��� �����ѵ� ��. ���⼭�� ����.

	WeaponMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("WeaponMesh"));
	WeaponMesh->CastShadow = true; // �׸��ڸ� �������
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // �浹�� ���� �ʰ� ��.
	RootComponent = WeaponMesh;  //��Ʈ ������Ʈ�� ����.


	WeaponCollision = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponCollision"));
	WeaponCollision->InitSphereRadius(30.0f);;
	WeaponCollision->AttachTo(WeaponMesh, "DamageSocket");

	
	DuringAttack = false;   // �� �κ��� �������� �ʾ����� ������ �浹 �Ͼ�� �ʰ� �ϴ� �κ� (����� 39���� ����)
}

void ASimpleWeapon::SetPawnOwner(APublicCharacter* NewOwner)  // NewOwner �κ��� PublicCharacter Ŭ�������� �Լ��� ���� �÷��̾�� �����̳Ŀ� ���� �˾Ƽ� ���ָ� �����
{
	if (MyPawn != NewOwner) // MyPawn�� NewOwner�� ���� �ʴٸ�, MyPawn�� NewOwner�� ������.
	{
		MyPawn = NewOwner;
	}
}

void ASimpleWeapon::AttachMeshToPawn() // �ҽ����Ͽ���, �Լ� ������ �ռ���, ���� �޽��� ���̱� ���ؼ�, 
// �� ������Ʈ�� �ִ� AttachTo() �Լ�(�ϳ��� �������� ����� �Լ�)�� ����ϰ� ���ٵ���, �� �Լ��� �μ��� �տ��� ����ߴ� 'ĳ���� �޽�'��  '���� �̸�'�� �μ��� ���˴ϴ�. 
// �����غ���,
{
	if (MyPawn) // MyPawn�� �ִٸ�
	{
		USkeletalMeshComponent* PawnMesh/*(�׳� � ���̷�Ż �޽�. *���̷�Ż �޽��� �ϳ��� �� ������Ʈ��.*)*/ = MyPawn/*(PublicCharacter)*/->GetSpecificPawnMesh(); // PawnMesh�� ������ �����, PublicCharacter���� Mesh�� ����������.
		FName AttachPoint = MyPawn->GetWeaponAttachPoint(); // �� �̷��� ���� �������� �𸣰���. �׳� PublicCharacter.h�� �ִ� FName WeaponAatachPoint�� �Ʒ��� �ٷ� ���� �ɰ� ������. return �ϴ� ���� �����ΰ�? �װ� ����?
		WeaponMesh->AttachTo(PawnMesh, AttachPoint); // "���� �޽��� ĳ���� ������ ������� ���Ͽ� �ٿ��� ���Դϴ�."
	}

}


void ASimpleWeapon::OnEquip(const ASimpleWeapon* SelectedWeapon)
{
	AttachMeshToPawn();
}

void ASimpleWeapon::OnUnEquip()
{
	WeaponMesh->DetachFromParent(); // "DetachFromParent(); �Լ��� �� ������Ʈ�� �ִ� �Լ���, �׳� ������ ���� ��."

}



void ASimpleWeapon::StartAttack()
{
	float AnimDuration = MyPawn->PlayAnimMontage(AttackAnim);

	DuringAttack = true;     // �� �κ��� �������� �ʾ����� ������ �浹 �Ͼ�� �ʰ� �ϴ� �κ�.

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

	if (OtherActor->IsA(AActor::StaticClass()) && DuringAttack == true)			// �� �κ��� �������� �ʾ����� ������ �浹 �Ͼ�� �ʰ� �ϴ� �κ�.   &&DuringAttack == true �κ�
	{
		UGameplayStatics::ApplyDamage(OtherActor, 50.0f, NULL, this, UDamageType::StaticClass());

		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, "ApplyDamage!!!");
	}
}

/* �𸮾� �� ������ ���� ���� ���

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