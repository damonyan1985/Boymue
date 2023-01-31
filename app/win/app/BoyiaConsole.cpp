#include "BoyiaConsole.h"
#include <iostream>
#include <string>
#include <windows.h>

namespace yanbo {
BoyiaConsole::BoyiaConsole()
{
    AllocConsole();
    SetConsoleTitle(L"BoyiaConsole");

    freopen("conin$", "r+t", stdin);
    freopen("conout$", "w+t", stdout);
    freopen("conout$", "w+t", stderr);
}

BoyiaConsole::~BoyiaConsole()
{
    fclose(stderr);
    fclose(stdout);
    fclose(stdin);

    FreeConsole();
}
}