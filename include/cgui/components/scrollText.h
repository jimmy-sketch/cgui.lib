#pragma once
#include <queue>
#include "component.h"
#include "string.h"

class scrollText : public component
{
public:
    scrollText(size_t height, size_t weight);

    size_t getWidth() const override;
    size_t getHeight() const override;
    std::vector<cgui::string> getData() const override;

    void addLine(cgui::string lineBuf);
    void clearAll();

private:
    std::queue<cgui::string> lineBufs;
    size_t height,width;
};