// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Prototypes/BasePrototype.h"
#include "../Visitor.h"
#include "HAL/FileManager.h"
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"
#include "GameFramework/Actor.h"
#include "BaseNode.generated.h"

UCLASS()
class ALLMALLSAREWALLS_API ABaseNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseNode();

protected:

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UBillboardComponent* Icon;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Outliner;

	UPROPERTY(VisibleAnywhere)
	TMap<ESocketDirection, FNodeSocketData> Neighbors;

	static TArray<TSubclassOf<class ABasePrototype>> PotentialPrototypes;

	UPROPERTY(VisibleAnywhere)
	TArray<TSubclassOf<class ABasePrototype>> TransientPotentialPrototypes;

	UPROPERTY(VisibleAnywhere)
	ABasePrototype* ActivePrototype;

	UPROPERTY()
	TArray<FString> Filenames;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	bool Propagate(ESocketDirection Direction);

	TSet<TSubclassOf<ABasePrototype>> FindEdges(ESocketDirection Direction);

	friend class ANodeManager;
};
