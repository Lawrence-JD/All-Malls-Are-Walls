// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseNode.h"
#include "../../HelperFiles/DefinedDebugHelpers.h"

TArray<TSubclassOf<class ABasePrototype>> ABaseNode::PotentialPrototypes;

// Sets default values
ABaseNode::ABaseNode()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Icon = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Icon"));
    // Structure to hold one-time initialization
    struct FConstructorStatics
    {
        ConstructorHelpers::FObjectFinderOptional<UTexture2D> DecalTexture;
        FConstructorStatics() : DecalTexture(TEXT("/Engine/Editor/Slate/Icons/AssetIcons/Sphere_64x.Sphere_64x")) {}
    };
    static FConstructorStatics ConstructorStatics;
    // Set up sprite component
    if (Icon)
    {
        Icon->Sprite = ConstructorStatics.DecalTexture.Get();
        Icon->SetupAttachment(RootComponent);
        Icon->SetUsingAbsoluteScale(true);
        Icon->bIsScreenSizeScaled = true;
        Icon->bReceivesDecals = false;
        Icon->bHiddenInGame = true;
    }

	Outliner = CreateDefaultSubobject<UBoxComponent>(TEXT("Outliner"));
	Outliner->SetBoxExtent(FVector(250));
	Outliner->SetupAttachment(GetRootComponent());
    Outliner->SetVisibility(true);
    Outliner->SetHiddenInGame(true);
    Outliner->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ABaseNode::BeginPlay()
{
	Super::BeginPlay();
	
}

bool ABaseNode::Propagate(ESocketDirection Direction)
{
	bool ret = false;
	if(!Neighbors.Contains(Direction) || Neighbors[Direction].Node->ActivePrototype)
		return false;
	ESocketDirection back;
	if(Neighbors[Direction].Type == ESocketCategory::FloorPropSocket)
		back = Direction;
	else
		back = static_cast<ESocketDirection>((static_cast<uint8>(Direction) + 3) % 6);
	ABaseNode * other = Neighbors[Direction].Node;
	TSet<TSubclassOf<ABasePrototype>> edges = other->FindEdges(back);
	if(edges.Num() == 0)
		return false;
	ret = edges.Num() < other->TransientPotentialPrototypes.Num();
	if(ret)
	{
		other->TransientPotentialPrototypes.Empty();
		for(auto prot: edges)
		{
			other->TransientPotentialPrototypes.Add(prot);
		}
	}
	return ret;
}

TSet<TSubclassOf<ABasePrototype>> ABaseNode::FindEdges(ESocketDirection Direction)
{
	TSet<TSubclassOf<ABasePrototype>> ret;
	ESocketDirection back;
	if (Neighbors.Contains(Direction))
	{
		if (Neighbors[Direction].Type == ESocketCategory::FloorPropSocket)
			back = Direction;
		else
			back = static_cast<ESocketDirection>((static_cast<uint8>(Direction) + 3) % 6);
		ABaseNode* other = Neighbors[Direction].Node;
		for (auto prototype1 : TransientPotentialPrototypes)
		{
			for (auto prototype2 : other->TransientPotentialPrototypes)
			{
				UObject* Reference = prototype1.GetDefaultObject();
				ABasePrototype* proto1 = Cast<ABasePrototype>(Reference);
				Reference = prototype2.GetDefaultObject();
				ABasePrototype* proto2 = Cast<ABasePrototype>(Reference);
				//if (!proto1->Rules.Contains(Direction) && !proto2->Rules.Contains(back))
				//ret.Add(prototype1);
				if (proto1->Rules.Contains(Direction) && proto2->Rules.Contains(back))
				{
					if (proto1->Rules[Direction]->Tag == ESocketTag::NoRepeat || proto2->Rules[back]->Tag == ESocketTag::NoRepeat)
						if (proto1->Rules[Direction]->Tag != proto2->Rules[back]->Tag)
						{
							ret.Add(prototype1);
							continue;;
						}

					if (proto1->Rules[Direction]->Tag == proto2->Rules[back]->Tag || proto1->Rules[Direction]->Tag == ESocketTag::All || proto1->Rules[Direction]->Tag == ESocketTag::All) {
						if (proto1->Rules[Direction]->Type == ESocketCategory::FloorSocket || proto1->Rules[Direction]->Type == ESocketCategory::CeilingSocket)
						{
							if (proto1->Rules[Direction]->Value == 1 && proto2->Rules[back]->Value == 2 ||
								proto1->Rules[Direction]->Value == 2 && proto2->Rules[back]->Value == 1)
								ret.Add(prototype1);
							else if (proto1->Rules[Direction]->Value != 1 && proto1->Rules[Direction]->Value != 2
								&& proto1->Rules[Direction]->Value == proto2->Rules[back]->Value)
								ret.Add(prototype1);
							continue;
						}
						if (proto1->Rules[Direction]->Value == proto2->Rules[back]->Value)
							ret.Add(prototype1);
					}
				}
			}
		}
	}
	return ret;
}
