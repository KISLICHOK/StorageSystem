// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/STWorldBuildSubsystem.h"

#include "Controllers/STResourceStorageController.h"

DEFINE_LOG_CATEGORY_STATIC(LogWorldBuildSubsystem, Log, All);

void USTWorldBuildSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	TArray<FLinearColor> Colors;

	for(auto Type: TEnumRange<EResourceType>())
	{
		auto Controller = NewObject<USTResourceStorageController>();

		FLinearColor Color = FLinearColor::MakeRandomColor();
		Color.A = 1.f;
		if(Colors.Contains(Color))
		{
			Color.R = Color.B;
		}
		Colors.Add(Color);

		Controller->Init(Type, Color);
		ResourceControllers.Add(Type, Controller);
	}
	
	UE_LOG(LogWorldBuildSubsystem, Warning, TEXT("Build Subsystem Initialized"));
}



bool USTWorldBuildSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if(this->GetClass()->IsInBlueprint() && Super::ShouldCreateSubsystem(Outer))
	{
		return true;
	}
	return false;
}


void USTWorldBuildSubsystem::StartGame()
{
	UE_LOG(LogWorldBuildSubsystem, Warning, TEXT("Start Game"));
	OnWorldBuildSubsystemInit.Execute();
}

void USTWorldBuildSubsystem::AddPreparedBuild(ASTBuild* Build)
{
	if(!IsValid(Build) || Builds.Contains(Build)) return;
	
	Builds.Add(Build);
	
	Build->InitBuild(FBuildData(DefaultMesh, Builds.Num()));

	auto Storage = Cast<ASTStorageBuild>(Build);
	if(!Storage) return;

	auto RandomNum = FMath::RandHelper(50);
	
	ResourceControllers[static_cast<EResourceType>(RandomNum)]->AddNotInitStorage(Storage);
}

void USTWorldBuildSubsystem::CreateNewStorage(TSubclassOf<ASTStorageBuild> BuildClass, const FTransform InTransform, EResourceType ResourceType, int32 StorageSize)
{
	if(!GetWorld()) return;

	FActorSpawnParameters SpawnParameters;
	
	//SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	auto Storage = GetWorld()->SpawnActor<ASTStorageBuild>(BuildClass, InTransform, SpawnParameters);
	
	if(!Storage) return;
	
	Builds.Add(Storage);
	Storage->InitBuild(FBuildData(DefaultMesh, Builds.Num()));
	
	ResourceControllers[ResourceType]->AddNotInitStorageWithParam(Storage, StorageSize);
}

TArray<USTResourceStorageController*> USTWorldBuildSubsystem::GetControllers() const
{
	TArray<USTResourceStorageController*> Controllers;
	ResourceControllers.GenerateValueArray(Controllers);
	
	return Controllers;
}


USTResourceStorageController* USTWorldBuildSubsystem::GetController(EResourceType Type) const
{
	return ResourceControllers[Type];
}

void USTWorldBuildSubsystem::StressUpdateStorages()
{
	int32 CountForUpdates = 1;
	
	for (auto Controller : ResourceControllers)
	{
		int32 Remain = Controller.Value->MaxUpgradeStorages(CountForUpdates);

		CountForUpdates = Remain < 1 ? 1 : 1 + Remain;
	}

	
}

void USTWorldBuildSubsystem::CreateControllers()
{
}
