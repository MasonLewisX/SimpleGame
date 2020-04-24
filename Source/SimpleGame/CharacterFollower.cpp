// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterFollower.h"
#include <Engine.h>
#include <PublicCharacter.h>
#include <ControllerAIFollower.h>

// Sets default values
ACharacterFollower::ACharacterFollower()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacterFollower::BeginPlay()
{
	Super::BeginPlay();



	// This is the thing controlling our AI charcter
	AControllerAIFollower* AIController = Cast<AControllerAIFollower>(GetController());

	// This is the character the follower should follow
	APublicCharacter* Character = Cast<APublicCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	

	if (AIController)
	{
//		AIController->SetPlayerToFollow(Character);
	}

	
}














/*
// Called every frame
void ACharacterFollower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called to bind functionality to input
void ACharacterFollower::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

*/