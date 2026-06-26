/*
 * Copyright (C) 2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "../GUI/interface.hpp"


// Part of interface to show parts of delays
class FPScounter {
 private:
    // Variables
    Uint64 prevCounter = 0;
    Uint64 avr1, avr2, avr3;  // Accumulated sums
    Uint64 count;  // Number of current cycle for avarage

    // Graphical objects
    GUI::DynamicText shower1;
    GUI::DynamicText shower2;
    GUI::DynamicText shower3;

 public:
    FPScounter(const Window& window, float X, float Y);
    void reset();

    // Update to count frames
    void start();
    void setPoint1();
    void setPoint2();
    void setPoint3();
    // Basic draw
    void blit() const;
};
