/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#define _USE_MATH_DEFINES
 
#include <cmath>
#include <iostream>
#include <chrono>
#include <memory>
#include "math/Random.h"
#include "gfx/Image.h"
#include "gfx/BmpCodec.h"
#include "gfx/Pattern.h"
#include "gfx/PatternRenderer.h"
#include "gfx/Quadtree.h"
#include "gfx/Turtle.h"

using namespace std;
using namespace astu;

// Constants
const double TO_RADIANS = M_PI / 180.0;
const double TO_DEGREES = 180.0 / M_PI;

// Globals
std::chrono::time_point<std::chrono::steady_clock> startTime;
std::chrono::time_point<std::chrono::steady_clock> stopTime;
std::unique_ptr<astu::Image> image;
Color drawColor(1, 1, 1);
Color clearColor(0, 0, 0);

std::shared_ptr<UnionPattern> rootPattern = std::make_shared<UnionPattern>();
std::shared_ptr<Quadtree> quadTree = std::make_shared<Quadtree>(5, 5);
// std::unique_ptr<IPatternRenderer> patternRenderer = std::make_unique<SimplePatternRenderer>();
std::unique_ptr<IPatternRenderer> patternRenderer = std::make_unique<AntiAlisaingPatternRenderer>();
Turtle turtle;

void SayHello()
{
    cout << "Hello AST World :-)" << endl;
}

/////////////////////////////////////////////////
/////// Math Functions
/////////////////////////////////////////////////

double ToRadians(double deg)
{
    return deg * TO_RADIANS;
}

double ToDegrees(double rad)
{
    return rad * TO_DEGREES;
}

double GetRandomDouble()
{
    return Random::GetInstance().NextDouble();
}

/////////////////////////////////////////////////
/////// Timer functions
/////////////////////////////////////////////////

void StartTimer()
{
    startTime = stopTime = std::chrono::steady_clock::now();
}

void StopTimer()
{
    stopTime = chrono::steady_clock::now();
}

int GetMilliseconds()
{
    auto dt = chrono::duration_cast<chrono::milliseconds>(stopTime - startTime).count();
    return static_cast<int>(dt);
}

/////////////////////////////////////////////////
/////// Graphics functions
/////////////////////////////////////////////////

void ValidateImage()
{
    if (!image) {
        throw std::logic_error("image not created");
    }
}

void CreateImage(int x, int y)
{
    image = std::make_unique<Image>(x, y);
    ClearImage();
    turtle.Reset();
    TurtleCenter();
}

void ClearImage()
{
    rootPattern->Clear();
    rootPattern->Add(std::make_shared<UnicolorPattern>(clearColor));
    rootPattern->Add(quadTree);
}

void SetDrawColor(int r, int g, int b, int a)
{
    drawColor.Set(r, g, b, a);
}

void SetClearColor(int r, int g, int b)
{
    clearColor.Set(r, g, b);
}

void SetPixel(int x, int y)
{
    ValidateImage();
    image->SetPixel(x, y, drawColor);
}

void WriteImage(const char* filename)
{
    ValidateImage();

    // if (!rootPattern->IsEmpty()) {

    //     // debug visualization of bounding boxes
    //     // std::vector<std::shared_ptr<Pattern>> boxes;
    //     // for (auto pattern : unionPattern) {
    //     //     auto box = pattern->GetBoundingBox();
    //     //     if (!box.IsInfinite()) {
    //     //         // cout << "box center = " << box.GetCenter() << endl;
    //     //         // cout << "box size = " << box.GetWidth() << ", " << box.GetHeight() << endl;
    //     //         auto rect = std::make_shared<RectanglePattern>(box.GetWidth(), box.GetHeight());
    //     //         rect->Translate(box.GetCenter());
    //     //         rect->SetPattern(std::make_shared<UnicolorPattern>(Color(1, 0, 0, 0.5)));
    //     //         boxes.push_back(rect);
    //     //     }
    //     // }
    //     // for (auto box : boxes) {
    //     //     unionPattern.Add(box);
    //     // }
    // }
    // cout << "building quadtree.." << endl;
    quadTree->BuildTree();

    // cout << "rendering pattern.." << endl;
    patternRenderer->Render(*rootPattern, *image);
    BmpEncoder bmpEnc;
    bmpEnc.Encode(*image, filename);
}

void DrawCircle(double x, double y, double r) 
{
    auto circle = std::make_shared<CirclePattern>(r);
    circle->Translate(x, y);
    circle->SetPattern(std::make_shared<UnicolorPattern>(drawColor));
    quadTree->Add(circle);
}

void DrawLine(double x0, double y0, double x1, double y1, double w)
{
    Vector2 v(x1 - x0, y1 - y0);

    double lng = v.Length();
    if (lng <= 0) {
        return;
    }

    double a = v.Angle(Vector2(1, 0));

    auto rect = std::make_shared<RectanglePattern>(lng, w);
    rect->Translate((x1 + x0) / 2, (y1 + y0) / 2);
    rect->Rotate(-a);
    rect->SetPattern(std::make_shared<UnicolorPattern>(drawColor));
    quadTree->Add(rect);
}

void TurtleCenter()
{
    ValidateImage();
    turtle.Go(image->GetWidth() / 2.0, image->GetHeight() / 2.0);
}

void TurtleSet(double x, double y)
{
    turtle.Go(x, y);
}

void TurtleTurnLeft(double phi)
{
    turtle.TurnLeft(phi);
}

void TurtleTurnRight(double phi)
{
    turtle.TurnRight(phi);
}

void TurtleForward(double distance)
{
    if (turtle.IsPenDown()) {        
        auto p1 = turtle.GetPosition();
        turtle.Forward(distance);
        auto p2 = turtle.GetPosition();
        drawColor = turtle.GetPenColor();
        DrawLine(p1.x, p1.y, p2.x, p2.y, turtle.GetPenWidth());
    } else {
        turtle.Forward(distance);
    }
}

void TurtleGo(double x, double y)
{
    turtle.Go(x, y);
}

void TurtlePenUp()
{
    turtle.PenUp();
}

void TurtlePenDown()
{
    turtle.PenDown();
}

void TurtlePenWidth(double w)
{
    turtle.SetPenWidth(w);
}

void TurtleColor(int r, int g, int b, int a)
{
    turtle.SetPenColor(Color::CreateFromRgb(r, g, b, a));
}