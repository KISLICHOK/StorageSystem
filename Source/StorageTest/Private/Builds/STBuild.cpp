// Fill out your copyright notice in the Description page of Project Settings.


#include "Builds/STBuild.h"

#include "GameTypes/STCoreTypes.h"
#include "Misc/MapErrors.h"
#include "Subsystems/STWorldBuildSubsystem.h"

// Sets default values
ASTBuild::ASTBuild()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

}

USTWorldBuildSubsystem* ASTBuild::GetBuildSubsystem() 
{
	if(WorldBuildSubsystem == nullptr)
	{
		WorldBuildSubsystem = GetWorld()->GetSubsystem<USTWorldBuildSubsystem>();
	}

	return WorldBuildSubsystem;
}

void ASTBuild::InitBuild(const FBuildData BuildData)
{
	ID = BuildData.ID;

	if(!BuildData.StaticMesh || MeshComponent->GetStaticMesh()) return;
	MeshComponent->SetStaticMesh(BuildData.StaticMesh);
}

void ASTBuild::SetBuildrColor(const FLinearColor& LinearColor)
{
	const auto MaterialInst = MeshComponent->CreateAndSetMaterialInstanceDynamic(0);
	if(!MaterialInst) return;

	MaterialInst->SetVectorParameterValue(TEXT("Color"), LinearColor);
}

bool ASTBuild::Activate()
{
	return true;
}

bool ASTBuild::Deactivate()
{
	return true;
}

void ASTBuild::BeginPlay()
{
	Super::BeginPlay();

	GetBuildSubsystem();
}



