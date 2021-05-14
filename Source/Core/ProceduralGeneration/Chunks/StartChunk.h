// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChunkBase.h"
#include "GameFramework/Actor.h"
#include "StartChunk.generated.h"

UCLASS()
class ALLMALLSAREWALLS_API AStartChunk : public AChunkBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStartChunk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
