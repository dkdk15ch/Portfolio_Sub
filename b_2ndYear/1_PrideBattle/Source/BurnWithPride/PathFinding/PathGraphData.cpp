// Fill out your copyright notice in the Description page of Project Settings.


#include "PathGraphData.h"

/**
 * @brief			�ۑ��ς݊m�F���āA�אڃf�[�^��ǉ�
 * @param Vertices  ���_�f�[�^
 * @param FromIndex �n�_�C���f�b�N�X
 * @param ToIndex	�I�_�C���f�b�N�X
 */
void FNeighbor::AddNeighborIfNotExists(TArray<FVertex>& Vertices, int32 FromIndex, int32 ToIndex)
{
	{
		if (!Vertices.IsValidIndex(FromIndex) || !Vertices.IsValidIndex(ToIndex)) return;
		auto& Neis = Vertices[FromIndex].Neighbors;
		for (const FNeighbor& n : Neis)
		{
			if (n.VertexIndex == ToIndex) return;
		}
		float Dist = FVector::Dist(Vertices[FromIndex].Position, Vertices[ToIndex].Position);

		Neis.Add(FNeighbor(ToIndex, Dist));
	}
}

