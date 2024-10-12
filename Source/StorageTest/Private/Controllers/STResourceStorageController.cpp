// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/STResourceStorageController.h"

DEFINE_LOG_CATEGORY_STATIC(LogResourceStorageController, All, All);

void USTResourceStorageController::Init(EResourceType InResourceType, const FLinearColor InColor)
{
	this->ResourceType = InResourceType;
	ResourceColor = InColor;
}


void USTResourceStorageController::AddNotInitStorage(ASTStorageBuild* Storage)
{
	if (!IsValid(Storage)) return;

	FStorageBuildData StorageData;

	switch (FMath::RandHelper(4))
	{
	case 0:
		StorageData.MaxResourceCount = 10;
		break;
	case 1:
		StorageData.MaxResourceCount = 50;
		break;
	case 2:
		StorageData.MaxResourceCount = 100;
		break;
	case 3:
		StorageData.MaxResourceCount = 500;
		break;
	default:
		StorageData.MaxResourceCount = 250;
		break;
	}

	StorageData.ResourceType = ResourceType;
	Storage->InitStorage(StorageData);

	AddStorage(Storage);
}

void USTResourceStorageController::AddNotInitStorageWithParam(ASTStorageBuild* Storage, int32 Size)
{
	if (Size <= 0) AddNotInitStorage(Storage);

	FStorageBuildData StorageData;
	StorageData.ResourceType = ResourceType;
	StorageData.MaxResourceCount = Size;
	Storage->InitStorage(StorageData);

	AddStorage(Storage);
}

bool USTResourceStorageController::AddStorage(ASTStorageBuild* InStorage)
{
	if (!IsValid(InStorage) || ResourceStorages.Contains(InStorage)) return false;

	InStorage->SetSameStorages(ResourceStorages);
	for (auto Storage : ResourceStorages)
	{
		Storage->AddNewStorage(InStorage);
	}

	ResourceStorages.Add(InStorage);

	InStorage->SetBuildrColor(ResourceColor);

	InStorage->OnCurrentResourcesChangedByPlayer.AddDynamic(this, &ThisClass::OnChangedResourceStorageByPlayer);


	RecalculateDataWithNewStorage(InStorage);
	return true;
}

void USTResourceStorageController::RecalculateDataWithNewStorage(const ASTStorageBuild* NewStorage)
{
	TotalNumberOfResources += NewStorage->GetCurrentResourceCount();
	MaxNumberOfResources += NewStorage->GetMaxResourceCount();

	TotalLoadPercent = TotalNumberOfResources / MaxNumberOfResources;
	RecalculateAllPercents();
}

void USTResourceStorageController::RecalculateData(const int32 Changes)
{
	UE_LOG(LogResourceStorageController, Display, TEXT("ResourceStorageController RecalculateData"));
	TotalNumberOfResources -= Changes;

	TotalLoadPercent = static_cast<double>(TotalNumberOfResources) / static_cast<double>(MaxNumberOfResources);
	RecalculateAllPercents();
}

void USTResourceStorageController::RecalculateAllPercents()
{
	TArray<FLoadOrder> NeedToAdd; //first ID second Delta
	TArray<FLoadOrder> NeedToRemove;
	int32 SummToAdd = 0;
	int32 SummToRemove = 0;


	for (auto Storage : ResourceStorages)
	{
		int TargetLoad = FMath::FloorToInt(Storage->GetMaxResourceCount() * TotalLoadPercent);
		int Needed = TargetLoad - Storage->GetCurrentResourceCount();

		if (Needed == 0)
		{
			continue;
		}
		else if (Needed > 0)
		{
			SummToAdd += Needed;
			FLoadOrder Load{Storage, Needed, ResourceType, false};
			NeedToAdd.Emplace(Load);
		}
		else
		{
			SummToRemove -= Needed;
			NeedToRemove.Emplace(FLoadOrder(Storage, -Needed, ResourceType, true));
		}
	}


	if (SummToRemove == 0 || SummToAdd == 0) return;

	NeedToAdd.Sort([](const FLoadOrder& A, const FLoadOrder& B)
	{
		return A.LoadCount > B.LoadCount;
	});

	NeedToRemove.Sort([](const FLoadOrder& A, const FLoadOrder& B)
	{
		return A.LoadCount > B.LoadCount;
	});

	
	const int32 Delta = SummToRemove - SummToAdd;
	

	if (Delta < 0)
	{
		int Remain = FMath::Abs(Delta);

		for (auto It = NeedToAdd.CreateIterator(); It; ++It)
		{
			It->LoadCount -= Remain;

			if (It->LoadCount == 0)
			{
				It.RemoveCurrent();
				break;
			}
			else if (It->LoadCount < 0)
			{
				Remain = FMath::Abs(It->LoadCount);
				It->LoadCount = 0;

				It.RemoveCurrent();
				continue;
			}
			break;
		}
	}
	else if (Delta > 0)
	{
		int Remain = FMath::Abs(Delta);

		for (auto It = NeedToRemove.CreateIterator(); It; ++It)
		{
			It->LoadCount -= Remain;

			if (It->LoadCount == 0)
			{
				It.RemoveCurrent();
				break;
			}
			else if (It->LoadCount < 0)
			{
				Remain = FMath::Abs(It->LoadCount);
				It->LoadCount = 0;

				It.RemoveCurrent();
				continue;
			}
			break;
		}
	}

	//SummToAdd = SummToRemove = 0;

	

	StoragesWithNeeding = NeedToAdd;
	StoragesWithExtra = NeedToRemove;


	OnHaveBeenRecalculate.Broadcast(this);
}

void USTResourceStorageController::OnChangedResourceStorageByPlayer(FStorageInfo StorageInfo)
{
	RecalculateData(StorageInfo.PrevLoadCount - StorageInfo.LoadCount);
}

int32 USTResourceStorageController::MaxUpgradeStorages(int32 CountForUpgrade)
{
	int32 NumOfIterations = ResourceStorages.Num();
	int32 Remain = 0;

	if (NumOfIterations - CountForUpgrade >= 0)
	{
		NumOfIterations = CountForUpgrade;
		Remain = 0;
	}else
	{
		Remain = CountForUpgrade - NumOfIterations;
	}

	int32 OldMaxCapacity = 0;
	int32 OldCountLoad = 0;
	
	for(int32 i = 0; i < NumOfIterations; ++i)
	{
		OldMaxCapacity = ResourceStorages[i]->GetMaxResourceCount();
		OldCountLoad = ResourceStorages[i]->GetCurrentResourceCount();
		ResourceStorages[i]->MaxUpgrade();
		TotalNumberOfResources = TotalNumberOfResources - OldCountLoad + 500;
		MaxNumberOfResources = MaxNumberOfResources - OldMaxCapacity + 500;
	}

	RecalculateData(0);
	
	return Remain;
}
