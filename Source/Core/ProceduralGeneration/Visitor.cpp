// Fill out your copyright notice in the Description page of Project Settings.


#include "Visitor.h"

bool FVisitor::Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory)
{
	new(Result)FString(FPaths::GetCleanFilename(FilenameOrDirectory));

	return true;
}
