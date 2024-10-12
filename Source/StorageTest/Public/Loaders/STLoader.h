// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameTypes/STCoreTypes.h"
#include "GameTypes/STEnumTypes.h"

#include "STLoader.generated.h"


enum class ELoaderState : uint8;

class USTLoadersController;
class ASTBuild;
class IBringable;
class UFloatingPawnMovement;

DEFINE_LOG_CATEGORY_STATIC(LogLoaderCategoty, All, All);


UCLASS()
class STORAGETEST_API ASTLoader : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASTLoader();

	UFUNCTION(BlueprintCallable)
	int32 GetID() const { return ID; }

	UFUNCTION(BlueprintCallable)
	void SetID(int32 InID) { ID = InID; };

protected:
	UPROPERTY()
	TObjectPtr<USTLoadersController> LoadersController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UFloatingPawnMovement> FloatingPawnMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MaxCapacity = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ID;

	UPROPERTY(VisibleAnywhere)
	FLoadOrder CurrentOrder;

	UPROPERTY(VisibleAnywhere)
	ELoaderState LoaderState = ELoaderState::IdleState;
	

	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	int32 GetMaxCapacity() const { return MaxCapacity; }
	
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentCount() const { return CurrentCount; }

	UFUNCTION(BlueprintCallable)
	FLoadOrder GetCurrentLoadOrder() const { return CurrentOrder; }

	UFUNCTION(BlueprintCallable)
	bool IsEmpty() const {return CurrentCount == 0;}

	UFUNCTION(BlueprintCallable)
	void SetOrder(FLoadOrder InOrder);
	
	UFUNCTION(BlueprintCallable)
	virtual void ReachedDestination();

	UFUNCTION(BlueprintCallable)
	void ResetLoader();

	UFUNCTION(BlueprintCallable)
	FVector GetTargetLocation();

	UFUNCTION(BlueprintCallable)
	ASTBuild* GetTargetBuild() const;

	
	FOnFinishWorkDelegate OnFinishWork;

	UPROPERTY(BlueprintAssignable)
	FOnTakeLoadDelegate OnStartTakeLoad;

	UPROPERTY(BlueprintAssignable)
	FOnBringLoadDelegate OnStartBringLoad;

private:
	int32 CurrentCount;

	void SwitchLoaderState(ELoaderState NextLoaderState);
	void SetLoaderState(ELoaderState NewLoaderState);

	void IntoTakeLoadState();
	void IntoBringLoadState();
	void IntoFinishWorkState();
};
