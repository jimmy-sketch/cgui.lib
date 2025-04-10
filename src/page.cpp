#include <cgui/page.h>
#include <iostream>
#include <cgui/utils/config.h>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>

static int terminalWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    return 0;
}
static int terminalHeight() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
    return 0;
}
static void terminalClear() {
    system("cls");
}

#else
#include <sys/ioctl.h>  // for winsize, ioctl, TIOCGWINSZ
#include <unistd.h>     // for STDOUT_FILENO

static int terminalWidth() {
    winsize w{};
    const int status = ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}
static int terminalHeight() {
    winsize w{};
    const int status = ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_row;
}
static void terminalClear() {
    system("clear");
}
#endif

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
    if (_canvas.getWidth() != terminalWidth() || _canvas.getHeight() != terminalHeight()) {
        terminalClear();
        _canvas.resize(terminalWidth(), terminalHeight());
    }

    _canvas.clear();
    for (auto&& [p, c] : components) {
        _canvas.set(p.col, p.row, c);
    }

    printf("\x1B[0;0H");
    for (auto&& line : _canvas.getData()) {
        printf("%s", line.getData());
    }
}

void page::set(int32_t x, int32_t y, int32_t z, std::shared_ptr<component> src)
{
    components[{ y, x, z }] = src;
}

void page::erase(int32_t x, int32_t y, int32_t z)
{
    components.erase({ y, x, z });
}

void page::clear()
{
    components.clear();
}

std::shared_ptr<component> page::get(int32_t x, int32_t y, int32_t z)
{
    return components.at({ x, y, z });
}

std::shared_ptr<const component> page::get(int32_t x, int32_t y, int32_t z) const
{
    return components.at({ x, y, z });
}