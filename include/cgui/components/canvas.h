#pragma once
#include "component.h"
#include "../utils/string.h"
#include <memory>
#include <vector>

class canvas : public component
{
public:
    canvas(int32_t width, int32_t height);

    size_t getWidth() const override;
    size_t getHeight() const override;
    std::vector<cgui::string> getData() const override;

    void clear();
    void resize(int32_t w, int32_t h);
    void set(int32_t x, int32_t y, const component& src);
    void set(int32_t x, int32_t y, std::shared_ptr<const component> src);

private:
    int32_t width;
    int32_t height;
    std::vector<cgui::string> data;
};
