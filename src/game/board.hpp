/*
 * Copyright (C) 2025-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#pragma once

#include "cell.hpp"


// Main object
class Board {
 private:
    // Lengthes of field (with side cells)
    static const int height = 100;
    static const int width  = 100;
    // Main array of cells
    Cell cells[height*width];
    Cell newCells[height*width];
    // Variables
    Cell enviroment;
    bool switched;
    Cell* currentField;  // Field, that currently using as data source
    Cell* calculatingField;    // Filed, that currently updating, swap with current after

 public:
    Board();
    void reset();
    int getWidth() const;
    int getHeight() const;
    bool in(SDL_Point pos) const;

    // Interaction
    void setCell(SDL_Point pos, const Cell cell);
    void resetCell(SDL_Point pos);
    void applyMass(SDL_Point pos, float deltaMass);
    void reduceMass(SDL_Point pos, float deltaMass);
    void applyTemperature(SDL_Point pos, float temperature);
    float getPressure(SDL_Point pos) const;
    float getTemperature(SDL_Point pos) const;

    // Every cycle update
    void update();
    void applyChanges();

    // Draw different modes/parts
    void blitNormal(const Window& window, SDL_FRect cellRect) const;
    void blitThermal(const Window& window, SDL_FRect cellRect) const;
    void blitPressure(const Window& window, SDL_FRect cellRect) const;
    void blitLines(const Window& window, SDL_FRect cellRect) const;
};
