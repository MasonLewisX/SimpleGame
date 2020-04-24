// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryPickup.h"

//Set default values
ABatteryPickup::ABatteryPickup()
{
	GetMesh()->SetSimulatePhysics(true);

	// ���͸��� �⺻ ���� ��������.
	BatteryPower = 100.f;

}

void ABatteryPickup::WasCollected_Implementation()
{
	//���̽� �Ⱦ� ��ũ��Ʈ ���
	Super::WasCollected_Implementation(); //Super Ű���带 ����ؼ� �θ� �Լ��� ȣ����.

	// ���͸� ������ �ı��ϱ�
		Destroy();

}


// ���͸��� �Ŀ� ���� ��ȯ��.
float ABatteryPickup::GetPower()
{
	return BatteryPower;
}