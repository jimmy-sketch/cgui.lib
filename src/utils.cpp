#include <cgui/utils/utils.h>
#include <iostream>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>

static int terminalWidth()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
    {
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    return 0;
}
static int terminalHeight()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
    {
        return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
    return 0;
}
static void terminalClear()
{
    system("cls");
}

#else
#include <sys/ioctl.h> // for winsize, ioctl, TIOCGWINSZ
#include <unistd.h>    // for STDOUT_FILENO

static int terminalWidth()
{
    winsize w{};
    const int status = ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}
static int terminalHeight()
{
    winsize w{};
    const int status = ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_row;
}
static void terminalClear()
{
    system("clear");
}
#endif