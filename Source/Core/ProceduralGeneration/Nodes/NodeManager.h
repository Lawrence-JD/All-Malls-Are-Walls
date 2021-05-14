// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseNode.h"
#include "GameFramework/Actor.h"
#include "NodeManager.generated.h"

UCLASS()
class ALLMALLSAREWALLS_API ANodeManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANodeManager();

	void PreloadNodes(TMap<int, TSubclassOf<class ABasePrototype>> ChunkData);

protected:

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	TArray<ABaseNode*> Nodes;

	UPROPERTY(VisibleAnywhere)
	TArray<ABaseNode*> NodesToCollapse;

	void OnConstruction(const FTransform& Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void SpawnNodes();

	UFUNCTION()
	FORCEINLINE bool NodesNeedCollapsed() { return (bool)NodesToCollapse.Num(); };

	UFUNCTION()
	void CollapseNodes();

	UFUNCTION()
	int GetMinEntropyNodeIndex();

	UFUNCTION()
	void CollapseNode(int Index);

	UFUNCTION()
	void PropagateConsequences(ABaseNode* CollapsedNode);

	UFUNCTION()
	void HackTransients();

	friend class AChunkBase;
};
