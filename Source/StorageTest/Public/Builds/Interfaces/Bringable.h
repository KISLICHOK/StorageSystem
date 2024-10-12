// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Bringable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBringable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STORAGETEST_API IBringable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetPointToBringLoad();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetPointToPickLoad();
};
