#pragma once
#include <string>

namespace cgui {

// 返回字符串中第一个UTF-8字符/ANSI转义序列的字节数
size_t charSize(const char* src);
// 返回字符串中第一个UTF-8字符/ANSI转义序列的宽度
size_t charWidth(const char* src);

// 一个没有\n \t，结尾会自动添加“恢复默认颜色”的字符串
class string {
public:
    string();
    string(const char* in);
    string(const char* first, const char* last);
    string(std::string_view in);
    string(size_t count, char c);

    // 字符串占用的字节数
    size_t getSize() const;
    // 字符串的宽度
    size_t getWidth() const;
    // 字符串中，字符的数量
    size_t getCount() const;
    // 字符串中，可见字符的数量
    size_t getVisibleCharCount() const;
    // C风格字符串
    const char* getData() const;
    
    // 追加字符串，追加的字符串会移除结尾的“恢复默认颜色”
    void append(const string& other);
    // 追加一个字符
    void pushBack(char other);
    // 在第n个可见字符前，插入字符串，插入的字符串会移除结尾的“恢复默认颜色”
    void insert(size_t n, const string& other);
    // 在第n个可见字符前，插入一个字符
    void insert(size_t n, char c);
    // 删除字符串中的可见字符
    // void erase(size_t first, size_t last);
    // 追加字符串，插入的字符串不会移除结尾的“恢复默认颜色”
    void appendDirectly(const string& other);
    // 删除字符串中的字符
    void eraseDirectly(size_t index, size_t n);
    // 从头开始，获取一个宽度为n的子字符串
    // 如果宽度w处截断了某个UTF-8字符，会把这个字符转化为一些peddingChar，保证宽度w
    string take(size_t w) const;
    // 从index宽开始，获取一个宽度为n的子字符串
    string take(size_t index, size_t w) const;
    // 从头开始，获取一个宽度为n的子字符串
    // 如果宽度w处截断了某个UTF-8字符，会补全这个字符，尽管这样会超过宽度w
    string takeComplete(size_t w) const;
    // 从index宽开始，用other的内容替换this的内容，且保证原宽不变
    string replace(size_t index, const string& other) const;

    // 追加颜色
    void pushBackDefaultRGB();
    void pushBackRGB(int r, int g, int b);
    void pushBackBackgroundRGB(int r, int g, int b);
    // 在第n个可见字符前，插入颜色
    void insertDefaultRGB(size_t n);
    void insertRGB(size_t n, int r, int g, int b);
    void insertBackgroundRGB(size_t n, int r, int g, int b);
    // 设置第n个可见字符的颜色
    void setDefaultRGB(size_t n);
    void setRGB(size_t n, int r, int g, int b);
    void setBackgroundRGB(size_t n, int r, int g, int b);
    
    // 与append相同，会移除结尾的“恢复默认颜色”
    string operator+(const string& other);
    string operator+(char other);
    string& operator+=(const string& other);
    string& operator+=(char other);
    // 赋值给当前字符串
    string& operator=(const string& other);
    string& operator=(std::string_view other);

    // 迭代器
    struct iterator {
        iterator& operator++();
        bool operator==(const iterator& other) const;
        char* operator*();
        operator char* ();

        char* p; // => cgui::string::bytes
    };
    struct constIterator {
        constIterator& operator++();
        bool operator==(const constIterator& other) const;
        const char* operator*() const;
        operator const char* () const;

        const char* p; // => cgui::string::bytes
    };
    iterator begin();
    iterator end();
    constIterator begin() const;
    constIterator end() const;

private:
    std::string bytes;
    size_t width = 0;
    size_t count = 0;
    size_t visibleCharCount = 0;

    // 返回末尾的“恢复默认颜色”的前面一个位置
    size_t pushBackPos() const;
    // 移除\n \t
    void removeBadChar();
    // 计算字符串的属性
    void calculateProperties();
    // rgb转换为ANSI转义序列
    std::string colorAnsiEscapeCode(int mod, int r, int g, int b);
    // 插入颜色
    void __insertRGB(size_t n, const string& other);
    // 插入颜色，并清理掉无用的颜色
    void __setRGB(size_t n, const string& other);
};
string operator+(std::string_view lhs, string& rhs);
string operator+(std::string_view lhs, string&& rhs);

}