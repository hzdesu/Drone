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
	//��gamemode��������һ������  ����
	UPROPERTY()
		uint32 points;
};
