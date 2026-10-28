// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletBase.generated.h"

/**
 * @brief	�v���C���[���甭�˂���e
 *			�E����X�e�[�W�ł̎g�p
 *			�E���ˎ��ɔ��˕�������肵�A��]������
 */
UCLASS()
class BURNWITHPRIDE_API ABulletBase : public AActor
{
	GENERATED_BODY()
public:
	ABulletBase();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet|Mesh")
	class UStaticMeshComponent* StaticMeshComponent;

	// ���_����̋���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet|Movement")
	float TurningRadius = 1200.0f;

	// �p���x�i�x�^�b�j
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet|Movement")
	float OrbitSpeed = 90.0f;

	// �����ʒu�̃\�P�b�g��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet|Movement")
	FName PlayerSocketName = "BulletSocket";

	UPROPERTY(EditAnywhere, Category = "Bullet|Damage")
	float BulletDamage = 10.0f;

	UFUNCTION(BlueprintCallable, Category = "Bullet|Damage")
	void DealDamage(AActor* actor);

private:
	// ��]���i���[���h��ԁj
	FVector AxisDirectionWorld = FVector::UpVector;

	// ���_����̏����ʒu�i���ɒ����������� DistanceFromCenter�j
	FVector InitialWorldPos = FVector::ZeroVector;

	// ���݂̊p�x�i�x�j
	float CurrentAngle = 0.0f;

};
