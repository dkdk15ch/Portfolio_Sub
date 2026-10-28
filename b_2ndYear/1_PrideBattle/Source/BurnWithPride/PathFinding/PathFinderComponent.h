// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PathGraphData.h"
#include "PathFinderComponent.generated.h"

/**
 * @brief	�o�H�T�����s���̒T���m�[�h
 *			�T���m�[�h�����K�v
 */
USTRUCT()
struct FPathNodeRuntimeData
{
	GENERATED_BODY();

	uint8 ClosedFlags;   // 0: ���A1: �R�X�g�v�Z�ς�
	uint8 InOpenFlags;   // 0: ���A1: �T�����ɒǉ��ς�
	int32 Parent;        // parent vertex index 
	float GCost;         // g cost
	float HCost;         // h cost (optional �L���b�V��)  
	float FCost;         // f cost (G + H)

	void Reset() {
		ClosedFlags = 0;
		InOpenFlags = 0;
		Parent = INDEX_NONE;
		GCost = FLT_MAX;
		HCost = 0.0f;
		FCost = FLT_MAX;
	}

	float CalcFCost() const{ return GCost + HCost; }
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BURNWITHPRIDE_API UPathFinderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPathFinderComponent();

	// 経路データ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
	UPathGraphData* m_GraphData = nullptr;

	// �T������
	UPROPERTY(BlueprintReadOnly, Category = "Pathfinding")
	TArray<FVector> m_ResultPath;

	// ���ɕԂ��C���f�b�N�X�iGetNextMove �p�j
	UPROPERTY(BlueprintReadOnly, Category = "Pathfinding")
	int32 NextPathIndex = 0;

	UFUNCTION(BlueprintCallable, Category = "Pathfinding")
	bool FindPath(const FVector& StartPos, const FVector& GoalPos);

	// ���̈ړ����Ԃ��i���݂��Ȃ���� false�j
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Pathfinding")
	FVector GetNextMovePos();

	//	�p�X�N���A
	UFUNCTION(BlueprintCallable, Category = "Pathfinding")
	void ClearResultPath();

	//	�S�[����O�̌o�H���ǂ�����`�F�b�N
	UFUNCTION(BlueprintCallable, Category = "Pathfinding")
	bool CheckLastPath()const { return m_bLastPath; }

	//	�����_���ȒT���m�[�h�̈ʒu��擾
	UFUNCTION(BlueprintCallable, Category = "Pathfinding")
	FVector GetRandomLocation();

	//	�ő�T����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
	int32 MaxIterations = 7000;

protected:
	// �����^�C���z��i�T�����Ƃɏ������j
	TArray<uint8> ClosedFlags;   // 0: ���A1: Closed
	TArray<uint8> InOpenFlags;   // 0: ���A1: In open set
	TArray<int32> Parent;        // parent vertex index (INDEX_NONE = none)
	TArray<float> GCost;         // g cost
	TArray<float> HCost;         // h cost (optional �L���b�V��)
	TArray<float> FCost;         // f cost (G + H)

	TArray<FPathNodeRuntimeData> Nodes;

	//	���݂̒T�����
	//	�V�����m�[�h�ɓ��B���ɁA�אڃm�[�h�����T����������OpenSet�ɒǉ�
	TArray<int32> OpenSet;

	// ������[�e�B���e�B
	int32 GetNearestVertexIndex(const FVector& Pos) const;
	float HeuristicCostByIndex(int32 AIndex, int32 BIndex) const;
	float CostBetweenIndices(int32 AIndex, int32 BIndex) const;
	int32 PopLowestF();
	void MakeResultPath(int32 GoalIndex);

private:
	bool m_bLastPath = false;
};
