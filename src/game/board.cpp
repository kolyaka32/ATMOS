/*
 * Copyright (C) 2025-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "board.hpp"


Board::Board() {
    reset();
}

void Board::reset() {
    enviroment.setGase(100.0, 300.0);
    for (int i=0; i < height*width; ++i) {
        cells[i].reset(enviroment);
        newCells[i].reset(enviroment);
    }
    switched = false;
    currentField = cells;
    calculatingField = newCells;
}

int Board::getWidth() const {
    return width-2;
}

int Board::getHeight() const {
    return height-2;
}

bool Board::in(SDL_Point _pos) const {
    return _pos.x > 0 && _pos.x < width-1 &&
        _pos.y > 0 && _pos.y < height-1;
}

float Board::getPressure(SDL_Point _pos) const {
    return currentField[_pos.y*width+_pos.x].getPressure();
}

float Board::getTemperature(SDL_Point _pos) const {
    return currentField[_pos.y*width+_pos.x].getTemperature();
}

void Board::setCell(SDL_Point _pos, const Cell _cell) {
    calculatingField[_pos.y*width+_pos.x] = _cell;
}

void Board::resetCell(SDL_Point _pos) {
    calculatingField[_pos.y*width+_pos.x].reset(enviroment);
}

void Board::applyMass(SDL_Point _pos, float _deltaMass) {
    calculatingField[_pos.y*width+_pos.x].applyMass(_deltaMass, enviroment);
}

void Board::reduceMass(SDL_Point _pos, float _deltaMass) {
    calculatingField[_pos.y*width+_pos.x].reduceMass(_deltaMass);
}

void Board::applyTemperature(SDL_Point _pos, float _temperature) {
    calculatingField[_pos.y*width+_pos.x].applyTemperature(_temperature);
}

void Board::update() {
    for (int y=1; y < height-1; ++y) {
        for (int x=1; x < width-1; ++x) {
            // Exchanging with surrounding cells
            // ! should be optimised to multithreading
            calculatingField[y*width+x].calculateNew(currentField+(y-1)*width+x-1,
                currentField+y*width+x-1, currentField+(y+1)*width+x-1);
        }
    }
}

void Board::applyChanges() {
    // Swapping pointers to arrays
    if (switched) {
        currentField = cells;
        calculatingField = newCells;
        switched = false;
    } else {
        currentField = newCells;
        calculatingField = cells;
        switched = true;
    }
    //memcpy(currentField, calculatingField, sizeof(cells));
}

void Board::blitNormal(const Window& _window, SDL_FRect _cellRect) const {
    for (int y=1; y < height-1; ++y) {
        for (int x=1; x < width-1; ++x) {
            currentField[y*width+x].blitNormal(_window, _cellRect);
            _cellRect.x += _cellRect.w;
        }
        _cellRect.x -= _cellRect.w * (width-2);
        _cellRect.y += _cellRect.h;
    }
}

void Board::blitThermal(const Window& _window, SDL_FRect _cellRect) const {
    for (int y=1; y < height-1; ++y) {
        for (int x=1; x < width-1; ++x) {
            currentField[y*width+x].blitThermal(_window, _cellRect);
            _cellRect.x += _cellRect.w;
        }
        _cellRect.x -= _cellRect.w * (width-2);
        _cellRect.y += _cellRect.h;
    }
}

void Board::blitPressure(const Window& _window, SDL_FRect _cellRect) const {
    for (int y=1; y < height-1; ++y) {
        for (int x=1; x < width-1; ++x) {
            currentField[y*width+x].blitPressure(_window, _cellRect);
            _cellRect.x += _cellRect.w;
        }
        _cellRect.x -= _cellRect.w * (width-2);
        _cellRect.y += _cellRect.h;
    }
}

void Board::blitLines(const Window& _window, SDL_FRect _cellRect) const {
    // Draw separating lines
    _window.setDrawColor(BLACK);
    // Fining opposite side
    const SDL_FPoint oppositeP = {_cellRect.x + _cellRect.w*(width-2),
        _cellRect.y + _cellRect.h*(height-2)};

    // Vertical lines
    float x = _cellRect.x;
    for (int i=0; i < width-1; ++i) {
        _window.drawLine(x, _cellRect.y, x, oppositeP.y);
        x += _cellRect.w;
    }

    // Horizontal lines
    float y = _cellRect.y;
    for (int i=0; i < height-1; ++i) {
        _window.drawLine(_cellRect.x, y, oppositeP.x, y);
        y += _cellRect.h;
    }
}
