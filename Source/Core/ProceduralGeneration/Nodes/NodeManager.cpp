// Fill out your copyright notice in the Description page of Project Settings.


#include "NodeManager.h"
#include "FloorNode.h"
#include "CeilingNode.h"
#include "PropNode.h"
#include "../Prototypes/BasePrototype.h"
#include "../../HelperFiles/DefinedDebugHelpers.h"

// Sets default values
ANodeManager::ANodeManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
}

void ANodeManager::PreloadNodes(TMap<int, TSubclassOf<class ABasePrototype>> ChunkData)
{
	FActorSpawnParameters Params;
	for (auto& Entry : ChunkData)
	{
		ABaseNode* Node = Nodes[Entry.Key];
		Params.Owner = Node;
		Node->ActivePrototype = GetWorld()->SpawnActor<ABasePrototype>(Entry.Value.Get(),
			Node->GetActorLocation(),
			FRotator(0), Params);
		Node->TransientPotentialPrototypes.Empty();
		Node->TransientPotentialPrototypes.Add(Entry.Value);
		NodesToCollapse.Remove(Node);
	}
	for (auto& Entry : ChunkData)
	{
		PropagateConsequences(Nodes[Entry.Key]);
	}
}

void ANodeManager::OnConstruction(const FTransform& Transform)
{

}

// Called when the game starts or when spawned
void ANodeManager::BeginPlay()
{
	Super::BeginPlay();
}

void ANodeManager::SpawnNodes()
{
	FActorSpawnParameters Params;
	Params.Owner = this;
	FVector ManagerLocation = GetActorLocation();
	// generate floor nodes
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			FVector Location = FVector(-2000 + 500 * x + ManagerLocation.X, -2000 + 500 * y + ManagerLocation.Y, 500);
			FRotator Rotation = FRotator(0);
			AFloorNode* NewNode = GetWorld()->SpawnActor<AFloorNode>(Location, Rotation, Params);

			Nodes.Add(NewNode);
		}
	}
	//generate ceiling nodes
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			FVector Location = FVector(-2000 + 500 * x + ManagerLocation.X, -2000 + 500 * y + ManagerLocation.Y, 1000);
			FRotator Rotation = FRotator(0);
			ACeilingNode* NewNode = GetWorld()->SpawnActor<ACeilingNode>(Location, Rotation, Params);

			Nodes.Add(NewNode);
		}
	}
	//generate prop nodes
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			FVector Location = FVector(-2000 + 500 * x + ManagerLocation.X, -2000 + 500 * y + ManagerLocation.Y, 500);
			FRotator Rotation = FRotator(0);
			APropNode* NewNode = GetWorld()->SpawnActor<APropNode>(Location, Rotation, Params);

			Nodes.Add(NewNode);
		}
	}

	//calculate neighbors
	int MAX = Nodes.Num();
	for (int i = 0; i < MAX; i++)
	{
		if (i % 8 != 0)
		{
			FNodeSocketData NewSocketData;
			NewSocketData.Node = Nodes[i - 1];
			if (Cast<AFloorNode>(Nodes[i]) || Cast<APropNode>(Nodes[i]))
				NewSocketData.Type = ESocketCategory::FloorSocket;
			else if (Cast<ACeilingNode>(Nodes[i]))
				NewSocketData.Type = ESocketCategory::CeilingSocket;
			Nodes[i]->Neighbors.Add(ESocketDirection::South, NewSocketData);
		}
		if (i % 8 != 7)
		{
			FNodeSocketData NewSocketData;
			NewSocketData.Node = Nodes[i + 1];
			if (Cast<AFloorNode>(Nodes[i]) || Cast<APropNode>(Nodes[i]))
				NewSocketData.Type = ESocketCategory::FloorSocket;
			else if (Cast<ACeilingNode>(Nodes[i]))
				NewSocketData.Type = ESocketCategory::CeilingSocket;
			Nodes[i]->Neighbors.Add(ESocketDirection::North, NewSocketData);
		}
		if ((i >= 8 && i < 64) || (i >= 72 && i < 128) || (i > 135))
		{
			FNodeSocketData NewSocketData;
			NewSocketData.Node = Nodes[i - 8];
			if (Cast<AFloorNode>(Nodes[i]) || Cast<APropNode>(Nodes[i]))
				NewSocketData.Type = ESocketCategory::FloorSocket;
			else if (Cast<ACeilingNode>(Nodes[i]))
				NewSocketData.Type = ESocketCategory::CeilingSocket;
			Nodes[i]->Neighbors.Add(ESocketDirection::West, NewSocketData);
		}
		if ((i <= 55 && i <= 71) || (i >= 64 && i <= 119) || (i < 183 && i > 127))
		{
			FNodeSocketData NewSocketData;
			NewSocketData.Node = Nodes[i + 8];
			if (Cast<AFloorNode>(Nodes[i]) || Cast<APropNode>(Nodes[i]))
				NewSocketData.Type = ESocketCategory::FloorSocket;
			else if (Cast<ACeilingNode>(Nodes[i]))
				NewSocketData.Type = ESocketCategory::CeilingSocket;
			Nodes[i]->Neighbors.Add(ESocketDirection::East, NewSocketData);
		}
		FNodeSocketData NewSocketData;
		if (Cast<AFloorNode>(Nodes[i]))
		{
			NewSocketData.Node = Nodes[i + 64];
			NewSocketData.Type = ESocketCategory::FloorCeilingSocket;
			Nodes[i]->Neighbors.Add(ESocketDirection::Top, NewSocketData);
			NewSocketData.Node = Nodes[i + 128];
			NewSocketData.Type = ESocketCategory::FloorPropSocket;
			Nodes[i]->Neighbors.Add(ESocketDirection::Bottom, NewSocketData);
		}
		else if (Cast<ACeilingNode>(Nodes[i]))
		{
			NewSocketData.Node = Nodes[i - 64];
			NewSocketData.Type = ESocketCategory::FloorCeilingSocket;
			Nodes[i]->Neighbors.Add(ESocketDirection::Bottom, NewSocketData);
		}
		else if (Cast<APropNode>(Nodes[i]))
		{
			NewSocketData.Node = Nodes[i - 128];
			NewSocketData.Type = ESocketCategory::FloorPropSocket;
			Nodes[i]->Neighbors.Add(ESocketDirection::Bottom, NewSocketData);
		}
	}
	HackTransients();
	NodesToCollapse = Nodes;
}

void ANodeManager::CollapseNodes()
{
	//PropagateConsequences(NodesToCollapse[0]);
	while (NodesNeedCollapsed())
	{
		// iterate the wave function 
		int MinEntropyIndex = GetMinEntropyNodeIndex();
		if (MinEntropyIndex != -1 && MinEntropyIndex < NodesToCollapse.Num())
			// collapse the node at MinEntropyIndex
			CollapseNode(MinEntropyIndex);
		else
			break;
	}
}

int ANodeManager::GetMinEntropyNodeIndex()
{
	int MinEntropy = 1000000;
	TArray<int> PossibleIndicies;
	int MAX = NodesToCollapse.Num();
	for (int i = 0; i < MAX; i++)
	{
		int Entropy = NodesToCollapse[i]->TransientPotentialPrototypes.Num();
		if (Entropy == 0)
			continue;
		if (Entropy == MinEntropy)
		{
			PossibleIndicies.Add(i);
		}
		else if (Entropy < MinEntropy)
		{
			MinEntropy = Entropy;
			PossibleIndicies.Empty();
			PossibleIndicies.Add(i);
		}
	}
	
	if (PossibleIndicies.Num())
		return PossibleIndicies[FMath::RandRange(0, PossibleIndicies.Num() - 1)];
	
	return -1;
}

void ANodeManager::CollapseNode(int Index)
{
	ABaseNode* Node = NodesToCollapse[Index];
	FActorSpawnParameters Params;
	Params.Owner = Node;

	if (!Node->TransientPotentialPrototypes.Num())
	{
		Node->ActivePrototype = GetWorld()->SpawnActor<ABasePrototype>(*Node->PotentialPrototypes.end(),
			Node->GetActorLocation(),
			FRotator(0), Params);
	}
	else
	{
		int ProtoIndex = FMath::RandRange(0, Node->TransientPotentialPrototypes.Num() - 1);

		Node->ActivePrototype = GetWorld()->SpawnActor<ABasePrototype>(Node->TransientPotentialPrototypes[ProtoIndex].Get(),
			Node->GetActorLocation(),
			FRotator(0), Params);
		auto Temp = Node->TransientPotentialPrototypes[ProtoIndex];
		Node->TransientPotentialPrototypes.Empty();
		Node->TransientPotentialPrototypes.Add(Temp);
	}
	NodesToCollapse.Remove(Node);
	PropagateConsequences(Node);
}

void ANodeManager::PropagateConsequences(ABaseNode* CollapsedNode)
{
	ABaseNode* Node = CollapsedNode;
	TSet<ABaseNode*> NodesToPropagate;
	TSet<ABaseNode*> NodesVisited;
	NodesToPropagate.Add(Node);
	while (NodesToPropagate.Num())
	{
		Node = *NodesToPropagate.begin();
		if (NodesVisited.Contains(Node))
		{
			NodesToPropagate.Remove(Node);
			continue;
		}
		for (auto& Neighbor : Node->Neighbors)
		{
			//if (Node->Propagate(Neighbor.Key))
				//NodesToPropagate.Add(Neighbor.Value.Node);
			Node->Propagate(Neighbor.Key);
		}
		NodesToPropagate.Remove(Node);
		NodesVisited.Add(Node);
	}
}

void ANodeManager::HackTransients()
{// WORKS
	for (auto& Node : Nodes)
	{
		for (int j = 0; j < Node->TransientPotentialPrototypes.Num(); j++)
		{
			UObject* Reference = Node->TransientPotentialPrototypes[j].GetDefaultObject();
			ABasePrototype* Proto = Cast<ABasePrototype>(Reference);
			if (Proto)
			{
				for (int i = 0; i < 6; i++)
				{
					ESocketDirection Direction = static_cast<ESocketDirection>(i);
					FString String;
					String.AppendInt(i);
					if (!Node->Neighbors.Contains(Direction) && Proto->Rules.Contains(Direction))
					{
						Node->TransientPotentialPrototypes.RemoveAt(j);
						j--;
						break;
					}
				}
			}
		}
	}
}
