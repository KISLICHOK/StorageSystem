// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameTypes/STCoreTypes.h"
#include "GameTypes/STEnumTypes.h"
#include "Subsystems/WorldSubsystem.h"
#include "STWorldBuildSubsystem.generated.h"

class ASTStorageBuild;
class USTResourceStorageController;
class ASTBuild;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class STORAGETEST_API USTWorldBuildSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	UFUNCTION(BlueprintCallable)
	void StartGame();

	UFUNCTION(BlueprintCallable)
	void AddPreparedBuild(ASTBuild* Build); //When Game Start

	UFUNCTION(BlueprintCallable)
	void CreateNewStorage(TSubclassOf<ASTStorageBuild> BuildClass, const FTransform InTransform, EResourceType ResourceType, int32 StorageSize);

	TArray<USTResourceStorageController*> GetControllers() const;

	FOnWorldBuildSubsystemInitDelegate OnWorldBuildSubsystemInit;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Build Subsystem")
	TMap<EResourceType, USTResourceStorageController*> ResourceControllers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Build Subsystem")
	TArray<ASTBuild*> Builds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Settings")
	TObjectPtr<UStaticMesh> DefaultMesh;

	
public:
	UFUNCTION(BlueprintCallable)
	USTResourceStorageController* GetController(EResourceType Type) const;

	UFUNCTION(BlueprintCallable)
	void StressUpdateStorages();

private:
	UFUNCTION()
	virtual void CreateControllers();

	
	
};
