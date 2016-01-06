#include "MogdinBaramud.h"
#include "Material.h"
#include "common.h"

#include <cmath>

MogdinBaramud::MogdinBaramud(std::shared_ptr<Point> location, float spinAngle, float theta, float xAngle, float zAngle) :
	Hero(location, spinAngle, theta, xAngle, zAngle) {
	name = "Mogdin Baramud";
}

void MogdinBaramud::drawBlade()
{
    glPushMatrix();

    // Color brown
	Material bronzeMaterial(
		Color(0.2125, 0.1275, 0.054),
		Color(0.714, 0.4284, 0.18144),
		Color(0.393548, 0.271906, 0.166721),
		0.2);
	bronzeMaterial.apply();

    // Move up
    glTranslatef(0, 0, 0.3);

    // Flatten and elongate
    glScalef(1, 10, 0.2);

    glutSolidCube(0.5);

    glPopMatrix();
}

void MogdinBaramud::drawBlades()
{
    glPushMatrix();

    glTranslatef(0, 0, 0.5);

	Material blackMaterial(
		Color(0.05375, 0.05, 0.06625),
		Color(0.18275, 0.17, 0.22525),
		Color(0.332741, 0.328634, 0.346435),
		0.3);
	blackMaterial.apply();
    // Shaft
    gluCylinder(gluNewQuadric(), 0.1, 0.1, 0.3, 32, 32);

    // Draw blades at right angles
    glRotatef(bladeRotation, 0, 0, 1);
    drawBlade();
    glRotatef(90, 0, 0, 1);
    drawBlade();

    glPopMatrix();
}

void MogdinBaramud::drawBody()
{
    glPushMatrix();
    // Color army green
	Material brasseMaterial(
		Color(0.329412,0.223529, 0.027451),
		Color(0.780392,	0.568627, 0.113725),
		Color(0.992157,	0.941176, 0.807843),
		0.21794872);
	brasseMaterial.apply();

    glScalef(1.5, 3, 1);

    glutSolidCube(1);

    glPopMatrix();
}

/*
void MogdinBaramud::drawFireSpriteSpike()
{
    glColor3f(242/255., 38/255., 19/255.);

    // Vary the height of the spike based on the position along the curve.
    float height = cos((companionPosition / curveCount * 2 * M_PI) * 4) + 3;
    height /= 1.5;

    glutSolidCone(0.5, height, 32, 32);
}

void MogdinBaramud::drawFireSpriteSpikes()
{
    glColor3f(242/255., 38/255., 19/255.);

    for (int i = 0; i <= 4; ++i)
    {
        for (int j = 0; j <= 8; ++j)
        {
            glPushMatrix();
            glRotatef(180 / 4 * i, 0, 1, 0);
            glRotatef(360 / 8 * j, 1, 0, 0);
            drawFireSpriteSpike();
            glPopMatrix();
        }
    }
}

void MogdinBaramud::drawFireSprite()
{
    glPushMatrix();

    int curve = ((int)companionPosition) * 3;
    
    Point pos = evaluateBezierCurve(controlPoints[curve + 0],
                                    controlPoints[curve + 1],
                                    controlPoints[curve + 2],
                                    controlPoints[curve + 3],
                                    companionPosition - (int)companionPosition);

    glTranslatef(pos.getX(), pos.getY(), pos.getZ());

    glScalef(0.25, 0.25, 0.25);

    // Draw the core
    glColor3f(207/255., 0/255., 15/255.);
    gluSphere(quadric, 1, 32, 32);

    drawFireSpriteSpikes();


    glPopMatrix();
}

void MogdinBaramud::drawCompanion()
{
    glPushMatrix();
    if (displayCage) drawControlCage();
    if (displayCurve) drawBezierCurve();
    drawFireSprite();
    glPopMatrix();
}
*/

/**
 * Draws the character using GLUT 3D primitives.
 */
void MogdinBaramud::drawHero()
{
    glPushMatrix();

    //glTranslate(*location);

    // Rotate to same orientation as camera
    // glRotatef(-cameraTheta * (180 / M_PI), 0, 1, 0);
    // glRotatef(cameraPhi * (180 / M_PI), 1, 0, 0);
    glRotatef(90, 0, 1, 0);
    glRotatef(90, 1, 0, 0);

    glScalef(1, 1, -1);

    drawBody();
    drawBlades();

    // TODO: include drawing of pet
    // drawCompanion();

    glPopMatrix();

    bladeRotation += 5;
}
// TODO: figure out whether everything below this needs to be kept
/**
 * Draw the control cage including control points and lines connecting them
 */
/*
void drawControlCage()
{
    for (const Point& p : controlPoints)
    {
        glPushMatrix();
        glTranslatef(p.getX(), p.getY(), p.getZ());
        glColor3f(0, 1, 0);
        gluSphere(quadric, 0.25, 32, 32);
        glPopMatrix();
    }
    
    glBegin(GL_LINES);
    glColor3f(1, 1, 0);
    for (size_t i = 0; i < controlPoints.size() - 1; ++i)
    {
        Point& p1 = controlPoints[i];
        Point& p2 = controlPoints[i + 1];

        glVertex3f(p1.getX(), p1.getY(), p1.getZ());
        glVertex3f(p2.getX(), p2.getY(), p2.getZ());
    }   
    glEnd();
}*/

/**
 * Draws the bezier curve defined by the global controlPoints map
 */
/*
void drawBezierCurve()
{
    for (size_t i = 3; i < controlPoints.size(); i += 3)
    {
        renderBezierCurve(
                controlPoints[i - 3],
                controlPoints[i - 2],
                controlPoints[i - 1],
                controlPoints[i - 0],
                32);
    }
}*/
