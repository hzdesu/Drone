// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

UCLASS()
class MYPROJECT_1_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh;

	//��������ϵͳ
	//UPROPERTY(EditAnywhere)
	//	class UParticleSystem* Explosion;

	//�������
	UPROPERTY(EditAnywhere)
		class USoundBase* ExplosionSound;

	UPROPERTY(EditAnywhere)
		float floatSpeed = 100.0f;

	//һ�������ƻ��ĺ���
	UFUNCTION()
		void DestroyHandler(AActor* DestroyedActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
