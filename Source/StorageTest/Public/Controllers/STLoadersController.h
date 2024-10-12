// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameTypes/STCoreTypes.h"
#include "STLoadersController.generated.h"

class ASTBuild;
class USTWorldBuildSubsystem;
class USTWorldLoadersSubsystem;
enum class EResourceType : uint8;
class USTResourceStorageController;
class ASTLoader;


DEFINE_LOG_CATEGORY_STATIC(LogLoadersController, All, All);

using IterOrder = TIndexedContainerIterator<TArray<FLoadOrder>, TArray<FLoadOrder>::ElementType, TArray<FLoadOrder>::SizeType>;

/**
 * 
 */
UCLASS()
class STORAGETEST_API USTLoadersController : public UObject
{
	GENERATED_BODY()

public:
	USTLoadersController();
	
	UFUNCTION()
	void Init();


protected:
	//subystems ptr
	UPROPERTY()
	TObjectPtr<USTWorldLoadersSubsystem> LoaderSubsystem;

	UPROPERTY()
	TObjectPtr<USTWorldBuildSubsystem> BuildSubsystem;

private:
	UPROPERTY()
	TArray<ASTLoader*> AvailableLoaders;
	
	TSet<int32> StoragesIDInProgress;
	
	TArray<FLoadOrder> StorageToTakeResources;

	TMap<int32, FLoadOrder> IDToStorageBring;


private:
	UFUNCTION()
	void OnLoaderTake(ASTLoader* Loader, FLoadOrder Order);

	UFUNCTION()
	void OnLoaderBring(ASTLoader* Loader, FLoadOrder Order);
	
	UFUNCTION()
	void OnLoaderFinishWork(ASTLoader* Loader, FLoadOrder OldOrder);

	UFUNCTION()
	void OnAddLoader(ASTLoader* Loader);

	UFUNCTION()
	void OnRecalculateResource(USTResourceStorageController* RecalculatedController);
	
	void PrepareTransactions(const TArray<FLoadOrder>& ExtraLoad, const TArray<FLoadOrder>& NotExtraLoad);

	void ConsumeTakeOrder();

	void ConsumeNearestTakeOrder();
	
	bool TryConsumeTakeOrder(ASTLoader* Loader);
	
	void ConsumeBringOrder(ASTLoader* Loader, FLoadOrder OldOrder);

	//bool TryConsumeBringOrder(ASTLoader* Loader);
	
};
