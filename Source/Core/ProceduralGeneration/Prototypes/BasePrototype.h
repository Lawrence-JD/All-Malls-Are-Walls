// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "BasePrototype.generated.h"



UENUM(BlueprintType)
enum class ESocketTag : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Store = 1 UMETA(DisplayName = "Store"),
	Hall = 2 UMETA(DisplayName = "Hall"),
	Shoe = 3 UMETA(DisplayName = "Shoe"),
	Toy = 4 UMETA(DisplayName = "Toy"),
	Game = 5 UMETA(DisplayName = "Game"),
	Bath = 6 UMETA(DisplayName = "Bath"),
	Clothes = 7 UMETA(DisplayName = "Clothes"),
	Jewelry = 8 UMETA(DisplayName = "Jewelry"),
    All = 9 UMETA(DisplayName = "All"),
	NoRepeat = 10 UMETA(DisplayName = "NoRepeat"),
	NoProp = 11 UMETA(DisplayName = "NoProp")
};

UENUM(BlueprintType)
enum class ESocketCategory : uint8
{
	FloorSocket  = 0 UMETA(DisplayName = "FloorSocket"),
	FloorPropSocket = 1 UMETA(DisplayName = "FloorPropSocket"),
	PropSocket = 2 UMETA(DisplayName = "PropSocket"),
	FloorCeilingSocket = 3 UMETA(DisplayName = "FloorCeilingSocket"),
	CeilingSocket = 4 UMETA(DisplayName = "CeilingSocket")
};

UENUM(BlueprintType)
enum class ESocketDirection : uint8
{
	Top = 0	UMETA(DisplayName = "Top"),
    North = 1 UMETA(DisplayName = "North"),
    East = 2 UMETA(DisplayName = "East"),
	Bottom = 3 UMETA(DisplayName = "Bottom"),
    South = 4 UMETA(DisplayName = "South"),
    West = 5 UMETA(DisplayName = "West")
};

UENUM(BlueprintType)
enum class EFloorSocket : uint8
{
	None = 0 UMETA(DisplayName = "None"),
    LeftWall = 1 UMETA(DisplayName = "LeftWall"),
    RightWall = 2 UMETA(DisplayName = "RightWall"),
	Floor = 3 UMETA(DisplayName = "Floor")
};

UENUM(BlueprintType)
enum class EPropSocket : uint8
{
	None = 0 UMETA(DisplayName = "None"),
    Blocked = 1 UMETA(DisplayName = "Blocked"),
    Unblocked = 2 UMETA(DisplayName = "Unblocked"),
	All = 3 UMETA(DisplayName = "All")
};

UENUM(BlueprintType)
enum class ECeilingSocket : uint8
{
	None = 0 UMETA(DisplayName = "None"),
    LeftWall = 1 UMETA(DisplayName = "LeftWall"),
    RightWall = 2 UMETA(DisplayName = "RightWall"),
    Floor = 3 UMETA(DisplayName = "Ceiling")
};

UENUM(BlueprintType)
enum class EWallSocket : uint8
{
	None = 0 UMETA(DisplayName = "None"),
    North = 1 UMETA(DisplayName = "North"),
    Northeast = 2 UMETA(DisplayName = "Northeast"),
    East = 3 UMETA(DisplayName = "East"),
    Southeast = 4 UMETA(DisplayName = "Southeast"),
    South = 5 UMETA(DisplayName = "South"),
    Southwest = 6 UMETA(DisplayName = "Southwest"),
    West = 7 UMETA(DisplayName = "West"),
    Northwest = 8 UMETA(DisplayName = "Northwest")
};


UCLASS(BlueprintType)
class ALLMALLSAREWALLS_API UPrototypeSocketData : public UActorComponent
{
	GENERATED_BODY()

	public:
	UPrototypeSocketData() :  UActorComponent() {};
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Data")
	ESocketTag Tag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Data")
	ESocketCategory Type;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Data")
	uint8 Value;
	
};

USTRUCT(BlueprintType)
struct FNodeSocketData
{
	GENERATED_BODY()
	
	ESocketCategory Type;
	class ABaseNode * Node;
};

UCLASS()
class ALLMALLSAREWALLS_API ABasePrototype : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Rules|Data")
	ESocketDirection editSocket;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Rules|Data")
	ESocketCategory editType;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Rules|Data")
	ESocketTag editTag;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Rules|Data|Types")
	EFloorSocket FloorData;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Rules|Data|Types")
	EPropSocket PropData;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Rules|Data|Types")
	ECeilingSocket CeilData;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Rules|Data|Types")
	EWallSocket WallData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Rules|Read Write")
	bool bSet;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Rules|Read Write")
	bool bGet;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Rules|Read Write")
	bool bRotate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Rules|Read Write")
	bool bMirror;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Rules|Read Write")
	bool bAllAll;
	

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Rules")
	TMap<ESocketDirection, UPrototypeSocketData *> Rules;
	
	// Sets default values for this actor's properties
	ABasePrototype();
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
protected:

};
