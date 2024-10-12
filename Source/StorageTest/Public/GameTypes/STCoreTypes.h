#pragma once


#include "CoreMinimal.h"
#include "STEnumTypes.h"

#include "STCoreTypes.generated.h"


enum class EResourceType : uint8;
class ASTStorageBuild;

DECLARE_DELEGATE(FOnWorldBuildSubsystemInitDelegate);

USTRUCT(BlueprintType)
struct FBuildData
{
	GENERATED_BODY()

	UPROPERTY()
	UStaticMesh* StaticMesh;

	int32 ID;
};

USTRUCT(BlueprintType)
struct FStorageBuildData
{
	GENERATED_BODY()
	
	int32 MaxResourceCount;
	
	EResourceType ResourceType;
	
};


class USTResourceStorageController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHaveBeenRecalculatedDelegate, USTResourceStorageController*, RecalculatedController);

USTRUCT(BlueprintType, Blueprintable)
struct FLoadOrder
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	ASTStorageBuild* Storage;

	UPROPERTY(VisibleAnywhere)
	int32 LoadCount;

	UPROPERTY(VisibleAnywhere)
	EResourceType ResourceType;

	UPROPERTY(VisibleAnywhere)
	uint8 bIsTakeOrder : 1;
	
	operator bool () const {return Storage != nullptr;}
	
	bool operator==(const FLoadOrder& Other) const {return Storage == Other.Storage && LoadCount == Other.LoadCount;}
};

USTRUCT(BlueprintType, Blueprintable)
struct FStorageUIInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int32 StorageID;
	
	UPROPERTY(BlueprintReadWrite)
	int32 MaxResourceCount;

	UPROPERTY(BlueprintReadWrite)
	EResourceType ResourceType;

	UPROPERTY(BlueprintReadWrite)
	int32 LoadCount;

	
};

USTRUCT(BlueprintType, Blueprintable)
struct FStorageInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int32 StorageID;
	
	UPROPERTY(BlueprintReadWrite)
	int32 MaxResourceCount;

	UPROPERTY(BlueprintReadWrite)
	EResourceType ResourceType;

	UPROPERTY(BlueprintReadWrite)
	int32 LoadCount;

	UPROPERTY(BlueprintReadWrite)
	int32 PrevLoadCount;
};

class ASTLoader;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAddNewLoaderDelegate, ASTLoader*);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentResourcesChangedByPlayerDelegate, FStorageInfo, StorageInfo);


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTakeLoadDelegate, ASTLoader*, Loader, FLoadOrder, Order);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBringLoadDelegate, ASTLoader*, Loader, FLoadOrder, Order);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnComeToStorageDelegate, ASTLoader*, Loader, FLoadOrder, Order);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFinishWorkDelegate, ASTLoader*, Loader, FLoadOrder, Order);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnFinishWorkDelegate, ASTLoader*,FLoadOrder);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeUIInfoDelegate, FStorageUIInfo, NewUIInfo);