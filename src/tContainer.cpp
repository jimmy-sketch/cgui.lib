#include <cgui/components/tContainer.h>
#include <cgui/utils/config.h>

size_t tContainer::getWidth() const
{
    std::map<int, size_t> lineWidthList;
    for (auto& [p, c] : components) {
        lineWidthList[p.col] = std::max(lineWidthList[p.col], c->getWidth());
    }
    size_t ret = 0;
    for (auto&& [_, w] : lineWidthList) {
        ret += w;
    }
    return ret;
}

size_t tContainer::getHeight() const
{
    std::map<int, size_t> lineHeightList;
    for (auto& [p, c] : components) {
        lineHeightList[p.row] = std::max(lineHeightList[p.row], c->getHeight());
    }
    size_t ret = 0;
    for (auto&& [_, h] : lineHeightList) {
        ret += h;
    }
    return ret;
}

std::vector<cgui::string> tContainer::getData() const
{
    std::vector<cgui::string> lines(getHeight());
    std::map<int, size_t> lineWidthList;
    std::map<int, size_t> lineHeightList;
    for (auto& [p, c] : components) {
        lineWidthList[p.col] = std::max(lineWidthList[p.col], c->getWidth());
        lineHeightList[p.row] = std::max(lineHeightList[p.row], c->getHeight());
    }
    for (auto&& [pos, c] : components) {
        size_t height = c->getHeight();
        size_t width = c->getWidth();
        auto data = c->getData();

        size_t lineHeight = lineHeightList.at(pos.row);
        size_t lineWidth = lineWidthList.at(pos.col);

        size_t yOffset = 0;
        for (int i = 0; i < pos.row; ++i) {
            if (lineHeightList.find(i) != lineHeightList.end()) {
                yOffset += lineHeightList.at(i);
            }
        }

        for (size_t i = 0; i < height; ++i) {
            lines[yOffset + i].appendDirectly(data[i]);
            lines[yOffset + i] += cgui::string(lineWidth - data[i].getWidth(), cgui::getPaddingChar());
        }
        for (size_t i = height; i < lineHeight; ++i) {
            lines[yOffset + i] += cgui::string(lineWidth, cgui::getPaddingChar());
        }
    }
    return lines;
}

void tContainer::set(cgui::logicPos pos, std::shared_ptr<component> src)
{
    components[pos] = src;
}

void tContainer::erase(cgui::logicPos pos)
{
    components.erase(pos);
}

void tContainer::clear()
{
    components.clear();
}
