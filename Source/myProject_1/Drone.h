// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Missle.h"
#include "Drone.generated.h"

UCLASS()		//Ϊ������ģ���ļ�
class MYPROJECT_1_API ADrone : public APawn
{
	GENERATED_BODY()		//���ɴ����滻�����λ��

public:
	// Sets default values for this pawn's properties
	ADrone();

	//һ�����������
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* OutCollision;

	//����һ����̬�������		class:ǰ������ Forward  Declaration (Ҫ��ֻ˵����һ������Ĺ�����ǰ������ֻ����ָ�룬�����ñ���
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;
	//�ĸ��ɽ�
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Paddle1;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Paddle2;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Paddle3;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Paddle4;

	//�����������ǰ��������
	UPROPERTY(VisibleAnywhere)
		class UPhysicsThrusterComponent* UpThruster;
	UPROPERTY(VisibleAnywhere)
		class UPhysicsThrusterComponent* ForwardThruster;

	//����ȫ�Ǳ���
	//��ʼ��һ�����¼��ٶ�
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Init")		//��Ϊû�и�ָ�룬���Բ�����VisibleAnywhere
		float LiftAcc = 1000.0f;
	//���������ٶ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")		
		float LiftThrustMax = 2000.0f;
	//ǰ�����ٶ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Init")
		float ForwardAcc = 500.0f;
	//���������ٶ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
		float ForwardThrustMax = 2000.0f;
	//��ת����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
		float TurnStrength = 500000.0f;
	//�ɽ�����ת�ٶ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
		float  PaddleRotateSpeed = 2500.0f;
	//����һ����������ͼ���޸ĵı���
	UPROPERTY(EditAnywhere)
		TSubclassOf<AMissle> Bullet;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	//��������������������
	UFUNCTION()
		void Lift(float val);
	UFUNCTION()
		void  Forward(float val);
	UFUNCTION()
		void Turn(float val);
	UFUNCTION()
		void DoFire();

	TArray<UStaticMeshComponent*>Paddles;
	void RotatePaddle(float Delta);

};
