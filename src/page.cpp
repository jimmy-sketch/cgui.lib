#include <cgui/page.h>
#include <iostream>
#include <cgui/utils/utils.h>
#include <cgui/utils/config.h>

size_t page::getWidth() const
{
    return _canvas.getWidth();
}

size_t page::getHeight() const
{
    return _canvas.getHeight();
}

void page::display()
{
    if (_canvas.getWidth() != terminalWidth() || _canvas.getHeight() != terminalHeight())
    {
        terminalClear();
        _canvas.resize(terminalWidth(), terminalHeight());
    }

    _canvas.clear();
    for (auto &&[p, c] : components)
    {
        _canvas.set(p.col, p.row, c);
    }

    printf("\x1B[0;0H");
    for (auto &&line : _canvas.getData())
    {
        printf("%s", line.getData());
    }
}

void page::set(int32_t x, int32_t y, int32_t z, std::shared_ptr<component> src)
{
    components[{y, x, z}] = src;
}

void page::erase(int32_t x, int32_t y, int32_t z)
{
    components.erase({y, x, z});
}

void page::clear()
{
    components.clear();
}

std::shared_ptr<component> page::get(int32_t x, int32_t y, int32_t z)
{
    return components.at({x, y, z});
}

std::shared_ptr<const component> page::get(int32_t x, int32_t y, int32_t z) const
{
    return components.at({x, y, z});
}
