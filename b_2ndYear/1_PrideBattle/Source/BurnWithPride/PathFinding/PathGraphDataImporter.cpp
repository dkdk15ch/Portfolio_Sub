// Fill out your copyright notice in the Description page of Project Settings.

#include "PathGraphDataImporter.h"

#if WITH_EDITOR

#include "PathGraphData.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"
#include "Misc/PackageName.h"
#include "OBJParser.h"

/**
 * @brief                       obj�t�@�C����ǂݍ���ŁAPathGraphData�^�̃A�Z�b�g��쐬
 * @param ObjFilePath           obj�̃t�@�C���p�X
 * @param DestinationFolder     �ۑ���̃t�H���_
 */
void UPathGraphDataImporter::MakeDataAssetFromOBJ(const FString& ObjFilePath, const FString& DestinationFolder)
{
    //  �t�@�C���`�F�b�N
    if (ObjFilePath.IsEmpty() || !FPaths::FileExists(ObjFilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("UPathGraphDataImporter::MakeDataAssetFromOBJ__Failed to found /file"));
    }

    //  �t�@�C���ǂݍ���
    FString ObjContext;
    if (!FFileHelper::LoadFileToString(ObjContext, *ObjFilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("UPathGraphDataImporter::MakeDataAssetFromOBJ__Failed to load file"));
    }

    //  �ۑ���p�X
    FString FileName = FPaths::GetBaseFilename(ObjFilePath);
    FString DestFolder = DestinationFolder;
    if (!DestFolder.StartsWith(TEXT("/Game")))
    {
        DestFolder = FString(TEXT("/Game/")) + DestFolder;
    }

    FString PackageName = DestFolder + FString(TEXT("/")) + FileName;
    PackageName.ReplaceInline(TEXT("//"), TEXT("/"));

    //  �T���m�[�h�̃f�[�^��擾���č쐬
    TArray<FVertex> Vertices;
    if (!OBJParser::ParseOBJ(ObjContext, Vertices))
    {
        UE_LOG(LogTemp, Warning, TEXT("UPathGraphDataImporter::MakeDataAssetFromOBJ__Failed to paser OBJ"));
    }

    //  �p�b�P�[�W�쐬
    UPackage* Package = CreatePackage(*PackageName);
    Package->FullyLoad();

    FString AssetName = FileName;
    UPathGraphData* NewData = NewObject<UPathGraphData>(Package, UPathGraphData::StaticClass(), *AssetName, RF_Public | RF_Standalone);
    if (!NewData)
    {
        UE_LOG(LogTemp, Error, TEXT("UPathGraphDataImporter::MakeDataAssetFromOBJ__Failed to create asset"));
    }
    NewData->m_Vertices = MoveTemp(Vertices);
        
    //  �A�Z�b�g�o�^
    Package->MarkPackageDirty();
    FAssetRegistryModule::AssetCreated(NewData);

    //  �t�@�C������擾���ĕۑ�
    FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());

    FSavePackageArgs SaveArgs = FSavePackageArgs();
    SaveArgs.SaveFlags = RF_Public | RF_Standalone;

    if (UPackage::SavePackage(Package, NewData, *PackageFileName, SaveArgs))
    {
        UE_LOG(LogTemp, Log, TEXT("Save DataAsset: %s"),*PackageFileName);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("UPathGraphDataImporter::MakeDataAssetFromOBJ__Failed to save package"));
    }
}

#endif // WITH_EDITOR