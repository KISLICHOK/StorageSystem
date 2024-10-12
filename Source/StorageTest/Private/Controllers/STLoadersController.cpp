// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/STLoadersController.h"


#include "Controllers/STResourceStorageController.h"
#include "Loaders/STLoader.h"
#include "Subsystems/STWorldBuildSubsystem.h"
#include "Subsystems/STWorldLoadersSubsystem.h"


USTLoadersController::USTLoadersController()
{
}

void USTLoadersController::Init()
{
	if (!GetWorld())
	{
		checkf(LoaderSubsystem, TEXT("Loader Controller not GetWorld"));
		return;
	}

	LoaderSubsystem = GetWorld()->GetSubsystem<USTWorldLoadersSubsystem>();
	checkf(LoaderSubsystem, TEXT("Loader Subsystem not create"));

	LoaderSubsystem->OnAddNewLoader.AddUObject(this, &ThisClass::OnAddLoader);

	BuildSubsystem = GetWorld()->GetSubsystem<USTWorldBuildSubsystem>();
	checkf(LoaderSubsystem, TEXT("Build Subsystem not create"));

	for (USTResourceStorageController* ResourceController : BuildSubsystem->GetControllers())
	{
		ResourceController->OnHaveBeenRecalculate.AddDynamic(this, &ThisClass::OnRecalculateResource);
	}
}


void USTLoadersController::PrepareTransactions(const TArray<FLoadOrder>& ExtraLoad, const TArray<FLoadOrder>& NotExtraLoad)
{
	if (ExtraLoad.IsEmpty() || NotExtraLoad.IsEmpty()) return;
	const auto ResourceType = ExtraLoad[0].ResourceType;

	StorageToTakeResources.RemoveAll([ResourceType](const FLoadOrder& Order) { return Order.ResourceType == ResourceType; });


	for (const auto& WithoutExtra : NotExtraLoad)
	{
		IDToStorageBring.Remove(WithoutExtra.Storage->GetID());
	}

	StorageToTakeResources.Append(ExtraLoad);

	for (const auto& WithoutExtra : NotExtraLoad)
	{
		IDToStorageBring.Add(WithoutExtra.Storage->GetID(), WithoutExtra);
	}

	ConsumeNearestTakeOrder();
}

void USTLoadersController::ConsumeTakeOrder()
{
	if (AvailableLoaders.IsEmpty() || StorageToTakeResources.IsEmpty()) return;

	for (auto It = AvailableLoaders.CreateIterator(); It; ++It)
	{
		if (!TryConsumeTakeOrder(*It)) return;

		It.RemoveCurrent();
	}

	
}

void USTLoadersController::ConsumeNearestTakeOrder()
{
	if (AvailableLoaders.IsEmpty() || StorageToTakeResources.IsEmpty()) return;

	int32 NearestIndexLoader = 0;
	int32 NearestIndexTakeOrder = 0;

	double MinDistance = DBL_MAX;
	
	for(int i = 0; i < AvailableLoaders.Num(); ++i)
	{
		for(int j = 0; j < StorageToTakeResources.Num(); ++j)
		{
			double DistanceSquared = AvailableLoaders[i]->GetSquaredDistanceTo(StorageToTakeResources[j].Storage);
			
			if(DistanceSquared < MinDistance)
			{
				NearestIndexLoader = i;
				NearestIndexTakeOrder = j;
				MinDistance = DistanceSquared;
			}
		}
	}

	if(MinDistance == DBL_MAX) {return;}

	auto Loader = AvailableLoaders[NearestIndexLoader];
	auto TakeOrder = StorageToTakeResources[NearestIndexTakeOrder];
	
	if(TakeOrder.LoadCount <= Loader->GetMaxCapacity())
	{
		StorageToTakeResources.RemoveAt(NearestIndexTakeOrder);
	}
	else
	{
		TakeOrder.LoadCount = Loader->GetMaxCapacity();
		StorageToTakeResources[NearestIndexTakeOrder].LoadCount -= TakeOrder.LoadCount;
	}

	AvailableLoaders.RemoveAt(NearestIndexLoader);

	Loader->Reset();
	
	/*if (Loader->OnFinishWork.IsAlreadyBound(this, &ThisClass::OnLoaderFinishWork))
	{
		checkf(false, TEXT("Alredy Bound"));
	}*/
	Loader->OnFinishWork.AddUObject(this, &ThisClass::OnLoaderFinishWork);
	Loader->SetOrder(TakeOrder);
	
}

bool USTLoadersController::TryConsumeTakeOrder(ASTLoader* Loader)
{
	if (!IsValid(Loader)) return false;

	auto LoaderLocation = Loader->GetActorLocation();
	double MinDistance = DBL_MAX;
	FLoadOrder NearestOrder;
	NearestOrder.Storage = nullptr;

	int32 OrderIndex = 0;

	for (int32 i = 0; i < StorageToTakeResources.Num(); ++i)
	{
		auto SquareDistance = FVector::DistSquared(LoaderLocation, StorageToTakeResources[i].Storage->GetActorLocation());

		if (SquareDistance < MinDistance)
		{
			NearestOrder = StorageToTakeResources[i];
			MinDistance = SquareDistance;
			OrderIndex = i;
		}
	}

	if (NearestOrder.Storage == nullptr) { return false; }

	Loader->ResetLoader();

	FLoadOrder OrderToConsume(NearestOrder.Storage, -1, NearestOrder.ResourceType, true);

	if (Loader->GetMaxCapacity() < NearestOrder.LoadCount)
	{
		OrderToConsume.LoadCount = Loader->GetMaxCapacity();
		int32 Remain = StorageToTakeResources[OrderIndex].LoadCount -= OrderToConsume.LoadCount;

		if (Remain == 0)
		{
			StorageToTakeResources.RemoveAt(OrderIndex);
		}
	}
	else
	{
		OrderToConsume.LoadCount = NearestOrder.LoadCount;
		StorageToTakeResources.RemoveAt(OrderIndex);
	}

	Loader->SetOrder(OrderToConsume);
	/*if (Loader->OnFinishWork.IsAlreadyBound(this, &ThisClass::OnLoaderFinishWork))
	{
		checkf(false, TEXT("Alredy Bound"));
	}*/
	Loader->OnFinishWork.AddUObject(this, &ThisClass::OnLoaderFinishWork);
	return true;
}



void USTLoadersController::ConsumeBringOrder(ASTLoader* Loader, FLoadOrder OldOrder)
{
	auto Storages = OldOrder.Storage->GetSameStorages();

	FLoadOrder NearestBringOrder;
	NearestBringOrder.Storage = nullptr;

	for (auto Storage : Storages)
	{
		if (!IDToStorageBring.Contains(Storage->GetID())) continue;

		auto Oreder = IDToStorageBring[Storage->GetID()];
		NearestBringOrder = Oreder;
		break;
	}

	if (NearestBringOrder.Storage == nullptr)
	{
		Loader->Reset();
		AvailableLoaders.Add(Loader);
		return;
	}

	if(NearestBringOrder.Storage == nullptr)
	{
		checkf(false, TEXT("NearestBringOrder.Storage nullptr"));
	}

	//UE_LOG(LogLoadersController, Display, TEXT("%p"), NearestBringOrder.Storage);

	if (NearestBringOrder.LoadCount <= Loader->GetCurrentCount())
	{
		IDToStorageBring.Remove(NearestBringOrder.Storage->GetID());
	}
	else
	{
		IDToStorageBring[NearestBringOrder.Storage->GetID()].LoadCount -= Loader->GetCurrentCount();
		NearestBringOrder.LoadCount = Loader->GetCurrentCount();

		if (IDToStorageBring[NearestBringOrder.Storage->GetID()].LoadCount == 0)
		{
			IDToStorageBring.Remove(NearestBringOrder.Storage->GetID());
		}
	}


	NearestBringOrder.bIsTakeOrder = false;


	//Loader->OnFinishWork.AddUObject(this, &ThisClass::OnLoaderFinishWork);
	Loader->SetOrder(NearestBringOrder);
}


void USTLoadersController::OnRecalculateResource(USTResourceStorageController* RecalculatedController)
{
	if (!IsValid(RecalculatedController)) return;

	PrepareTransactions(RecalculatedController->StoragesWithExtra, RecalculatedController->StoragesWithNeeding);

	UE_LOG(LogLoadersController, Warning, TEXT("Loader Controller Recalculate"));
}


void USTLoadersController::OnAddLoader(ASTLoader* Loader)
{
	if (!IsValid(Loader) || AvailableLoaders.Contains(Loader)) return;
	AvailableLoaders.Add(Loader);

	ConsumeNearestTakeOrder();
	UE_LOG(LogLoadersController, Warning, TEXT("Loader Controller OnAddLoader"));
}


void USTLoadersController::OnLoaderTake(ASTLoader* Loader, FLoadOrder Order)
{
}

void USTLoadersController::OnLoaderBring(ASTLoader* Loader, FLoadOrder Order)
{
}

void USTLoadersController::OnLoaderFinishWork(ASTLoader* Loader, FLoadOrder OldOrder)
{
	if (!Loader->IsEmpty())
	{
		ConsumeBringOrder(Loader, OldOrder);
		return;
	}

	Loader->ResetLoader();
	AvailableLoaders.Add(Loader);
	ConsumeNearestTakeOrder();
}
