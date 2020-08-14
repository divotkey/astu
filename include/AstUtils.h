/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 * 
 *----------------------------------------------------------------------------
 * Meanings of ASTU: Slightly melted butter (Hinduism / Religious Law), 
 * Let it be / Be it so (Sanskrit), City (Ancient Greek)
 *----------------------------------------------------------------------------
 */

#pragma once

#include "AstUtils0.h"

// TODO move the following functions to API-Level 0 or remove entirely if not needed.

// Graphics Functions
void WriteImage(const char* filename);
void ReadImage(const char* filename);
int GetImageWidth();
int GetImageHeight();
void ClearImage();
void SetDrawColor(int r, int g, int b, int a = 255);
void SetClearColor(int r, int g, int b);
void DrawCircle(double x, double y, double r);
void DrawLine(double x0, double y0, double x1, double y1, double w = 1);
void WriteImage(const char* filename);
void ReadImage(const char* filename);

// Turtle Graphics
void TurtlePenUp(); 
void TurtlePenDown();
void TurtlePenWidth(double w);
void TurtleSet(double x, double y);
void TurtleTurnLeft(double phi);
void TurtleTurnRight(double phi);
void TurtleForward(double distance);
void TurtleGo(double x, double y);
void TurtleCenter();
void TurtleColor(int r, int g, int b, int a = 255);