// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCBase.h"

// 初期値を設定します
ANPCBase::ANPCBase()
{
 	// このキャラクターのTickを毎フレーム呼び出す設定です。不要な場合は無効にして性能を改善できます。
	PrimaryActorTick.bCanEverTick = false;

}

// ゲーム開始時または生成時に呼び出されます
void ANPCBase::BeginPlay()
{
	Super::BeginPlay();
}

// 毎フレーム呼び出されます
void ANPCBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 入力に機能を割り当てるために呼び出されます
void ANPCBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPCBase::ApplyDamage(float Damage, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse)
{
	DamageHP(Damage);
}
