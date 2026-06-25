/*
 * Copyright (C) 2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#pragma once

#include "../data/app.hpp"


// Class for interact with gase state and exchange
class Cell {
 private:
    // Parameters of one cell of gase
    enum States {
        None,
        Solid,
        Liquid,
        Gase,
    };
    Uint32 state;
    // Required 3 from 4 paramters: mass, presure, temperature, volume
    static constexpr float volume = 1.0;  // [m^3]
    float mass;         // [kg]
    float temperature;  // [K]
    // Pressure as result

    // Constants
    static constexpr float divisionKoef = 32.0;   // Koef, which part can go away
    static constexpr float pressureKoef = 0.0001;  // [1]
    static constexpr float diagonalKoef = pressureKoef * 1.1414/2;
    static constexpr float heatCapacity = 1.0;    // [J/kg/K]
    static constexpr float drawTemperatureKoef = 2.5;
    static constexpr float drawPressureKoef = 1.0;

    // Return mass, flowing from current to other cell, signed
    float getMassFlow(const Cell& current, const Cell& other, float koef) const;

 public:
    Cell();
    void setGase(float pressure, float temperature);
    void reset(const Cell environment);

    // Getters
    Uint32 getState() const;
    float getPressure() const;
    float getMass() const;
    float getTemperature() const;
    bool isPass() const;
    bool isBlock() const;

    // Interactions
    void setState(Uint32 state);
    void reduceMass(float koefMass);
    void applyMass(float deltaMass, const Cell srcGase);
    void applyTemperature(float power);

    // Every cycle updates
    void calculateNew(const Cell upCells[3], const Cell midleCells[3],
        const Cell downCells[3]);  // Calculate new cell value rely on surrounding
    // Change between current and other
    float exchange(const Cell& current, const Cell& other, float koef);

    // Machine work - sholud migrate
    //void vent(Cell& outGase, float power);  // Take air from current cell to outlet
    //void exchangeValved(Cell& outGase);  // Allow flow only to one direction
    //void cool(Cell& outGase, float power);  // Cool tile by heating up out tile

    // Drawing
    void blitNormal(const Window& window, SDL_FRect rect) const;
    void blitThermal(const Window& window, SDL_FRect rect) const;
    void blitPressure(const Window& window, SDL_FRect rect) const;
};
