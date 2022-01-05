// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Missle.h"
#include "Drone.generated.h"

UCLASS()		//为类生成模板文件
class MYPROJECT_1_API ADrone : public APawn
{
	GENERATED_BODY()		//生成代码替换到这个位置

public:
	// Sets default values for this pawn's properties
	ADrone();

	//一个包裹框组件
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* OutCollision;

	//声明一个静态网格组件		class:前向声明 Forward  Declaration (要是只说中文一定会打错的哈哈）前置声明只能用指针，不能用变量
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;
	//四个飞桨
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Paddle1;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Paddle2;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Paddle3;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Paddle4;

	//添加上升和向前推理的组件
	UPROPERTY(VisibleAnywhere)
		class UPhysicsThrusterComponent* UpThruster;
	UPROPERTY(VisibleAnywhere)
		class UPhysicsThrusterComponent* ForwardThruster;

	//下面全是变量
	//初始化一个上下加速度
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Init")		//因为没有给指针，所以不能用VisibleAnywhere
		float LiftAcc = 1000.0f;
	//上下最大加速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")		
		float LiftThrustMax = 2000.0f;
	//前进加速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Init")
		float ForwardAcc = 500.0f;
	//上下最大加速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
		float ForwardThrustMax = 2000.0f;
	//旋转的力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
		float TurnStrength = 500000.0f;
	//飞桨的旋转速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
		float  PaddleRotateSpeed = 2500.0f;
	//引入一个可以在蓝图类修改的变量
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

	//声明方法，用来绑定输入
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
