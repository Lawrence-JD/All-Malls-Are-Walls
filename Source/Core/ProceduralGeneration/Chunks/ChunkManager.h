// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChunkBase.h"
#include "GameFramework/Actor.h"
#include "ChunkManager.generated.h"

UCLASS()
class ALLMALLSAREWALLS_API AChunkManager : public AActor
{
	GENERATED_BODY()
	
public:	

	TArray<AChunkBase*> Chunks;

	// Sets default values for this actor's properties
	AChunkManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
