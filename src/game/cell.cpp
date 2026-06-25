/*
 * Copyright (C) 2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "cell.hpp"


Cell::Cell() {}

void Cell::setGase(float _pressure, float _temperature) {
    temperature = _temperature;
    mass = _pressure * volume / _temperature;
    state = Gase;
}

void Cell::reset(const Cell _environment) {
    // Setting parameters to global
    temperature = _environment.temperature;
    mass = _environment.mass;
    state = States::Gase;
}

Uint32 Cell::getState() const {
    return state;
}

float Cell::getPressure() const {
    return mass * temperature / volume;
}

float Cell::getMass() const {
    return mass;
}

float Cell::getTemperature() const {
    return temperature;
}

bool Cell::isPass() const {
    return state >= Liquid;
}

bool Cell::isBlock() const {
    return state <= Solid;
}

void Cell::setState(Uint32 _state) {
    state = _state;
}

void Cell::reduceMass(float _change) {
    // Temperature doesn't change
    mass /= _change;
}

void Cell::applyTemperature(float _power) {
    // Mass stay constant
    temperature += _power/mass/heatCapacity;
}

float Cell::getMassFlow(const Cell& _current, const Cell& _other, float _koef) const {
    float d = (_other.getPressure() - _current.getPressure()) * _koef;
    // Check, if exceed mass
    /*if (d > _other.mass/divisionKoef) {
        return _other.mass/divisionKoef;
    }
    if (d < -_current.mass/divisionKoef) {
        return -_current.mass/divisionKoef;
    }*/
    return d;
}

void Cell::applyMass(float _deltaMass, const Cell _srcGase) {
    // Changing tempearture
    temperature = (mass * temperature * heatCapacity + _deltaMass * _srcGase.temperature * _srcGase.heatCapacity) / 
        (mass + _deltaMass) * heatCapacity;
    // Changing mass
    mass += _deltaMass;
}

void Cell::calculateNew(const Cell _upCells[3], const Cell _midleCells[3],
    const Cell _downCells[3]) {
    const Cell* src = _midleCells + 1;
    if (src->isPass()) {
        float energy = mass * temperature * heatCapacity;
        energy += exchange(*src, _upCells[0],    diagonalKoef);
        energy += exchange(*src, _upCells[1],    pressureKoef);
        energy += exchange(*src, _upCells[2],    diagonalKoef);
        energy += exchange(*src, _midleCells[0], pressureKoef);
        energy += exchange(*src, _midleCells[2], pressureKoef);
        energy += exchange(*src, _downCells[0],  diagonalKoef);
        energy += exchange(*src, _downCells[1],  pressureKoef);
        energy += exchange(*src, _downCells[2],  diagonalKoef);
        temperature = energy / mass / heatCapacity;
    }
}

float Cell::exchange(const Cell& _current, const Cell& _other, float _koef) {
    if (_other.isPass()) {
        float deltaMass = getMassFlow(_current, _other, _koef);
        //logger.additional("%f", deltaMass);
        // Changing mass
        mass += deltaMass;
        // Changing temperture
        if (deltaMass > 0) {
            return deltaMass * _other.temperature * heatCapacity;
        } else {
            return deltaMass * _current.temperature * heatCapacity;
        }
    }
    return 0.0;
}

/*void Cell::vent(Cell& _outGase, float _power) {
    // Getting change
    float deltaMass = getMassFlow(_outGase, _power);

    // Changing temperture
    if (deltaMass > 0) {
        addMass(-deltaMass, *this);
        _outGase.addMass(deltaMass, *this);
    } else {
        addMass(-deltaMass, _outGase);
        _outGase.addMass(deltaMass, _outGase);
    }
}

void Cell::exchangeValved(Cell& _outGase) {
    // Getting change
    float deltaMass = getMassFlow(_outGase);

    // Allow only to one side
    if (deltaMass > 0) {
        // Changing temperture
        addMass(-deltaMass, *this);
        _outGase.addMass(deltaMass, *this);
    }
}

void Cell::cool(Cell& _outGase, float _power) {
    float delta = (temperature - _outGase.temperature - _power) * 0.5;

    if (temperature > delta) {
        // Exchanging energy
        newEnergy -= delta * mass * heatCapacity;
        _outGase.newEnergy += delta * _outGase.mass * heatCapacity;
    }
}*/

void Cell::blitNormal(const Window& _window, SDL_FRect _rect) const {
    // Nothing
    _window.setDrawColor(WHITE);
    _window.drawRect(_rect);
}

void Cell::blitThermal(const Window& _window, SDL_FRect _rect) const {
    if (temperature < 300.0) {
        // Cold spectre
        _window.setDrawColor({0, 0, Uint8(255 - temperature*(255.0/300.0)), 255});
    } else if (temperature < 300.0 + 255.0) {
        // Warm spectre
        _window.setDrawColor({Uint8(temperature-300.0), 0, 0, 255});
    } else {
        // Over hotx
        _window.setDrawColor({255, 0, 0, 255});
    }
    _window.drawRect(_rect);
}

void Cell::blitPressure(const Window& _window, SDL_FRect _rect) const {
    float pressure = getPressure();

    // Doesn't check less then 0
    if (pressure > 255/drawPressureKoef) {
        _window.setDrawColor({0, 0, 255, 255});
    } else {
        _window.setDrawColor({0, 0, Uint8(pressure*drawPressureKoef), 255});
    }
    _window.drawRect(_rect);
}
