  // Fill out your copyright notice in the Description page of Project Settings.


#include "PathFinderComponent.h"

UPathFinderComponent::UPathFinderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UPathFinderComponent::FindPath(const FVector& StartPos, const FVector& GoalPos)
{
	if (!m_GraphData)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPathFinderComponent::FindPath__Failde GraphData is null"));
		return false;
	}
	
	//	GraphData�̒��_���̔z���m�ۂ��āA������
	const int32 NodeNum = m_GraphData->m_Vertices.Num();
	if (NodeNum == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPathFinderComponent::FindPath__Failde no vertices"));
		return false;
	}
	Nodes.SetNum(NodeNum);
	for (FPathNodeRuntimeData& node : Nodes)
	{
		node.Reset();
	}
	OpenSet.Empty();
	m_ResultPath.Empty();
	NextPathIndex = 0;
	m_bLastPath = false;

	//	Start, Goal �̋ߗ׃m�[�h��T��
	int32 StartIndex = GetNearestVertexIndex(StartPos);
	int32 GoalIndex = GetNearestVertexIndex(GoalPos);

	// ������
	Nodes[StartIndex].HCost			= HeuristicCostByIndex(StartIndex, GoalIndex);
	Nodes[StartIndex].GCost			= 0.0f;
	Nodes[StartIndex].FCost			= Nodes[StartIndex].CalcFCost();
	Nodes[StartIndex].InOpenFlags	= true;
	OpenSet.Add(StartIndex);

	int32 Iter = 0;
	while (OpenSet.Num() > 0 && Iter++ < MaxIterations)
	{
		int32 Current = PopLowestF();
		if (Current == INDEX_NONE) break;

		//	Goal�`�F�b�N
		if (Current == GoalIndex)
		{		
			//	Goal���ɁA�ŏI�I�Ȍo�H��쐬
			MakeResultPath(GoalIndex);

			return true;
		}

		Nodes[Current].ClosedFlags = true;

		//	�אڃm�[�h�𑖍�
		const FVertex& CurrentVert = m_GraphData->m_Vertices[Current];
		for (const FNeighbor& neigh : CurrentVert.Neighbors)
		{
			int32 NeighIndex = neigh.VertexIndex;
			if (NeighIndex == INDEX_NONE )			continue;
			if (!Nodes.IsValidIndex(NeighIndex))	continue;
			if (Nodes[NeighIndex].ClosedFlags)		continue;

			//	���v�ړ��R�X�g
			float TotalG = Nodes[Current].GCost + neigh.Cost;

			UE_LOG(LogTemp, Log, TEXT("UPathFinderComponent::FindPath__Check Neighbor=%3d from Current=%3d: PrevG=%.2f NewG=%.2f F=%.2f [Parent=%d]"),
				NeighIndex,
				Current,
				Nodes[NeighIndex].GCost,
				TotalG,
				Nodes[NeighIndex].FCost,
				Nodes[NeighIndex].Parent);

			//	�����B�̃m�[�h or �O������R�X�g�@�̏ꍇ�@
			//	���s���̒T���m�[�h��X�V
			if (TotalG >= Nodes[NeighIndex].GCost) continue;

			UE_LOG(LogTemp, Log, TEXT("UPathFinderComponent::FindPath__Update Neighbor=%3d Node: Set Parent=%3d G=%.2f H=%.2f F=%.2f"),
				NeighIndex,
				Current,
				TotalG,
				HeuristicCostByIndex(NeighIndex, GoalIndex),
				TotalG + HeuristicCostByIndex(NeighIndex, GoalIndex));
			
			Nodes[NeighIndex].Parent	= Current;
			Nodes[NeighIndex].GCost		= TotalG;
			Nodes[NeighIndex].HCost		= HeuristicCostByIndex(NeighIndex, GoalIndex);
			Nodes[NeighIndex].FCost		= Nodes[NeighIndex].CalcFCost();
			if (!Nodes[NeighIndex].InOpenFlags)
			{
				OpenSet.Add(NeighIndex);
				Nodes[NeighIndex].InOpenFlags = true;
			}
		}
	}

	// �T�����s
	UE_LOG(LogTemp, Warning, TEXT("UPathFinderComponent::FindPath__path not found (iter=%d)"), Iter);
	return false;

}

//	���̈ړ��ꏊ��擾
FVector UPathFinderComponent::GetNextMovePos()
{
	if (m_ResultPath.Num()==0)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPathFinderComponent::GetNextMove__Failde m_ResultPath = 0"));
		return FVector(0,0,0);
	}
	if (NextPathIndex >= m_ResultPath.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("UPathFinderComponent::GetNextMove__Failde NextPathIndex >= m_ResultPath"));
		return FVector(0,0,0);
	}

	FVector NextPos = m_ResultPath[NextPathIndex++];
	if (NextPathIndex >= m_ResultPath.Num())
	{
		m_bLastPath = true;
	}
	
	return NextPos;
}

/**
 * @brief	Pos����ł�߂��m�[�h��������āA�C���f�b�N�X�ŕԂ�
 */
int32 UPathFinderComponent::GetNearestVertexIndex(const FVector& Pos) const
{
	if (!m_GraphData)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPathFinderComponent::GetNearestVertexIndex__Failde"));
		return INDEX_NONE;
	}

	int32 Best = INDEX_NONE;
	float BestDist2 = FLT_MAX;
	const int32 N = m_GraphData->m_Vertices.Num();
	for (int32 i = 0; i < N; ++i)
	{
		float d2 = FVector::DistSquared(Pos, m_GraphData->m_Vertices[i].Position);
		if (d2 < BestDist2)
		{
			BestDist2 = d2;
			Best = i;
		}
	}
	return Best;
}

/**
 * @brief			�����ŗ^�����m�[�h�Ԃ̃q���[���X�e�B�b�N(����)������擾
 * @param AIndex	�X�^�[�g�̃C���f�b�N�X
 * @param BIndex	�S�[���̃C���f�b�N�X(�ŏI�I�ȃS�[��)
 * @return			h�R�X�g 
 */
float UPathFinderComponent::HeuristicCostByIndex(int32 AIndex, int32 BIndex) const
{
	if (!m_GraphData) return 0.f;
	return FVector::Dist(m_GraphData->m_Vertices[AIndex].Position, m_GraphData->m_Vertices[BIndex].Position);
}

/**
 * @brief			�����ŗ^�����m�[�h�Ԃ̈ړ��R�X�g��v�Z
 * @param AIndex	�X�^�[�g�̃C���f�b�N�X	
 * @param BIndex	�S�[���̃C���f�b�N�X
 * @return			g�R�X�g
 */
float UPathFinderComponent::CostBetweenIndices(int32 AIndex, int32 BIndex) const
{
	//	GraphData��Neighbors�ɃR�X�g��ێ����Ă���̂ŁA�����g�p����B
	if (!m_GraphData) return FLT_MAX;
	const FVertex& V = m_GraphData->m_Vertices[AIndex];
	for (const FNeighbor& nb : V.Neighbors)
	{
		if (nb.VertexIndex == BIndex)
		{
			return nb.Cost;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("UPathFinderComponent::CostBetweenIndices__Failde"));
	return FLT_MAX;
}


/**
 * @brief	OpenSet�̒��ŁAF�R�X�g���ł�������m�[�h����o��
 * @return 
 */
int32 UPathFinderComponent::PopLowestF()
{
	//�@�ŏ��R�X�g���r���Ď��o��
	int32 BestIdxInOpen = INDEX_NONE;
	float BestF = FLT_MAX;
	for (int32 i = 0; i < OpenSet.Num(); ++i)
	{
		int32 v = OpenSet[i];

		if (v < 0 || v >= Nodes.Num()) continue;

		UE_LOG(LogTemp, Log, TEXT("UPathFinderComponent::PopLowestF__Node=%3d F=%.2f G=%.2f H=%.2f"), v, Nodes[v].FCost, Nodes[v].GCost, Nodes[v].HCost);

		if (Nodes[v].FCost < BestF)
		{
			BestF = Nodes[v].FCost;
			BestIdxInOpen = i;
		}
	}

	if (BestIdxInOpen == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPathFinderComponent::PopLowestF__Failde"));
		return INDEX_NONE;
	}
	
	int32 NodeIndex = OpenSet[BestIdxInOpen];
	UE_LOG(LogTemp, Log, TEXT("UPathFinderComponent::PopLowestF__PopNode=%3d F=%.2f G=%.2f H=%.2f OpenSet.Num=%d"), NodeIndex, Nodes[NodeIndex].FCost, Nodes[NodeIndex].GCost, Nodes[NodeIndex].HCost,OpenSet.Num());

	OpenSet.RemoveAtSwap(BestIdxInOpen);
	Nodes[NodeIndex].InOpenFlags = 0;

	return NodeIndex;
}

//	�p�X�̏�����
void UPathFinderComponent::ClearResultPath()
{
	m_ResultPath.Empty();
	NextPathIndex = 0;
	m_bLastPath = false;
}

/**
 * @brief	GoalIndex����e��H���čŏI�I�Ȍo�H��쐬	
 */
void UPathFinderComponent::MakeResultPath(int32 GoalIndex)
{
	m_ResultPath.Empty();
	if (!m_GraphData)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPathFinderComponent::MakeResultPath__Failde"));
		return;
	}

	//	�S�[������e��H���āA�ŏI�I�Ȍo�H��쐬
	int32 CurrentIndex = GoalIndex;
	while (CurrentIndex != INDEX_NONE)
	{
		m_ResultPath.Add(m_GraphData->m_Vertices[CurrentIndex].Position);
		
		if (Nodes.IsValidIndex(CurrentIndex))
		{
			CurrentIndex = Nodes[CurrentIndex].Parent;
		}
		else
		{
			CurrentIndex = INDEX_NONE;
		}

	}
	//	�X�^�[�g����ɂȂ�悤�ɋt����
	Algo::Reverse(m_ResultPath); 
	NextPathIndex = 0;

	UE_LOG(LogTemp, Warning, TEXT("UPathFinderComponent::MakeResultPath__Success m_ResultPath.Num = %d"),m_ResultPath.Num());

}

/**
 * @brief	�����_���ȃm�[�h�̈ʒu��擾
 */
FVector UPathFinderComponent::GetRandomLocation()
{
	if (m_GraphData->m_Vertices.Num() == 0)
	{
		return FVector();
	}

	int32 Index = FMath::RandHelper(m_GraphData->m_Vertices.Num());

	return m_GraphData->m_Vertices[Index].Position;

}
