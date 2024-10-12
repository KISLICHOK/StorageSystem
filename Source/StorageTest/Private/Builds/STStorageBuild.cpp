// Fill out your copyright notice in the Description page of Project Settings.


#include "Builds/STStorageBuild.h"

#include "GameTypes/STCoreTypes.h"
#include "Loaders/STLoader.h"

ASTStorageBuild::ASTStorageBuild()
{
	BringLocation = CreateDefaultSubobject<USceneComponent>("Bring Location");
	BringLocation->SetupAttachment(MeshComponent);

	PickLocation = CreateDefaultSubobject<USceneComponent>("Pick Location");
	PickLocation->SetupAttachment(MeshComponent);
}

void ASTStorageBuild::InitStorage(const FStorageBuildData StorageBuildData)
{
	ResourceType = StorageBuildData.ResourceType;
	MaxResourceCount = StorageBuildData.MaxResourceCount;

	SetUIInfo();
}


void ASTStorageBuild::SetSameStorages(const TArray<ASTStorageBuild*>& SameStorages)
{
	SameTypeStorages = SameStorages;
	RecalculateStorageDistance();
}

void ASTStorageBuild::AddNewStorage(ASTStorageBuild* InStorage)
{
	SameTypeStorages.Add(InStorage);
	RecalculateStorageDistance();
}

void ASTStorageBuild::PrepareToComeLoader(ASTLoader* InLoader, FLoadOrder Order)
{
	if (Order.bIsTakeOrder)
	{
		CurrentResourceCount -= Order.LoadCount;
		SetUIInfo();
	}
	else
	{
		CurrentResourceCount += Order.LoadCount;
	}

	//ComingLoaders.Add(InLoader);

	auto DelegateHandle = InLoader->OnFinishWork.AddUObject(this, &ThisClass::OnLoaderToCome);

	LoaderComingDelegateHandles.Add(InLoader->GetID(),DelegateHandle);
}


void ASTStorageBuild::AddResource(int32 AddingResources)
{
	const auto NextCurrentResourceCount = FMath::Clamp(AddingResources + CurrentResourceCount, 0, MaxResourceCount);
	const auto ResourceCountDelta = NextCurrentResourceCount - CurrentResourceCount;

	CurrentResourceCount = NextCurrentResourceCount;
	SetUIInfo();
	ResourceCountWasChanged(CurrentResourceCount, ResourceCountDelta);
}

void ASTStorageBuild::ResourceCountWasChanged(int32 ResourceCount, int ResourceCountDelta) const
{
	OnCurrentResourcesChangedByPlayer.Broadcast(FStorageInfo(ID, MaxResourceCount, ResourceType,
	                                                         ResourceCount, CurrentResourceCount - ResourceCountDelta));
}

float ASTStorageBuild::GetPercent() const
{
	return CurrentResourceCount / MaxResourceCount;
}

FStorageUIInfo ASTStorageBuild::GetUIInfo() const
{
	return UIInfo;
}

void ASTStorageBuild::SetUIInfo()
{
	UIInfo.StorageID = ID;
	UIInfo.MaxResourceCount = MaxResourceCount;
	UIInfo.ResourceType = ResourceType;
	UIInfo.LoadCount = CurrentResourceCount;

	OnChangeUIInfo.Broadcast(UIInfo);
}


void ASTStorageBuild::RecalculateStorageDistance()
{
	SameTypeStorages.Sort([this](const ASTStorageBuild& A, const ASTStorageBuild& B)
	{
		float DistanceA = A.GetSquaredDistanceTo(this);
		float DistanceB = B.GetSquaredDistanceTo(this);

		return DistanceA < DistanceB;
	});
}

void ASTStorageBuild::OnLoaderToCome(ASTLoader* Loader, FLoadOrder Order)
{
	
	if (LoaderComingDelegateHandles.Contains(Loader->GetID()))
	{
		Loader->OnFinishWork.Remove(LoaderComingDelegateHandles[Loader->GetID()]);
	}

	SetUIInfo();

	//Loader->OnFinishWork.Remove(this, &ThisClass::OnLoaderToCome);
}

void ASTStorageBuild::MaxUpgrade()
{
	MaxResourceCount = 500;
	CurrentResourceCount = 500;
}
