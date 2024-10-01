// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
enum class CellSideTag {
	North,
	South,
	East,
	West,
	Center
};

class Cell {
public:
	FVector Position;
	CellSideTag Side;

	Cell(FVector InPosition, CellSideTag InSide)
		: Position(InPosition), Side(InSide) {}
};
