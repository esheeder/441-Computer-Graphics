#pragma once

#include "Point.h"

#include <memory>

/**
* A class to represent a camera in OpenGL.
* Keeps track of position, look at point, and up vector.
* Doesn't control any logic for movement on its own.
*/
class Camera {
public:

	Camera();

	/**
	* Basic Camera constructor.
	*
	* @param position The position of the camera
	* @param lookAt The point the camera is looking at
	* @param up The up-vector for the camera
	*/
	Camera(const Point& position, std::shared_ptr<Point> lookAt, const Point& up);

	Camera(const Camera& other);


    /**
     * Updates the modelview matrix with the view from this camera.
     */
    void look() const;

	Point getPosition();

	void setPosition(const Point& position);

	Point getLookAt();

	void setLookAt(const Point& lookAt);

	void setLookAtPointer(std::shared_ptr<Point> lookAt);

    Point getUp();

	void setUp(const Point& up);

    Point getDirection();

private:

    Point position;
    std::shared_ptr<Point> lookAt;
    Point up;
};

