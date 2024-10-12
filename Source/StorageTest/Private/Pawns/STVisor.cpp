// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/STVisor.h"

// Sets default values
ASTVisor::ASTVisor()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


void ASTVisor::BeginPlay()
{
	Super::BeginPlay();
	
}


void ASTVisor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ASTVisor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASTVisor::Move(const FInputActionValue& Value)
{
}

void ASTVisor::OnClickAction(const FInputActionValue& Value)
{
}

