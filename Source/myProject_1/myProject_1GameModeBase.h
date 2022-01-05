// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "myProject_1GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_1_API AmyProject_1GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	//在gamemode里面声明一个变量  分数
	UPROPERTY()
		uint32 points;
};
