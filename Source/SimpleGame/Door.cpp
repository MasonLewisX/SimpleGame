// Fill out your copyright notice in the Description page of Project Settings.

#include "Door.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();

	OpenDoor();

	
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float t = GetGameTimeSinceCreation();

	FRotator DoorRotation = GetActorRotation(); // NewRotaion�� �ʱⰪ�� GetActorRotation(); �Լ��� ������Ʈ ���� ���Ϳ��� ���� ��������. GetActorRotation()�� ���� �� ��𿡼��� �� �� �ִ� �Լ���. FORCEINLINE ��ũ��. ���� ������ ��.
	DoorRotation.Yaw = 10.f * t; // ���� ȸ���� ���¿��� 90���� �� �ؼ� ��.(�̹� ��� ������ ȸ���Ǿ� �ִ� ���¿��ٰ�)
	SetActorRotation(DoorRotation);

}


void ADoor::OpenDoor() // ����� 1����. �׷��� float t �κ��� �̻��ϰ� ������� ����.
{
	FRotator DoorRotation = GetActorRotation(); // NewRotaion�� �ʱⰪ�� GetActorRotation(); �Լ��� ������Ʈ ���� ���Ϳ��� ���� ��������. GetActorRotation()�� ���� �� ��𿡼��� �� �� �ִ� �Լ���. FORCEINLINE ��ũ��. ���� ������ ��.
	DoorRotation.Yaw = 90.f; // ���� ȸ���� ���¿��� 90���� �� �ؼ� ��.(�̹� ��� ������ ȸ���Ǿ� �ִ� ���¿��ٰ�)
	//SetActorRotation(DoorRotation);
}

