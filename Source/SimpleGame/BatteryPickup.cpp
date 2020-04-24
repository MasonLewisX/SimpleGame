// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryPickup.h"

//Set default values
ABatteryPickup::ABatteryPickup()
{
	GetMesh()->SetSimulatePhysics(true);

	// 배터리의 기본 값을 설정해줌.
	BatteryPower = 100.f;

}

void ABatteryPickup::WasCollected_Implementation()
{
	//베이스 픽업 스크립트 사용
	Super::WasCollected_Implementation(); //Super 키워드를 사용해서 부모 함수를 호출함.

	// 배터리 아이템 파괴하기
		Destroy();

}


// 배터리의 파워 값을 반환함.
float ABatteryPickup::GetPower()
{
	return BatteryPower;
}