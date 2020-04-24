// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponPickup.h"

AWeaponPickup::AWeaponPickup()
{
	GetMesh()->SetSimulatePhysics(true);


}


void AWeaponPickup::WasCollected_Implementation()
{
	Super::WasCollected_Implementation();


}