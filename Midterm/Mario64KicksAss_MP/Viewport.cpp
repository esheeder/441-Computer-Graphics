#include "Viewport.h"

#include "common.h"

#include <iostream>
#include <utility>

using std::weak_ptr;
using std::pair;
using std::make_shared;
using std::make_pair;

Viewport::Viewport():
    displayFunction(nullptr)
{
}

Viewport::Viewport(float x, float y, float width, float height):
    x(x),
    y(y),
    width(width),
    height(height),
    displayFunction(nullptr)
{
}

void Viewport::display()
{
    if (displayFunction)
    {
        glEnable(GL_SCISSOR_TEST);
        glScissor(x, y, width, height);
        glViewport(x, y, width, height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, width / height, 0.1, 100000);
        displayFunction();
    }

    for (auto child : children)
    {
        child->display();
    }
}

void Viewport::setDisplayFunction(std::function<void()> func)
{
    displayFunction = func;
}


pair<weak_ptr<Viewport>, weak_ptr<Viewport>> Viewport::splitHorizontal(float leftProportion)
{
    // Clamp proportions to the interval [0, 1]
    if (leftProportion < 0) leftProportion = 0;
    if (leftProportion > 1) leftProportion = 1;

    // TODO: remove this member once there are proper child classes.
    this->leftProportion = leftProportion;

    float leftWidth = width * leftProportion;
    float rightWidth = width * (1 - leftProportion);

    children.emplace_back(make_shared<Viewport>(x, y, leftWidth, height));
    children.emplace_back(make_shared<Viewport>(x + leftWidth, y, rightWidth, height));

    return make_pair<weak_ptr<Viewport>, weak_ptr<Viewport>>(children[0], children[1]);
}

void Viewport::setWidth(float width)
{
    this->width = width;

    // TODO: replace with calls to child->setParentWidth or something similar
    if (!children.empty())
    {
        children[0]->setWidth(width * leftProportion);
        children[1]->setWidth(width * (1 - leftProportion));

        children[1]->setX(width * leftProportion);
    }
}

void Viewport::setHeight(float height)
{
    this->height = height;

    // TODO: replace with calls to child->setParentHeight or something similar
    if (!children.empty())
    {
        children[0]->setHeight(height);
        children[1]->setHeight(height);
    }
}

void Viewport::setX(float x)
{
    this->x = x;

    // TODO: set children here later
}


void Viewport::setY(float y)
{
    this->y = y;

    // TODO: set children here later
}

