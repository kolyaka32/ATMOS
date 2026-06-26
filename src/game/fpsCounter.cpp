/*
 * Copyright (C) 2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "fpsCounter.hpp"


FPScounter::FPScounter(const Window& _window, float _X, float _Y)
: shower1(_window, _X, _Y,    {"Input: %6llu %6llu", "Ввод: %6llu %6llu"}, Height::Main, BLACK, GUI::Aligment::Left),
shower2(_window, _X, _Y+0.03, {"Update: %6llu %6llu", "Обновление: %6llu %6llu"}, Height::Main, BLACK, GUI::Aligment::Left),
shower3(_window, _X, _Y+0.06, {"Draw: %6llu %6llu", "Отрисовка: %6llu %6llu"}, Height::Main, BLACK, GUI::Aligment::Left) {}

void FPScounter::reset() {
    shower1.setValues(Uint64(0));
    shower2.setValues(Uint64(0));
    shower3.setValues(Uint64(0));
    avr1 = avr2 = avr3 = 0;
    count = 0;
}

void FPScounter::start() {
    prevCounter = SDL_GetPerformanceCounter();
    count++;
}

void FPScounter::setPoint1() {
    Uint64 delta = SDL_GetPerformanceCounter() - prevCounter;
    avr1 += delta;
    shower1.setValues(delta, avr1/count);
    prevCounter = SDL_GetPerformanceCounter();
}

void FPScounter::setPoint2() {
    Uint64 delta = SDL_GetPerformanceCounter() - prevCounter;
    avr2 += delta;
    shower2.setValues(delta, avr2/count);
    prevCounter = SDL_GetPerformanceCounter();
}

void FPScounter::setPoint3() {
    Uint64 delta = SDL_GetPerformanceCounter() - prevCounter;
    avr3 += delta;
    shower3.setValues(delta, avr3/count);
    prevCounter = SDL_GetPerformanceCounter();
}

void FPScounter::blit() const {
    shower1.blit();
    shower2.blit();
    shower3.blit();
}
