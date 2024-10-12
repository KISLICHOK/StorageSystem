// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameTypes/STCoreTypes.h"
#include "Subsystems/WorldSubsystem.h"
#include "STWorldLoadersSubsystem.generated.h"

/**
 * 
 */


class USTLoadersController;
class ASTLoader;

UCLASS(BlueprintType, Blueprintable)
class STORAGETEST_API USTWorldLoadersSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	
	UFUNCTION(BlueprintCallable)
	void StartGame();
	
protected:
	UPROPERTY()
	TArray<ASTLoader*> Loaders;
	
	UPROPERTY()
	TObjectPtr<USTLoadersController> LoadersController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default Loader Class")
	TSubclassOf<ASTLoader> DefaultLoaderClass;

public:
	UFUNCTION(BlueprintCallable)
	USTLoadersController* GetLoaderController() const {return LoadersController;}
	
	UFUNCTION(BlueprintCallable)
	void CreateDefaultLoader(const FTransform InTransform);

	UFUNCTION(BlueprintCallable)
	void AddPreparedLoader(ASTLoader * Loader);

	UFUNCTION(BlueprintCallable)
	bool HasLoaderByID(int32 InID);

	UFUNCTION(BlueprintCallable)
	ASTLoader* GetLoaderByID(int32 InID);


	FOnAddNewLoaderDelegate OnAddNewLoader;
	
};
