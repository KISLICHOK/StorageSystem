// Fill out your copyright notice in the Description page of Project Settings.


#include "Loaders/STLoader.h"
#include "Builds/STStorageBuild.h"
#include "Controllers/STLoadersController.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Subsystems/STWorldLoadersSubsystem.h"

// Sets default values
ASTLoader::ASTLoader()
{
	PrimaryActorTick.bCanEverTick = false;

	FloatingPawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("Floating Pawn Movement Component");
}


void ASTLoader::BeginPlay()
{
	Super::BeginPlay();

	auto LoaderSubsystem = GetWorld()->GetSubsystem<USTWorldLoadersSubsystem>();
	checkf(LoaderSubsystem, TEXT("BeginPlay Loadersubsystem not exist"));

	LoadersController = LoaderSubsystem->GetLoaderController();
	checkf(LoadersController, TEXT("BeginPlay Loader Controller not exist"));
}


void ASTLoader::SetOrder(FLoadOrder InOrder)
{
	CurrentOrder = InOrder;
	
	if (InOrder.bIsTakeOrder)
	{
		SwitchLoaderState(ELoaderState::GoingTakeLoadState);
	}
	else
	{
		SwitchLoaderState(ELoaderState::GoingBringLoadState);
	}
}

void ASTLoader::SwitchLoaderState(ELoaderState NextLoaderState)
{
	switch (NextLoaderState)
	{
	case ELoaderState::GoingTakeLoadState:
		
		IntoTakeLoadState();
		SetLoaderState(ELoaderState::GoingTakeLoadState);
		OnStartTakeLoad.Broadcast(this, CurrentOrder);
		
		break;
		
	case ELoaderState::GoingBringLoadState:
		
		IntoBringLoadState();
		SetLoaderState(ELoaderState::GoingBringLoadState);
		OnStartBringLoad.Broadcast(this, CurrentOrder);
		
		break;
		
	case ELoaderState::IdleState:
		
		IntoFinishWorkState();
		SetLoaderState(ELoaderState::IdleState);
		
		break;
		
	default:
		checkf(false, TEXT("ASTLoader::SwitchLoaderState not reached"));
	}
}

void ASTLoader::SetLoaderState(ELoaderState NewLoaderState)
{
	LoaderState = NewLoaderState;
}

void ASTLoader::IntoTakeLoadState()
{
	CurrentCount = FMath::Clamp(CurrentCount + CurrentOrder.LoadCount, 0, MaxCapacity);
	CurrentOrder.Storage->PrepareToComeLoader(this, CurrentOrder);
}

void ASTLoader::IntoBringLoadState()
{
	CurrentCount = FMath::Clamp(CurrentCount - CurrentOrder.LoadCount, 0, MaxCapacity);
	CurrentOrder.Storage->PrepareToComeLoader(this, CurrentOrder);
}

void ASTLoader::IntoFinishWorkState()
{
	CurrentOrder = FLoadOrder();
	//CurrentCount = 0;
	
}

void ASTLoader::ReachedDestination()
{
	auto OldOrder = CurrentOrder;
	SwitchLoaderState(ELoaderState::IdleState);
	OnFinishWork.Broadcast(this, OldOrder);
}

void ASTLoader::ResetLoader()
{
	LoaderState = ELoaderState::IdleState;
	OnFinishWork.Clear();
}

FVector ASTLoader::GetTargetLocation()
{
	return CurrentOrder.Storage->GetActorLocation() += FVector(100.f, 0.f, 0.f) ;
}

ASTBuild* ASTLoader::GetTargetBuild() const
{
	return CurrentOrder.Storage;
}


//Test
