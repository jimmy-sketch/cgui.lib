#include <cgui/components/canvas.h>
#include <cgui/utils/config.h>

canvas::canvas(int32_t width, int32_t height)
    : width(width), height(height), data(height, cgui::string(width, cgui::getPaddingChar()))
{}

size_t canvas::getWidth() const
{
    return width;
}

size_t canvas::getHeight() const
{
    return height;
}

std::vector<cgui::string> canvas::getData() const
{
    return data;
}

void canvas::clear()
{
    data.assign(height, cgui::string(width, cgui::getPaddingChar()));
}

void canvas::resize(int32_t w, int32_t h)
{
    width = w;
    height = h;
    data.resize(height, cgui::string(width, cgui::getPaddingChar()));
}

void canvas::set(int32_t x, int32_t y, const component& src)
{
    if (x >= width || y >= height) {
        return;
    }

    auto srcHeight = src.getHeight();
    auto srcData = src.getData();

    if (srcHeight + y > height) {
        srcHeight = height - (y >= 0 ? y : 0);
    }

    if (x >= 0) {
        for (int32_t i = (y < 0 ? -y : 0); i < srcHeight; ++i) {
            data[static_cast<size_t>(i + y)] = data[static_cast<size_t>(i + y)].replace(x, srcData[i]);
        }
    }
    else {
        for (int32_t i = (y < 0 ? -y : 0); i < srcHeight; ++i) {
            auto srcLineWidth = srcData[i].getWidth();
            if (x + srcLineWidth < 0) {
                continue;
            }
            data[static_cast<size_t>(i + y)] = data[static_cast<size_t>(i + y)].replace(0, srcData[i].take(-x, srcLineWidth + x));
        }
    }
}

void canvas::set(int32_t x, int32_t y, std::shared_ptr<const component> src)
{
    set(x, y, *src);
}
