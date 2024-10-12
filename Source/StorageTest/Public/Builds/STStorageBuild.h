
#pragma once

#include "CoreMinimal.h"
#include "STBuild.h"
#include "GameTypes/STCoreTypes.h"
#include "GameTypes/STEnumTypes.h"
#include "Interfaces/Bringable.h"

#include "STStorageBuild.generated.h"


class ASTLoader;


DEFINE_LOG_CATEGORY_STATIC(LogStorage, All, All );
/**
 * 
 */
UCLASS()
class STORAGETEST_API ASTStorageBuild : public ASTBuild
{
	GENERATED_BODY()
public:
	
	ASTStorageBuild();

	UFUNCTION(BlueprintCallable)
	void InitStorage(const FStorageBuildData BuildData);
	
	UPROPERTY(BlueprintAssignable)
	FOnCurrentResourcesChangedByPlayerDelegate OnCurrentResourcesChangedByPlayer;

	UFUNCTION(BlueprintCallable)
	void MaxUpgrade();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "Storage")
	TObjectPtr<USceneComponent> BringLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "Storage")
	TObjectPtr<USceneComponent> PickLocation;
	
	UPROPERTY(EditAnywhere, Category = "Storage" , meta=(ClampMin="1", ClampMax="1000"))
	int32 MaxResourceCount = 0;

	UPROPERTY(EditAnywhere, Category = "Storage")
	EResourceType ResourceType;

	UPROPERTY(EditAnywhere, Category = "Storage")
	int32 CurrentResourceCount = 0;

	UPROPERTY(EditAnywhere, Category = "Storage")
	int32 PendingResource = 0;

	UPROPERTY(VisibleAnywhere, Category = "Storage")
	bool bIsAwaitDeliver = 0;

	UPROPERTY(VisibleAnywhere, Category = "Storage")
	TArray<ASTLoader*> ComingLoaders;

	UPROPERTY(EditAnywhere)
	TArray<ASTStorageBuild*> SameTypeStorages; //first nearest, last farest

public:
	
	UFUNCTION(BlueprintCallable)
	void AddResource(int32 AddingResources);

	UFUNCTION(BlueprintCallable)
	float GetPercent() const;

	UFUNCTION(BlueprintCallable)
	EResourceType GetResourceType() const {return ResourceType;}
	
	UFUNCTION(BlueprintCallable)
	int32 GetMaxResourceCount() const {return MaxResourceCount; }

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentResourceCount() const {return CurrentResourceCount; }

	UFUNCTION(BlueprintCallable)
	FStorageUIInfo GetUIInfo() const;
	

	UFUNCTION(BlueprintCallable)
	const TArray<ASTStorageBuild*>& GetSameStorages() const {return SameTypeStorages; }
	
	UFUNCTION(BlueprintCallable)
	void SetSameStorages(const TArray<ASTStorageBuild*>& SameStorages);
	
	UFUNCTION(BlueprintCallable)
	void AddNewStorage(ASTStorageBuild* InStorage);
	
	UFUNCTION(BlueprintCallable)
	void PrepareToComeLoader(ASTLoader* InLoader, FLoadOrder Order);

	UPROPERTY(BlueprintAssignable)
	FOnChangeUIInfoDelegate OnChangeUIInfo;

private:
	TMap<int32, FDelegateHandle> LoaderComingDelegateHandles;

	FStorageUIInfo UIInfo;

	void SetUIInfo();
	
	void ResourceCountWasChanged(int32 ResourceCount, int ResourceCountDelta) const;
	
	void RecalculateStorageDistance();

	UFUNCTION()
	void OnLoaderToCome(ASTLoader* Loader, FLoadOrder Order);

	
	
};
