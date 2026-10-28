// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "CombatDamageable.h"
#include "Math/UnrealMathUtility.h"

ABulletBase::ABulletBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABulletBase::BeginPlay()
{
		Super::BeginPlay();

	//	�������̏����ݒ�
	//	StaticMeshComponent ���Z�b�g����Ă��邱�Ƃ�m�F
	if (!StaticMeshComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABulletBase::BeginPlay__StaticMeshComponent is not set."));
		return;
	}

	// ���ˎ��ɉ�]���̌��������
	// �v���C���[�̉�]����g�p
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (Player)
	{
		AxisDirectionWorld = Player->GetActorRightVector();
		AxisDirectionWorld.Normalize();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ABulletBase::BeginPlay__no Player"))
	}

	// �����ʒu��Z�b�g
	USkeletalMeshComponent* Skel = Player->FindComponentByClass<USkeletalMeshComponent>();
	if (Skel && Skel->DoesSocketExist(PlayerSocketName))
	{
		const FVector SocketLoc = Skel->GetSocketLocation(PlayerSocketName); 
		FVector V = SocketLoc;
		V.Normalize();
		InitialWorldPos = V * TurningRadius;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ABulletBase::BeginPlay__PlayerSocket is not found"));
	}

	// �����z�u�i�C�Ӂj
	StaticMeshComponent->SetWorldLocation(InitialWorldPos);

	CurrentAngle = 0.0f;
}

/**
 * @brief	�e�����������A�N�^�Ƀ_���[�W��^����
 */
void ABulletBase::DealDamage(AActor* actor)
{
	ICombatDamageable* Damageable = Cast<ICombatDamageable>(actor);
	FVector Impulselocation = FVector(0, 0, 0);

	Damageable->ApplyDamage(BulletDamage, this, this->GetActorLocation(), Impulselocation);
}

void ABulletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// �p�x�X�V
	CurrentAngle += OrbitSpeed * DeltaTime;
	CurrentAngle = FMath::Fmod(CurrentAngle, 360.0f);

	// ���_���S�E�� AxisDirectionWorld ����ŉ�]�iRotateAngleAxis �͓x�P�ʁj
	FVector Rotated = InitialWorldPos.RotateAngleAxis(CurrentAngle, AxisDirectionWorld);

	// ���b�V���̃��[���h�ʒu��X�V
	StaticMeshComponent->SetWorldLocation(Rotated);
}


