// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseNode.h"
#include "GameFramework/Actor.h"
#include "CeilingNode.generated.h"

UCLASS()
class ALLMALLSAREWALLS_API ACeilingNode : public ABaseNode
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACeilingNode();

protected:

	UPROPERTY(VisibleAnywhere)
	ABaseNode* BottomNeighbor;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
