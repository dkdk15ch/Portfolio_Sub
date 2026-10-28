// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFinding/OBJParser.h"

/**
 * @brief			obj���璸�_������o���A�ʏ�񂩂�אڃf�[�^��Z�b�g����B
 *					�E�ʂ��O�p�`�����ꂽ�����b�V���̎g�p��z��(Blender�ō쐬)
 *					�E�󔒂ŋ�؂�ꂽ�f�[�^��z��(v 1.0 0.7 0.5 / f 1 2 3)
 * @param obj			obj
 * @param OutputData	
 * @return 
 */
bool OBJParser::ParseOBJ(const FString& obj, TArray<FVertex>& OutputData)
{
	TArray<FString> Lines;
	obj.ParseIntoArrayLines(Lines, true);

	int startLine = 0;
	//	�擪�̒��_�f�[�^�܂Ői�߂�
	for (const FString& RawLine : Lines)
	{
		FString Line = RawLine.TrimStartAndEnd();
		if (Line.IsEmpty())			continue;
		if (Line[0] == TEXT('v'))	break;

		startLine++;
	}

	//	���_��擾
	for (int i = startLine; i < Lines.Num(); i++)
	{
		FString Line = Lines[i].TrimStartAndEnd();
		if (Line.IsEmpty()) break;

		TArray<FString> Positions;
		Line.ParseIntoArrayWS(Positions);
		if (!Positions[0].Equals(TEXT("v"), ESearchCase::CaseSensitive)) break;

		if (Positions.Num() == 4)
		{
			const float x = FCString::Atof(*Positions[1]);
			const float y = FCString::Atof(*Positions[2]);
			const float z = FCString::Atof(*Positions[3]);
			const FVector VerPos = FVector(x, y, z);
			OutputData.Add(FVertex(VerPos));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OBJParse::ParseOBJ__Failde Data Mismatch"));
			return false;
		}

		startLine++;
	}

	if (OutputData.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("OBJParse::ParseOBJ__Failde No Vertex Data"));
		return false;
	}

	//	�ʃf�[�^�܂Ői�߂�
	for (int i = startLine; i < Lines.Num(); i++)
	{
		FString Line = Lines[i].TrimStartAndEnd();
		if (Line.IsEmpty()) continue;
		if (Line[0] == TEXT('f')) break;

		startLine++;
	}
	//	�ʂ�擾���āA�אڃf�[�^�Ƃ��ė��p
	for (int i = startLine; i < Lines.Num(); i++)
	{
		FString Line = Lines[i].TrimStartAndEnd();
		if (Line.IsEmpty()) break;

		TArray<FString> Faces;
		Line.ParseIntoArrayWS(Faces);
		if (!Faces[0].Equals(TEXT("f"), ESearchCase::CaseSensitive)) break;

		if (Faces.Num() == 4)
		{
			int32 index1 = FCString::Atoi(*Faces[1]);
			int32 index2 = FCString::Atoi(*Faces[2]);
			int32 index3 = FCString::Atoi(*Faces[3]);
			//	obj�̃C���f�b�N�X��1����Ȃ̂�-1
			index1--;
			index2--;
			index3--;

			//	�͈̓`�F�b�N
			if (!OutputData.IsValidIndex(index1) || 
				!OutputData.IsValidIndex(index2) || 
				!OutputData.IsValidIndex(index3))
			{
				UE_LOG(LogTemp, Error, TEXT("OBJParse::ParseOBJ_Failde Out of Range Vertex Index"));
				return false;
			}

			//	�o�����ɒǉ�
			FNeighbor::AddNeighborIfNotExists(OutputData, index1, index2);
			FNeighbor::AddNeighborIfNotExists(OutputData, index2, index1);
			FNeighbor::AddNeighborIfNotExists(OutputData, index1, index3);
			FNeighbor::AddNeighborIfNotExists(OutputData, index3, index1);
			FNeighbor::AddNeighborIfNotExists(OutputData, index2, index3);
			FNeighbor::AddNeighborIfNotExists(OutputData, index3, index2);
		}
	}

	return true;
}
