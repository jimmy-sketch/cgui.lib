#include <cgui/utils/string.h>
#include <cgui/utils/config.h>
#include <wtswidth.h>

static const std::string defaultColor = "\x1b[0m";

static size_t utf8CharSize(char firstByte) 
{
    if ((firstByte & 0x80) == 0x00) return 1;
    else if ((firstByte & 0xE0) == 0xC0) return 2;
    else if ((firstByte & 0xF0) == 0xE0) return 3;
    else if ((firstByte & 0xF8) == 0xF0) return 4;
    else return 0;
}

size_t cgui::charSize(const char* src)
{
    if (!src) {
        return 0;
    }
    if (src[0] == '\x1b') {
        size_t ret = 1;
        for (; src[ret] != 0; ++ret) {
            if (src[ret] == 'm') {
                return ret + 1;
            }
        }
        return ret;
    }
    size_t ret = 0;
    for (; (ret < utf8CharSize(src[0])) && (src[ret] != 0 && src[ret] != '\x1b'); ++ret);
    return ret;
}

size_t cgui::charWidth(const char* src) 
{
    if (!src) {
        return 0;
    }
    if (src[0] == '\x1b') {
        return 0;
    }
    size_t ret = 0;
    for (; (ret < utf8CharSize(src[0])) && (src[ret] != 0 && src[ret] != '\x1b'); ++ret);
    return wts8width(src, ret);
}

cgui::string::string()
    : bytes(defaultColor)
{}

cgui::string::string(const char* in) 
    : bytes(in)
{
    bytes += defaultColor;
    removeBadChar();
    calculateProperties();
}

cgui::string::string(const char* first, const char* last)
    : bytes(first, last)
{
    bytes += defaultColor;
    removeBadChar();
    calculateProperties();
}

cgui::string::string(std::string_view in) 
    : string(in.data()) 
{}

cgui::string::string(size_t count, char c) 
    : string(std::string(count, c).data())
{}

size_t cgui::string::getSize() const
{ 
    return bytes.size();
}

size_t cgui::string::getWidth() const
{ 
    return width; 
}

size_t cgui::string::getCount() const
{
    return count;
}

size_t cgui::string::getVisibleCharCount() const
{
    return visibleCharCount;
}

const char* cgui::string::getData() const
{
    return bytes.data();
}

void cgui::string::append(const string& other)
{
    bytes.insert(pushBackPos(), other.bytes.substr(0, other.pushBackPos()));
    width += other.width;
    count += other.count;
    visibleCharCount += other.visibleCharCount;
}

void cgui::string::pushBack(char other)
{
    append(string(1, other));
}

void cgui::string::insert(size_t n, const string& other)
{
    if (n > visibleCharCount) {
        return;
    }
    // 找到第n个可见字符的位置
    auto it = begin();
    for (size_t i = 0; i < n; ) {
        if (charWidth(it) != 0) {
            ++i;
        }
        ++it;
    }
    bytes.insert(it.p - begin(), other.bytes.substr(0, other.pushBackPos()));
    width += other.width;
    count += other.count;
    visibleCharCount += other.visibleCharCount;
}

void cgui::string::insert(size_t n, char c)
{
    insert(n, string(1, c));
}

void cgui::string::appendDirectly(const string& other)
{
    bytes.insert(pushBackPos(), other.bytes);
    width += other.width;
    count += other.count;
    visibleCharCount += other.visibleCharCount;
}

void cgui::string::eraseDirectly(size_t index, size_t n)
{
    if (index > count || n == 0) {
        return;
    }
    if (index + n > count) {
        n = count - index;
    }
    // 找到first字符的位置
    auto it = begin();
    for (size_t i = 0; i < index; ++i, ++it);
    size_t firstPos = it - begin();
    // 找到last字符的位置
    for (size_t i = 0; i < n; ++i, --count, ++it) {
        if (charWidth(it) != 0) {
            --visibleCharCount;
        }
    }
    size_t lastPos = it - begin();
    // 清除
    bytes.erase(firstPos, lastPos - firstPos);
}

cgui::string cgui::string::take(size_t w) const
{
    auto it = begin();
    auto last = it;
    size_t currentWidth = 0;
    for (; (currentWidth < w) && (it != end()); ++it) {
        last = it;
        currentWidth += charWidth(it);
    }
    if (currentWidth == w) {
        return { begin().p, it.p };
    }
    else if (currentWidth > w) {
        currentWidth -= charWidth(last);
    }
    std::string ret(begin().p, last.p);
    ret += std::string(w - currentWidth, cgui::getPaddingChar());
    return { ret.data() };
}

cgui::string cgui::string::take(size_t index, size_t w) const
{
    if (index > getWidth() || w == 0) {
        return "";
    }
    auto it = begin();
    std::string ret;
    size_t cW = 0;
    for (; cW < index; cW += charWidth(it.p), ++it);
    if (cW > index) {
        ret = std::string(cW - index, ' ');
    }
    auto first = it, last = it;
    for (cW = 0; (it != end()) && (cW < w); last = it, cW += charWidth(it.p), ++it);
    if (cW == w) {
        ret += std::string(first.p, it.p);
    }
    else if (cW > w) {
        ret += std::string(first.p, last.p) + std::string(cW - w, getPaddingChar());
    }
    else if (cW < w) {
        ret += std::string(first.p, it.p) + std::string(w - cW, getPaddingChar());
    }
    return ret.data();
}

cgui::string cgui::string::replace(size_t index, const string& other) const
{
    if (index > getWidth()) {
        return "";
    }
    auto width = getWidth();
    auto otherWidth = other.getWidth();
    auto before = take(0, index);
    if (otherWidth + index > width) {
        return before + other.take(0, width - index);
    }
    else {
        auto after = take(index + otherWidth, width - index - otherWidth);
        return before + other + after;
    }
}

cgui::string cgui::string::takeComplete(size_t w) const
{
    auto it = begin();
    auto last = it;
    size_t currentWidth = 0;
    for (; (currentWidth < w) && (it != end()); ++it) {
        last = it;
        currentWidth += charWidth(it);
    }
    if (currentWidth >= w) {
        return { begin().p, it.p };
    }
    std::string ret(begin().p, last.p);
    ret += std::string(w - currentWidth, cgui::getPaddingChar());
    return { ret.data() };
}

void cgui::string::pushBackDefaultRGB() 
{
    append(defaultColor.data());
}

void cgui::string::pushBackRGB(int r, int g, int b) 
{
    append(colorAnsiEscapeCode(38, r, g, b).data());
}

void cgui::string::pushBackBackgroundRGB(int r, int g, int b)
{
    append(colorAnsiEscapeCode(48, r, g, b).data());
}

void cgui::string::insertDefaultRGB(size_t n)
{
    __insertRGB(n, defaultColor.data());
}

void cgui::string::insertRGB(size_t n, int r, int g, int b)
{
    __insertRGB(n, colorAnsiEscapeCode(38, r, g, b).data());
}

void cgui::string::insertBackgroundRGB(size_t n, int r, int g, int b)
{
    __insertRGB(n, colorAnsiEscapeCode(48, r, g, b).data());
}

void cgui::string::setDefaultRGB(size_t n)
{
    __setRGB(n, defaultColor.data());
}

void cgui::string::setRGB(size_t n, int r, int g, int b)
{
    __setRGB(n, colorAnsiEscapeCode(38, r, g, b).data());
}

void cgui::string::setBackgroundRGB(size_t n, int r, int g, int b)
{
    __setRGB(n, colorAnsiEscapeCode(48, r, g, b).data());
}

cgui::string cgui::string::operator+(const string& other)
{
    string ret = *this;
    ret.append(other);
    return ret;
}

cgui::string cgui::string::operator+(char other)
{
    string ret = *this;
    ret.pushBack(other);
    return ret;
}

cgui::string& cgui::string::operator+=(const string& other) {
    append(other);
    return *this;
}

cgui::string& cgui::string::operator+=(char other)
{
    pushBack(other);
    return *this;
}

cgui::string& cgui::string::operator=(const string& other)
{
    bytes = other.bytes;
    width = other.width;
    return *this;
}

cgui::string& cgui::string::operator=(std::string_view other)
{
    return operator=(string(other));
}

cgui::string::iterator cgui::string::begin()
{
    return { bytes.data() };
}

cgui::string::iterator cgui::string::end()
{
    return { bytes.data() + bytes.size() };
}

cgui::string::constIterator cgui::string::begin() const
{
    return { bytes.data() };
}

cgui::string::constIterator cgui::string::end() const
{
    return { bytes.data() + bytes.size() };
}

size_t cgui::string::pushBackPos() const
{
     return bytes.end() - bytes.begin() - defaultColor.size();
}

void cgui::string::removeBadChar()
{
    for (auto& byte : bytes) {
        if (byte == '\n' || byte == '\t') {
            byte = ' ';
        }
    }
}

void cgui::string::calculateProperties()
{
    width = 0;
    count = 0;
    visibleCharCount = 0;
    for (auto c : *this) {
        auto cWidth = charWidth(c);
        width += cWidth;
        ++count;
        if (cWidth != 0) {
            ++visibleCharCount;
        }
    }
}

std::string cgui::string::colorAnsiEscapeCode(int mod, int r, int g, int b)
{
    return "\x1b[" + std::to_string(mod) + ";2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
}

void cgui::string::__insertRGB(size_t n, const string& other)
{
    if (n > visibleCharCount) {
        return;
    }
    // 找到第n个可见字符的位置
    auto it = begin();
    for (size_t i = 0; i < n; ++it) {
        if (charWidth(it) != 0) {
            ++i;
        }
    }
    // 跳过后面的彩色转义符
    while (charWidth(it) == 0) {
        ++it;
    }
    bytes.insert(it - begin(), other.bytes.substr(0, other.pushBackPos()));
    width += other.width;
    count += other.count;
    visibleCharCount += other.visibleCharCount;
}

void cgui::string::__setRGB(size_t n, const string& other)
{
    if (n > visibleCharCount) {
        return;
    }
    // 找到第n个可见字符的位置
    auto it = begin();
    size_t eraseIndex = 0;
    for (size_t i = 0; i < n; ++it, ++eraseIndex) {
        if (charWidth(it) != 0) {
            ++i;
        }
    }
    // 跳过后面的彩色转义符
    size_t eraseCount = 0;
    while (charWidth(it) == 0) {
        ++eraseCount;
        ++it;
    }
    bytes.insert(it - begin(), other.bytes.substr(0, other.pushBackPos()));
    width += other.width;
    count += other.count;
    visibleCharCount += other.visibleCharCount;
    // 删除无用的彩色转义符
    eraseDirectly(eraseIndex, eraseCount);
}

cgui::string cgui::operator+(std::string_view lhs, string& rhs)
{
    return string(lhs) + rhs;
}

cgui::string cgui::operator+(std::string_view lhs, string&& rhs)
{
    return string(lhs) + rhs;
}

cgui::string::iterator& cgui::string::iterator::operator++()
{
    p += charSize(p);
    return *this;
}

bool cgui::string::iterator::operator==(const iterator& other) const
{
    return p == other.p;
}

char* cgui::string::iterator::operator*()
{
    return p;
}

cgui::string::iterator::operator char* ()
{
    return p;
}

cgui::string::constIterator& cgui::string::constIterator::operator++()
{
    p += charSize(p);
    return *this;
}

bool cgui::string::constIterator::operator==(const constIterator& other) const
{
    return p == other.p;
}

const char* cgui::string::constIterator::operator*() const
{
    return p;
}

cgui::string::constIterator::operator const char* () const
{
    return p;
}