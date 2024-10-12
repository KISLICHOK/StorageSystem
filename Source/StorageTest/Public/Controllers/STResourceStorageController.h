// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Builds/STStorageBuild.h"
#include "GameTypes/STCoreTypes.h"

#include "STResourceStorageController.generated.h"

/**
 * 
 */


UCLASS(BlueprintType)
class STORAGETEST_API USTResourceStorageController : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnHaveBeenRecalculatedDelegate OnHaveBeenRecalculate;

	TArray<FLoadOrder> StoragesWithExtra;
	TArray<FLoadOrder> StoragesWithNeeding;
	
	void Init(EResourceType ResourceType,const FLinearColor Color);


protected:
	UPROPERTY()
	TArray<ASTStorageBuild*> ResourceStorages;
	
	UPROPERTY()
	TMap<int32, ASTStorageBuild*> IDToStorage;

	UPROPERTY()
	TMap<ASTStorageBuild*, float> StoragesDeltaPercent;

	UPROPERTY()
	EResourceType ResourceType;

	UPROPERTY()
	FLinearColor ResourceColor;

private:
	int32 TotalNumberOfResources;
	int32 MaxNumberOfResources;

	double TotalLoadPercent;

public:
	UFUNCTION(BlueprintCallable)
	bool AddStorage(ASTStorageBuild* NewStorage);

	UFUNCTION(BlueprintCallable)
	void AddNotInitStorage(ASTStorageBuild* Storage);

	UFUNCTION(BlueprintCallable)
	void AddNotInitStorageWithParam(ASTStorageBuild* Storage, int32 Size = 0);

	int32 MaxUpgradeStorages(int32 CountForUpgrade);

protected:
	UFUNCTION()
	void OnChangedResourceStorageByPlayer(FStorageInfo StorageInfo);
	
	UFUNCTION(BlueprintCallable)
	void RecalculateDataWithNewStorage(const ASTStorageBuild* NewStorage);

	UFUNCTION(BlueprintCallable)
	void RecalculateData(const int32 Changes);

	UFUNCTION(BlueprintCallable)
	void RecalculateAllPercents();
	
};
