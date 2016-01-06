#pragma once

#include "Hero.h"

class MogdinBaramud : public Hero
{
public:
    /**
     * Draws the character using GLUT 3D primitives.
     */
    void drawHero();

	MogdinBaramud(std::shared_ptr<Point> location, float spinAngle, float theta, float xAngle, float zAngle);

private:
    /**
     * Draws one of the helicopter blades
     */
    void drawBlade();

    /**
     * Draws both helicopter blades and the shaft connecting them to the body.
     */
    void drawBlades();

    /**
     * Draws the body of the helicopter
     */
    void drawBody();

    /**
     * Draw a single spike on the fire sprite
     */
    void drawFireSpriteSpike();

    /**
     * Draw the pulsating spikes on the fire sprite
     */
    void drawFireSpriteSpikes();

    /**
     * Draws the fire sprite following the bezier curve using GLUT 3D primitives
     */
    void drawFireSprite();

    /**
     * Draws the character's companion using GLUT 3D primitives.
     * Also draws the bezier curve it's following and the control cage.
     */
    void drawCompanion();

    float bladeRotation;
};
