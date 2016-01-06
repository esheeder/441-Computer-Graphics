#include "Truck.h"
#include "Color.h"
#include "Material.h"
#include "Point.h"

Truck::Truck()
{
	headlightsOff = false;
	brightsOff = false;
	spinAngle = 0;
	backLegAngle = 0;
	frontLegAngle = 0;
	frontAngleChange = 5;
	backAngleChange = -5;
	t = 0;
	createSpiralPoints();
}

Truck::Truck(std::shared_ptr<Point> location, float spinAngle2, float theta2, float xAngle2, float zAngle2):
    Hero(location, spinAngle2, theta2, xAngle2, zAngle2) {
	name = "IB Trollinson";
	headlightsOff = false;
	brightsOff = false;
	backLegAngle = 0;
	frontLegAngle = 0;
	frontAngleChange = 5;
	backAngleChange = -5;
	t = 0;
	createSpiralPoints();
}
void Truck::drawEar()
{
	Material rabbitMaterial(
		Color(0.19225, 0.19225, 0.19225),
		Color(0.50754, 0.50754, 0.50754),
		Color(0.508273, 0.508273, 0.508273),
		0.4);
	rabbitMaterial.apply();
	glPushMatrix(); {
		glTranslatef(0, 1, 0);
		glScalef(1, 3, 1);
		glutSolidCube(1);
	}; glPopMatrix();


	//draw the inner ear
	//glColor3f(1, 0, 0.7);
	glPushMatrix(); {

	};glPopMatrix();
}
void Truck::drawTeeth() {
	Material teethMaterial(
		Color(0,0,0),
		Color(0.55, 0.55, 0.55),
		Color(0.7, 0.7, 0.7),
		0.25);
	teethMaterial.apply();
	glPushMatrix(); {
		glTranslatef(0, 1, 0);
		glScalef(0.2, 1.5, 0.2);
		glutSolidCube(1);
	}; glPopMatrix();
}
void Truck::drawNose()
{
	Material rabbitMaterial(
		Color(0.19225, 0.19225, 0.19225),
		Color(0.50754, 0.50754, 0.50754),
		Color(0.508273, 0.508273, 0.508273),
		0.4);
	rabbitMaterial.apply();
	glPushMatrix(); {
		glScalef(1.5, 1.5, 1.5);
		glutSolidCube(1);
	};glPopMatrix();

	Material noseMaterial(
		Color(0, 0, 0),
		Color(0.01, 0.01, 0.01),
		Color(0.5, 0.5, 0.5),
		0.25);
	noseMaterial.apply();
	glPushMatrix(); {
		glTranslatef(1, 0.5, 0);
		glutSolidSphere(0.5, 100, 100);
	};glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(0.5, -2.3, 0.5);
		drawTeeth();
	};glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(0.5, -2.3, -0.5);
		drawTeeth();
	};glPopMatrix();

}
void Truck::drawHead()
{
	Material rabbitMaterial(
		Color(0.19225, 0.19225, 0.19225),
		Color(0.50754, 0.50754, 0.50754),
		Color(0.508273, 0.508273, 0.508273),
		0.4);
	rabbitMaterial.apply();
	glPushMatrix(); {
		glTranslatef(0, 2.5, 0.7);
		drawEar();
	};glPopMatrix();
	glPushMatrix(); {
		glTranslatef(0, 2.5, -0.7);
		drawEar();
	};glPopMatrix();
	glPushMatrix(); {
		glTranslatef(2, -0.6, 0);
		drawNose();
	};glPopMatrix();
	glPushMatrix(); {
		glScalef(4, 4, 4);
		glutSolidCube(1);
	};glPopMatrix();

	Material eyeMaterial(
		Color(0.1745, 0.01175, 0.01175),
		Color(0.61424, 0.04136, 0.04136),
		Color(0.727811, 0.626959, 0.626959),
		0.6);
	eyeMaterial.apply();
	glPushMatrix(); {
		glTranslatef(2, 0.5, 1);
		glutSolidSphere(0.5, 100, 100);
	};glPopMatrix();
	eyeMaterial.apply();
	glPushMatrix(); {
		glTranslatef(2, 0.5, -1);
		glutSolidSphere(0.5, 100, 100);
	};glPopMatrix();
}
void Truck::drawBackLeg()
{
	Material rabbitMaterial(
		Color(0.19225, 0.19225, 0.19225),
		Color(0.50754, 0.50754, 0.50754),
		Color(0.508273, 0.508273, 0.508273),
		0.4);
	rabbitMaterial.apply();
	if (backLegAngle == 45 || backLegAngle == -45)
	{
		backAngleChange = -backAngleChange;
	}
	backLegAngle += backAngleChange;
	glRotatef(backLegAngle, 0, 0, 1);
	glPushMatrix();
	{
		glScalef(4, 4, 4);
		glutSolidCube(1);
	};glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(1.7, -1.5, 0);
		glScalef(5, 1, 4);
		glutSolidCube(1);
	};glPopMatrix();
}

void Truck::drawFrontLeg()
{

	if (frontLegAngle == 45 || frontLegAngle == -45)
	{
		frontAngleChange = -frontAngleChange;
	}
	frontLegAngle += frontAngleChange;
	glRotatef(frontLegAngle, 0, 0, 1);
	glPushMatrix();
	glRotatef(-90, 0, 1, 0);
	Material rabbitMaterial(
		Color(0.19225, 0.19225, 0.19225),
		Color(0.50754, 0.50754, 0.50754),
		Color(0.508273, 0.508273, 0.508273),
		0.4);
	rabbitMaterial.apply();
	glPushMatrix(); {
		glRotatef(45, 1, 0, 0);
		glScalef(2, 5, 1);

		glutSolidCube(1);
	};glPopMatrix();
	glPushMatrix(); {
		glTranslatef(0, -1.5, -1.5);
		glScalef(2, 1, 4);

		glutSolidCube(1);
	};glPopMatrix();
	glPopMatrix();
}


void Truck::createSpiralPoints()
{
	int numOfPoints = 149;
	float radius = 25.0;
	float t = 0;
	float x = 0.0;
	float y = 0.0;
	float z = 0.0;
	
	for (int i = 0; i<numOfPoints; i++)
	{
		z = radius*cosf(t);
		y = radius*sinf(t);
		x = t;

		addControlPoint(Point(x, y, z));
		t += 0.3;

	}
	z = 0;
	y = radius;
	x = 0;

	addControlPoint(Point(x, y, z));
}
void Truck::drawPet()
{
	
	calculatePetPath(10);
	Point petLocation = petPath[timer % petPath.size()];
	glPushMatrix(); {
		glTranslatef(petLocation.getX() - 20, petLocation.getY(), petLocation.getZ());
		Material rabbitMaterial(
			Color(0.19225, 0.19225, 0.19225),
			Color(0.50754, 0.50754, 0.50754),
			Color(0.508273, 0.508273, 0.508273),
			0.4);
		rabbitMaterial.apply();
		glPushMatrix();
		{
			glScalef(8, 6, 6);
			glutSolidCube(1);
		};glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(4, 4, 0);
			drawHead();
		};glPopMatrix();
		glPushMatrix(); {
			glTranslatef(-2.5, -1.5, -3.5);
			drawBackLeg();
		};glPopMatrix();

		glPushMatrix(); {
			glTranslatef(-2.5, -1.5, 3.5);
			drawBackLeg();
		};glPopMatrix();

		glPushMatrix(); {
			glTranslatef(4, -2, 3.5);
			drawFrontLeg();
		};glPopMatrix();

		glPushMatrix(); {
			glTranslatef(4, -2, -3.5);
			drawFrontLeg();
		};glPopMatrix();
		Material tailMaterial(
			Color(0, 0, 0),
			Color(0.55, 0.55, 0.55),
			Color(0.7, 0.7, 0.7),
			0.25);
		tailMaterial.apply();
		glPushMatrix();
		{
			glTranslatef(-5, 3, 0);
			glutSolidSphere(2, 100, 100);
		} glPopMatrix();
	} glPopMatrix();
}

void Truck::drawHero()
{
	//this draws the body of the truck
	glTranslatef(0, 8, 0);
	glPushMatrix(); {
		Material truckMaterial(
			Color(0.0, 0.0, 0.0),
			Color(0.1, 0.35, 0.1),
			Color(0.45, 0.55, 0.45),
			0.25);
		truckMaterial.apply();
		glRotatef(90, 0, 1, 0);
		glScalef(8, 3, 25);
		glutSolidCube(1);
	};glPopMatrix();
	//this draws the "cab" of the truck
	glPushMatrix(); {
		Material truckMaterial(
			Color(0.0, 0.0, 0.0),
			Color(0.1, 0.35, 0.1),
			Color(0.45, 0.55, 0.45),
			0.25);
		truckMaterial.apply();
		glTranslatef(5, 2, 0);
		glRotatef(90, 0, 1, 0);
		glScalef(8, 4, 6);
		glutSolidCube(1);
	};glPopMatrix();
	//draw back wheels
	glPushMatrix();
	{
		glTranslatef(-7, -6, -5);
		drawWheels();
	};
	glPopMatrix();
	//draw front wheels
	glPushMatrix();
	{
		glTranslatef(5, -6, -5);
		drawWheels();
	};
	glPopMatrix();
		//this creates the bumper
	//the first cube is the front
	glPushMatrix(); {
		Material bumperMaterial(
			Color(0.25, 0.25, 0.25),
			Color(0.4, 0.4, 0.4),
			Color(0.774597, 0.774597, 0.774597),
			0.6);
		bumperMaterial.apply();
		glTranslatef(13, -1, 0);
		glScalef(1, 1, 10);
		glutSolidCube(1);
	};glPopMatrix();
	//these create the sides of the bumper
	glPushMatrix(); {
		Material bumperMaterial(
			Color(0.25, 0.25, 0.25),
			Color(0.4, 0.4, 0.4),
			Color(0.774597, 0.774597, 0.774597),
			0.6);
		bumperMaterial.apply();
		glTranslatef(11, -1, 4.5);
		glScalef(3, 1, 1);
		glutSolidCube(1);
	};glPopMatrix();
	glPushMatrix(); {
		GLfloat headlightPos1[4] = { 0,0,0,1};
		GLfloat headlightDir[4] = { 1,0,0, 0 };
		Material headlightMaterial(
			Color(0.25, 0.20725, 0.20725),
			Color(1, 0.829, 0.829),
			Color(0.296648, 0.296648, 0.296648),
			0.088);
		headlightMaterial.apply();

		glTranslatef(12, 0, -2);
		glutSolidSphere(1, 100, 100);
		glLightfv(GL_LIGHT1, GL_POSITION, headlightPos1);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, headlightDir);
	};glPopMatrix();
	glPushMatrix(); {
		GLfloat headlightPos1[4] = { 0,0,0,1 };
		GLfloat headlightDir[4] = { 1,0,0, 0 };
		Material headlightMaterial(
			Color(0.25, 0.20725, 0.20725),
			Color(1, 0.829, 0.829),
			Color(0.296648, 0.296648, 0.296648),
			0.088);
		headlightMaterial.apply();

		glTranslatef(12, 0, 2);
		glutSolidSphere(1, 100, 100);
		glLightfv(GL_LIGHT2, GL_POSITION, headlightPos1);
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, headlightDir);



	};glPopMatrix();
	glPushMatrix(); {
		Material axelMaterial(
			Color(0.25, 0.25, 0.25),
			Color(0.4, 0.4, 0.4),
			Color(0.774597, 0.774597, 0.774597),
			0.6);
		axelMaterial.apply();
		
		glTranslatef(11, -1, -4.5);
		glScalef(3, 1, 1);
		glutSolidCube(1);
	};glPopMatrix();
}

void Truck::drawAxle()
{
	Material axelMaterial(
		Color(0.25, 0.25, 0.25),
		Color(0.4, 0.4, 0.4),
		Color(0.774597, 0.774597, 0.774597),
		0.6);
	axelMaterial.apply();
	GLUquadric* axle = gluNewQuadric();
		//create an open cyliner
	gluCylinder(axle, 1, 1, 1, 100, 100);
	//create a disk to close one side of the cylinder
	glRotatef(180, 1, 0, 0);
	gluDisk(axle, 0.0f, 1, 100, 1);
	//create another disk to cover the other side of the open cylinder
	glRotatef(180, 1, 0, 0);
	glTranslatef(0.0f, 0.0f, 1);
	gluDisk(axle, 0.0f, 1, 100, 1);
	glTranslatef(0.0f, 0.0f, -1);
	}
void Truck::drawWheels()
{
	glTranslatef(0, 4, 0);
	glPushMatrix(); {
		//change the axel's length to 10
		glScalef(1, 1, 10);
		drawAxle();
	};glPopMatrix();
	//draw 2 tires and place them at the ends of the axle
	Material headlightMaterial(
		Color(0.02, 0.02, 0.02),
		Color(0.01, 0.01, 0.01),
		Color(0.4, 0.4, 0.4),
		0.078125);
	headlightMaterial.apply();
	
	glRotatef(-timer*2, 0, 0, 1);
	glutSolidTorus(2, 3, 100, 6);
	glTranslatef(0, 0, 10);
	glutSolidTorus(2, 3, 100, 6);
}
	void Truck::toggleHeadlights()
{
	headlightsOff = !headlightsOff;
}
	void Truck::toggleBrights()
{
	brightsOff = !brightsOff;
}
bool Truck::headlightsOn()
{
	return !headlightsOff;
}
bool Truck::brightsOn()
{
	return !brightsOff;
}
