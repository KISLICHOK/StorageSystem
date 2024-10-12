// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/STWorldLoadersSubsystem.h"

#include "Controllers/STLoadersController.h"
#include "LevelInstance/LevelInstanceTypes.h"
#include "Loaders/STLoader.h"
#include "Subsystems/STWorldBuildSubsystem.h"

DEFINE_LOG_CATEGORY_STATIC(LogWorldLoaderSubsystem, All, All);

void USTWorldLoadersSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadersController = NewObject<USTLoadersController>(GetWorld());
	checkf(LoadersController, TEXT("LoadersControllerNotInit"));
	
	
	UE_LOG(LogWorldLoaderSubsystem, Warning, TEXT("Loader Subsystem Initialized"));
}

void USTWorldLoadersSubsystem::StartGame()
{
	UE_LOG(LogWorldLoaderSubsystem, Warning, TEXT("Loader Subsystem Start Game"));
	
	auto BuildSubsystem = GetWorld()->GetSubsystem<USTWorldBuildSubsystem>();
	checkf(BuildSubsystem, TEXT("Build Subsystem not create"));

	BuildSubsystem->OnWorldBuildSubsystemInit.BindLambda([this]()
	{
		LoadersController->Init();
	});
	
}

bool USTWorldLoadersSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if(this->GetClass()->IsInBlueprint() && Super::ShouldCreateSubsystem(Outer))
	{
		return true;
	}
	return false;
}

void USTWorldLoadersSubsystem::CreateDefaultLoader(const FTransform InTransform)
{
	if(!GetWorld()) return;
	
	auto Loader = GetWorld()->SpawnActor<ASTLoader>(DefaultLoaderClass, InTransform);
	if(!Loader) return;
	
	Loader->SetID(Loaders.Num());
	
	Loaders.Add(Loader);
	
	OnAddNewLoader.Broadcast(Loader);
}

void USTWorldLoadersSubsystem::AddPreparedLoader(ASTLoader* Loader)
{
	Loader->SetID(Loaders.Num());
	Loaders.Add(Loader);
	OnAddNewLoader.Broadcast(Loader);
}

bool USTWorldLoadersSubsystem::HasLoaderByID(int32 InID)
{
	return Loaders.ContainsByPredicate([InID](ASTLoader* Loader)
	{
		return Loader->GetID() == InID;
	});
}

ASTLoader* USTWorldLoadersSubsystem::GetLoaderByID(int32 InID)
{
	if(!HasLoaderByID(InID)) return nullptr;
	return Loaders[InID];
}


