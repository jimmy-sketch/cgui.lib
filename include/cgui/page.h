#pragma once
#include "utils/utils.h"
#include "components/canvas.h"
#include "utils/pos.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

class page
{
public:
    size_t getWidth() const;
    size_t getHeight() const;
    void display();

    void set(int32_t x, int32_t y, int32_t z, std::shared_ptr<component> src);
    void erase(int32_t x, int32_t y, int32_t z);
    void clear();
    std::shared_ptr<component> get(int32_t x, int32_t y, int32_t z);
    std::shared_ptr<const component> get(int32_t x, int32_t y, int32_t z) const;

private:
    std::map<cgui::Pos, std::shared_ptr<component>> components;
    canvas _canvas = canvas(0, 0);
};