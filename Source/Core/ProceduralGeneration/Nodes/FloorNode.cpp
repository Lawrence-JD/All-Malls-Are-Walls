// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorNode.h"
#include "../../HelperFiles/DefinedDebugHelpers.h"

// Sets default values
AFloorNode::AFloorNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	FString RootPath = FPaths::GetProjectFilePath();
	RootPath.RemoveFromEnd("AllMallsAreWalls.uproject");
	RootPath.Append("\\Content\\AllMallsAreWalls\\Core\\ProceduralBPs\\Prototypes\\FloorPieces");
	FPlatformFileManager* FileManager = new FPlatformFileManager;
	TArray<FString> FileNames;
	FVisitor Visitor(FileNames, ".uasset", true, false);

	if (FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*RootPath))
	{
		FileManager->Get().GetPlatformFile().IterateDirectory(*RootPath, Visitor);
	}
	for (int i = 0; i < FileNames.Num(); i++)
	{
		FileNames[i].RemoveFromEnd(".uasset");
		FileNames[i].RemoveFromEnd(".uexp");

		FString AssetReference = "Class'/Game/AllMallsAreWalls/Core/ProceduralBPs/Prototypes/FloorPieces/" + FileNames[i] + '.' + FileNames[i] + "_C'";
		ConstructorHelpers::FObjectFinder<UClass> BlueprintProtoFinder(*AssetReference);
		if (BlueprintProtoFinder.Object)
		{
			PotentialPrototypes.Add((UClass*)BlueprintProtoFinder.Object);
			TransientPotentialPrototypes.Add((UClass*)BlueprintProtoFinder.Object);
		}
	}
	ConstructorHelpers::FObjectFinder<UClass> EmptyProtoFinder(TEXT("/Game/AllMallsAreWalls/Core/ProceduralBPs/Prototypes/Archived/BP_EmptyProto.BP_EmptyProto_C"));
	if (EmptyProtoFinder.Object)
	{
		PotentialPrototypes.Add((UClass*)EmptyProtoFinder.Object);
	}
}

// Called when the game starts or when spawned
void AFloorNode::BeginPlay()
{
	Super::BeginPlay();
	
}

