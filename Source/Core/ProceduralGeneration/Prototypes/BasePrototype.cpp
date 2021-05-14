// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePrototype.h"

// Sets default values
ABasePrototype::ABasePrototype()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	for(int i = 0; i < 6; i++)
	{
		UPrototypeSocketData * tmp = CreateDefaultSubobject<UPrototypeSocketData>("DataPacket" + i);
	}
}
#if WITH_EDITOR
void ABasePrototype::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if(bGet)
	{
		bGet = false;
		if(Rules.Contains(editSocket))
		{
			editType = Rules[editSocket]->Type;
			editTag = Rules[editSocket]->Tag;
			FloorData = EFloorSocket::None;
			WallData = EWallSocket::None;
			CeilData = ECeilingSocket::None;
			PropData = EPropSocket::None;
			switch (editType)
			{
			case ESocketCategory::FloorSocket:
				FloorData = static_cast<EFloorSocket>(Rules[editSocket]->Value);
				break;
			case ESocketCategory::FloorPropSocket:
			case ESocketCategory::FloorCeilingSocket:
				WallData = static_cast<EWallSocket>(Rules[editSocket]->Value);
				break;
			case ESocketCategory::PropSocket:
				PropData = static_cast<EPropSocket>(Rules[editSocket]->Value);
				break;
			case ESocketCategory::CeilingSocket:
				CeilData = static_cast<ECeilingSocket>(Rules[editSocket]->Value);
				break;
			}
		}
	}
	if(bSet)
	{
		bSet = false;
		UPrototypeSocketData * tmp = Cast<UPrototypeSocketData>(GetComponentsByClass(UPrototypeSocketData::StaticClass())[static_cast<uint8>(editSocket)]);
		tmp->Tag = editTag;
		tmp->Type = editType;
		switch (editType)
		{
		case ESocketCategory::FloorSocket:
			tmp->Value = static_cast<uint8>(FloorData);
			break;
		case ESocketCategory::FloorPropSocket:
        case ESocketCategory::FloorCeilingSocket:
            tmp->Value = static_cast<uint8>(WallData);
			break;
		case ESocketCategory::PropSocket:
			tmp->Value = static_cast<uint8>(PropData);
			break;
		case ESocketCategory::CeilingSocket:
			tmp->Value = static_cast<uint8>(CeilData);
			break;
		}
		if(!Rules.Contains(editSocket))
			Rules.Add(editSocket, tmp);
		editTag = ESocketTag::None;
		FloorData = EFloorSocket::None;
		WallData = EWallSocket::None;
		CeilData = ECeilingSocket::None;
		PropData = EPropSocket::None;
	}
	if(bRotate)
	{
		bRotate = false;
		bool next = false;
		ESocketTag tmpTag;
		ESocketCategory tmpCat;
		uint8 tmpVal;
		for(int i = 1; i < 6; i += i%3)
		{
			
			if(next)
			{
				if(Rules.Contains(static_cast<ESocketDirection>(i)))
				{
					UPrototypeSocketData * tmp =
                        Rules[static_cast<ESocketDirection>(i)];
					
					ESocketTag swapTag = tmp->Tag;
					ESocketCategory swapCat = tmp->Type;
					uint8 swapVal = tmp->Value;
					tmp->Tag = tmpTag;
					tmp->Type =tmpCat;
					tmp->Value = tmpVal;
					tmpTag = swapTag;
					tmpCat = swapCat;
					tmpVal = swapVal;
					
				}
				else
				{
					UPrototypeSocketData * tmp =
						Cast<UPrototypeSocketData>(GetComponentsByClass(UPrototypeSocketData::StaticClass())[i]);
					tmp->Tag = tmpTag;
					tmp->Type =tmpCat;
					tmp->Value = tmpVal;
					Rules.Add(static_cast<ESocketDirection>(i), tmp);
					next = false;
				}
			}
			else
			{
				if(Rules.Contains(static_cast<ESocketDirection>(i)))
				{
					UPrototypeSocketData * tmp =
                        Rules[static_cast<ESocketDirection>(i)];
					tmpTag = tmp->Tag;
					tmpCat = tmp->Type;
					tmpVal = tmp->Value;
					next = true;
					Rules.Remove(static_cast<ESocketDirection>(i));
				}
			}
		}
		if(next)
		{
			UPrototypeSocketData * tmp =
                        Cast<UPrototypeSocketData>(GetComponentsByClass(UPrototypeSocketData::StaticClass())[1]);
			tmp->Tag = tmpTag;
			tmp->Type =tmpCat;
			tmp->Value = tmpVal;
			Rules.Add(static_cast<ESocketDirection>(1), tmp);
		}if(Rules.Contains(ESocketDirection::Top))
		{
			UPrototypeSocketData * tmp =
                Rules[ESocketDirection::Top];
			if(tmp->Type == ESocketCategory::FloorCeilingSocket || tmp->Type == ESocketCategory::FloorPropSocket)
				if(static_cast<EWallSocket>(tmp->Value) != EWallSocket::None)
					tmp->Value = ((tmp->Value+1)%8)+1;
		}
		if(Rules.Contains(ESocketDirection::Bottom))
		{
			UPrototypeSocketData * tmp =
                Rules[ESocketDirection::Bottom];
		if(static_cast<EWallSocket>(tmp->Value) != EWallSocket::None)
			if(tmp->Type == ESocketCategory::FloorCeilingSocket || tmp->Type == ESocketCategory::FloorPropSocket)
				tmp->Value = ((tmp->Value+1)%8)+1;
		}
	}
	if(bMirror)
	{
		bMirror = false;
		if(Rules.Contains(ESocketDirection::East))
		{
			if(Rules.Contains(ESocketDirection::West))
			{
				UPrototypeSocketData * tmp =
              Rules[ESocketDirection::East];
				UPrototypeSocketData * tmp2 =
                    Rules[ESocketDirection::West];
					
				ESocketTag swapTag = tmp->Tag;
				ESocketCategory swapCat = tmp->Type;
				uint8 swapVal = tmp->Value;
				tmp->Tag = tmp2->Tag;
				tmp->Type = tmp2->Type;
				tmp->Value = tmp2->Value;
				tmp2->Tag = swapTag;
				tmp2->Type = swapCat;
				tmp2->Value = swapVal;
			}
			else
			{
				UPrototypeSocketData * tmp =
					Rules[ESocketDirection::East];
				UPrototypeSocketData * tmp2 = Cast<UPrototypeSocketData>(
					GetComponentsByClass(UPrototypeSocketData::StaticClass())[static_cast<uint8>(ESocketDirection::West)]);
				
				tmp2->Tag = tmp->Tag;
				tmp2->Type = tmp->Type;
				tmp2->Value = tmp->Value;
				Rules.Remove(ESocketDirection::East);
				Rules.Add(ESocketDirection::West, tmp2);
			}
		}
		else if(Rules.Contains(ESocketDirection::West)) {
			UPrototypeSocketData * tmp =
                Rules[ESocketDirection::West];
			UPrototypeSocketData * tmp2 = Cast<UPrototypeSocketData>(
                GetComponentsByClass(UPrototypeSocketData::StaticClass())[static_cast<uint8>(ESocketDirection::East)]);
				
			tmp2->Tag = tmp->Tag;
			tmp2->Type = tmp->Type;
			tmp2->Value = tmp->Value;
			Rules.Remove(ESocketDirection::West);
			Rules.Add(ESocketDirection::East, tmp2);
		}
		
		if(Rules.Contains(ESocketDirection::Top))
		{
			UPrototypeSocketData * tmp =
                Rules[ESocketDirection::Top];
			if(tmp->Type == ESocketCategory::FloorCeilingSocket || tmp->Type == ESocketCategory::FloorPropSocket)
			{
				switch ((EWallSocket)tmp->Value)
				{
				case EWallSocket::Northeast:
					tmp->Value = static_cast<uint8>(EWallSocket::Northwest);
					break;
				case EWallSocket::Northwest:
					tmp->Value = static_cast<uint8>(EWallSocket::Northeast);
					break;
				case EWallSocket::East:
					tmp->Value = static_cast<uint8>(EWallSocket::West);
					break;
				case EWallSocket::West:
					tmp->Value = static_cast<uint8>(EWallSocket::East);
					break;
				case EWallSocket::Southeast:
					tmp->Value = static_cast<uint8>(EWallSocket::Southwest);
					break;
				case EWallSocket::Southwest:
					tmp->Value = static_cast<uint8>(EWallSocket::Southeast);
					break;
				}
			}
		}
		if(Rules.Contains(ESocketDirection::Bottom))
		{
			UPrototypeSocketData * tmp =
                Rules[ESocketDirection::Bottom];
			if(tmp->Type == ESocketCategory::FloorCeilingSocket || tmp->Type == ESocketCategory::FloorPropSocket)
			{
				switch ((EWallSocket)tmp->Value)
				{
				case EWallSocket::Northeast:
					tmp->Value = static_cast<uint8>(EWallSocket::Northwest);
					break;
				case EWallSocket::Northwest:
					tmp->Value = static_cast<uint8>(EWallSocket::Northeast);
					break;
				case EWallSocket::East:
					tmp->Value = static_cast<uint8>(EWallSocket::West);
					break;
				case EWallSocket::West:
					tmp->Value = static_cast<uint8>(EWallSocket::East);
					break;
				case EWallSocket::Southeast:
					tmp->Value = static_cast<uint8>(EWallSocket::Southwest);
					break;
				case EWallSocket::Southwest:
					tmp->Value = static_cast<uint8>(EWallSocket::Southeast);
					break;
				}
			}	
		}
		if(Rules.Contains(ESocketDirection::North))
		{
			UPrototypeSocketData * tmp =
                Rules[ESocketDirection::North];
			if(tmp->Type == ESocketCategory::FloorSocket || tmp->Type == ESocketCategory::CeilingSocket)
			{
				if(static_cast<EFloorSocket>(tmp->Value) == EFloorSocket::LeftWall)
					tmp->Value = static_cast<uint8>(EFloorSocket::RightWall);
				else if(static_cast<EFloorSocket>(tmp->Value) == EFloorSocket::RightWall)
					tmp->Value = static_cast<uint8>(EFloorSocket::LeftWall);
			}
		}
		if(Rules.Contains(ESocketDirection::South))
		{
			UPrototypeSocketData * tmp =
                Rules[ESocketDirection::South];
			if(tmp->Type == ESocketCategory::FloorSocket || tmp->Type == ESocketCategory::CeilingSocket)
			{
				if(static_cast<EFloorSocket>(tmp->Value) == EFloorSocket::LeftWall)
					tmp->Value = static_cast<uint8>(EFloorSocket::RightWall);
				else if(static_cast<EFloorSocket>(tmp->Value) == EFloorSocket::RightWall)
					tmp->Value = static_cast<uint8>(EFloorSocket::LeftWall);
			}
		}
	}
	if(bAllAll)
	{
		for(int i = 1; i < 6; i += i%3)
		{
			UPrototypeSocketData * tmp =
                        Cast<UPrototypeSocketData>(GetComponentsByClass(UPrototypeSocketData::StaticClass())[i]);
			tmp->Tag = ESocketTag::All;
		}
	}
}
#endif