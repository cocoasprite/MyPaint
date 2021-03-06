#include "toolRectangle.h"
#include <event_manager/eventmanager.h>
#include <mypaint.h>
#include <image/image.h>
#include <glm/common.hpp>

namespace mp
{

CRectangleTool::CRectangleTool(CMyPaint *ow) :
    CTool(ow)
{
    name = "Rectangle";
}

void CRectangleTool::Begin(const glm::ivec2 &mousePos)
{
    started = true;
    startPosition = mousePos;
    prevMousePosition = mousePos;
}

void CRectangleTool::GetDrawBBox(const glm::ivec2 &newPos, int &xStart, int &xEnd, int &yStart, int &yEnd) const
{
    xStart = glm::min(startPosition.x, newPos.x);
    xEnd   = glm::max(startPosition.x, newPos.x);
    yStart = glm::min(startPosition.y, newPos.y);
    yEnd   = glm::max(startPosition.y, newPos.y);
}

void CRectangleTool::Update(const glm::ivec2 &mousePos)
{
    if(!started)
    {
        return;
    }

    unsigned brushSize = owner->GetBrushSize();
    int xStart, xEnd, yStart, yEnd;
    GetDrawBBox(prevMousePosition, xStart, xEnd, yStart, yEnd);
    xStart -= brushSize;
    xEnd   += brushSize;
    yStart -= brushSize;
    yEnd   += brushSize;

    CImage& img = *(owner->GetCurrentImage());
    CImage* prevImg = owner->GetPreviousImage();

    if(!prevImg)
    {
        return;
    }

    img.CopyRectFrom(*prevImg, glm::ivec2(xStart, yStart), glm::ivec2(xEnd, yEnd));

    DrawRectTo(mousePos);

    CEventManager::GetInstance().SendEvent(CEvent("ImageUpdated"));

    prevMousePosition = mousePos;
}

void CRectangleTool::End(const glm::ivec2 &mousePos)
{
    started = false;
}

void CRectangleTool::DrawRectTo(const glm::ivec2 &mousePos)
{
    CImage& img = *(owner->GetCurrentImage());

    float brushSize = owner->GetBrushSize() * 0.5f;
    glm::uvec3 brushColor = owner->GetBrushColor();

    int xStart, xEnd, yStart, yEnd;
    GetDrawBBox(mousePos, xStart, xEnd, yStart, yEnd);

    for(int x = xStart - brushSize; x <= xEnd + brushSize; x++)
    for(int y = yEnd - brushSize; y <= yEnd + brushSize; y++)
    {
        img.SetPixelColor(glm::uvec2(x, y), brushColor);
    }

    for(int x = xStart - brushSize; x <= xEnd + brushSize; x++)
    for(int y = yStart - brushSize; y <= yStart + brushSize; y++)
    {
        img.SetPixelColor(glm::uvec2(x, y), brushColor);
    }

    for(int y = yStart - brushSize; y <= yEnd + brushSize; y++)
    for(int x = xEnd - brushSize; x <= xEnd + brushSize; x++)
    {
        img.SetPixelColor(glm::uvec2(x, y), brushColor);
    }

    for(int y = yStart - brushSize; y <= yEnd + brushSize; y++)
    for(int x = xStart - brushSize; x <= xStart + brushSize; x++)
    {
        img.SetPixelColor(glm::uvec2(x, y), brushColor);
    }
}

}
