// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Nodes/NodeManager.h"
#include "GameFramework/Actor.h"
#include "ChunkBase.generated.h"

// Deme's Heiroglyphs

UENUM()
enum EConnectorDirection
{
    North UMETA(DisplayName = "North"),
    East UMETA(DisplayName = "East"),
    South UMETA(DisplayName = "South"),
    West UMETA(DisplayName = "West")
};

UENUM()
enum EConnectorCategory
{
	Blocked UMETA(DisplayName = "Blocked"),
    Short UMETA(DisplayName = "Short"),
    Wide UMETA(DisplayName = "Wide")
};

UCLASS()
class ALLMALLSAREWALLS_API AChunkBase : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ANodeManager* NodeManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int, TSubclassOf<class ABasePrototype>> ChunkData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TEnumAsByte<EConnectorDirection>, TEnumAsByte<EConnectorCategory>> Connections;
	
	// Sets default values for this actor's properties
	AChunkBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Tick(float DeltaTime) override;
	bool Begun = false;
	float Wait = 10;

};
