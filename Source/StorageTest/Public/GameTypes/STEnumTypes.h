// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STEnumTypes.generated.h"

UENUM(BlueprintType, Blueprintable)
enum class EResourceType : uint8
{
	ResourceType1,
	ResourceType2,
	ResourceType3,
	ResourceType5,
	ResourceType6,
	ResourceType7,
	ResourceType8,
	ResourceType9,
	ResourceType10,
	ResourceType11,
	ResourceType12,
	ResourceType13,
	ResourceType14,
	ResourceType15,
	ResourceType16,
	ResourceType17,
	ResourceType18,
	ResourceType19,
	ResourceType20,
	ResourceType21,
	ResourceType22,
	ResourceType23,
	ResourceType24,
	ResourceType25,
	ResourceType26,
	ResourceType27,
	ResourceType4 ,
	ResourceType28,
	ResourceType29,
	ResourceType30,
	ResourceType31,
	ResourceType32,
	ResourceType33,
	ResourceType34,
	ResourceType35,
	ResourceType36,
	ResourceType37,
	ResourceType38,
	ResourceType39,
	ResourceType40,
	ResourceType41,
	ResourceType42,
	ResourceType43,
	ResourceType44,
	ResourceType45,
	ResourceType46,
	ResourceType47,
	ResourceType48,
	ResourceType49,
	ResourceType50,
	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EResourceType, EResourceType::Count);

UENUM(BlueprintType, Blueprintable)
enum class ELoaderState : uint8
{
	IdleState,
	GoingTakeLoadState,
	GoingBringLoadState,
	Count
};
ENUM_RANGE_BY_COUNT(ELoaderState, ELoaderState::Count);