// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseNode.h"
#include "GameFramework/Actor.h"
#include "FloorNode.generated.h"

UCLASS()
class ALLMALLSAREWALLS_API AFloorNode : public ABaseNode
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorNode();

protected:
	
	UPROPERTY(VisibleAnywhere)
	ABaseNode* TopNeighbor;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
