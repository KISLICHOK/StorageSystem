#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "STBuild.generated.h"

class USTWorldBuildSubsystem;
struct FBuildData;

UCLASS(Abstract, Blueprintable)
class STORAGETEST_API ASTBuild : public AActor
{
	GENERATED_BODY()
	
public:	
	ASTBuild();

	UFUNCTION(BlueprintCallable)
	int32 GetID() const { return ID; }

	UFUNCTION(BlueprintCallable)
	USTWorldBuildSubsystem* GetBuildSubsystem();

	UFUNCTION(BlueprintCallable)
	void SetBuildrColor(const FLinearColor& LinearColor);

	UFUNCTION(BlueprintCallable)
	void InitBuild(const FBuildData BuildData);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Storage")
	TObjectPtr<UStaticMeshComponent> MeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Storage")
	int32 ID;
	
	UFUNCTION(BlueprintCallable)
	bool Activate();

	UFUNCTION(BlueprintCallable)
	bool Deactivate();
	
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	TObjectPtr<USTWorldBuildSubsystem> WorldBuildSubsystem;
	
	int32 SetID(int32 NewID) { return ID = NewID; }
	

};
