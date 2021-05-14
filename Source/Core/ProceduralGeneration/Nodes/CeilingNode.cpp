// Fill out your copyright notice in the Description page of Project Settings.


#include "CeilingNode.h"

// Sets default values
ACeilingNode::ACeilingNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	FString RootPath = FPaths::GetProjectFilePath();
	RootPath.RemoveFromEnd("AllMallsAreWalls.uproject");
	RootPath.Append("\\Content\\AllMallsAreWalls\\Core\\ProceduralBPs\\Prototypes\\CeilingPieces");
	FPlatformFileManager* FileManager = new FPlatformFileManager;
	TArray<FString> FileNames;
	FVisitor Visitor(FileNames, ".uasset", true, false);
	FString Ending;
	Ending = "_C";

	if (FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*RootPath))
	{
		FileManager->Get().GetPlatformFile().IterateDirectory(*RootPath, Visitor);
	}
	for (int i = 0; i < FileNames.Num(); i++)
	{
		FileNames[i].RemoveFromEnd(".uasset");
		FileNames[i].RemoveFromEnd(".uexp");

		FString AssetReference = "/Game/AllMallsAreWalls/Core/ProceduralBPs/Prototypes/CeilingPieces/" + FileNames[i] + '.' + FileNames[i] + "_C";
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
void ACeilingNode::BeginPlay()
{
	Super::BeginPlay();
	
}
