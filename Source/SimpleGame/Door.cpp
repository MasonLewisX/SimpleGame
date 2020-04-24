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

	FRotator DoorRotation = GetActorRotation(); // NewRotaion의 초기값을 GetActorRotation(); 함수로 프로젝트 내의 액터에서 따와 설정해줌. GetActorRotation()는 엔진 내 어디에서나 쓸 수 있는 함수임. FORCEINLINE 매크로. 정의 참조할 것.
	DoorRotation.Yaw = 10.f * t; // 현재 회전값 상태에다 90도를 더 해서 줌.(이미 어느 정도로 회전되어 있는 상태에다가)
	SetActorRotation(DoorRotation);

}


void ADoor::OpenDoor() // 여기는 1번만. 그런데 float t 부분은 이상하게 적용되지 않음.
{
	FRotator DoorRotation = GetActorRotation(); // NewRotaion의 초기값을 GetActorRotation(); 함수로 프로젝트 내의 액터에서 따와 설정해줌. GetActorRotation()는 엔진 내 어디에서나 쓸 수 있는 함수임. FORCEINLINE 매크로. 정의 참조할 것.
	DoorRotation.Yaw = 90.f; // 현재 회전값 상태에다 90도를 더 해서 줌.(이미 어느 정도로 회전되어 있는 상태에다가)
	//SetActorRotation(DoorRotation);
}

