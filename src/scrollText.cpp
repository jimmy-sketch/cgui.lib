#include "cgui/components/scrollText.h"
#include "cgui/utils/config.h"

scrollText::scrollText(size_t height, size_t width)
{
    this->height = height;
    this->width = width;
}

size_t scrollText::getWidth() const
{
    return this->width;
}

size_t scrollText::getHeight() const
{
    return this->height;
}

std::vector<cgui::string> scrollText::getData() const
{
    std::queue<cgui::string> tmp = this->lineBufs;
    std::vector<cgui::string> tgtRet;
    // 高度不足：空行补齐
    if (lineBufs.size() < height)
    {
        for (int i = 1; i <= height - lineBufs.size(); i++)
            tgtRet.emplace_back(cgui::string(width, cgui::getPaddingChar()));
    }
    while (!tmp.empty())
    {
        // 对字符串进行补齐
        cgui::string tmpLine = tmp.front();
        if (tmpLine.getWidth() < this->width)
            tmpLine.append(cgui::string(this->width - tmpLine.getWidth(), cgui::getPaddingChar()));

        tgtRet.emplace_back(tmp.front());
        tmp.pop();
    }
    return tgtRet;
}

void scrollText::addLine(cgui::string lineBuf)
{
    // 先对字符串进行截断处理
    lineBuf.take(width);
    // 接着加入队列中
    lineBufs.push(lineBuf);
    // 如果超高则拿掉最上面一个
    if (lineBufs.size() > height)
        lineBufs.pop();
    return;
}

void scrollText::clearAll()
{
    while (!lineBufs.empty())
        lineBufs.pop();
    return;
}