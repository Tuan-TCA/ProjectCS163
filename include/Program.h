#pragma once
#include <raylib.h>
#include <bits/stdc++.h>
#include "Variables.h"
#include "Menu.h"
#include "Page.h"

class Program
{
public:
    Menu menu;
    Page page;
    Program();
    void run();

    void draw();
    void event();
};