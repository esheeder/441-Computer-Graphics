#pragma once

#include <memory>
#include <vector>
#include <functional>

/**
 * A class representing an OpenGL viewport.
 * Takes care of calling glViewport and glScissor.
 * Stores a function to draw the contents of the viewport, or
 * can alternatively delegate to child Viewport objects
 * for nested views.
 */
class Viewport
{
public:
    
    /**
     * Empty Viewport constructor.
     */
    Viewport();

    /**
     * Viewport constructor which initializes member variables.
     */
    Viewport(float x, float y, float width, float height);
    
    /**
     * Renders the Viewport and all child Viewports.
     */
    void display();

    /**
     * Sets the function to be used when displaying this viewport.
     */
    void setDisplayFunction(std::function<void()> func);

    // TODO: possibly factor out into child class with getLeft and getRight methods?
    // Not in the scope of the midterm project, but putting it here as a reminder for later.
    // This would also allow each Viewport child class to have methods for rearranging its children.
    /**
     * Splits the viewport into two child viewports.
     *
     * @param leftProportion The proportion of the horizontal space to allocate to the left child
     *     viewport, in the range [0, 1]. Will be clamped if outside.
     *
     * @return A pair of weak_ptrs to the resulting viewports. The first element is the left Viewport,
     *     and the second is the right Viewport.
     */
    std::pair<std::weak_ptr<Viewport>, std::weak_ptr<Viewport>> splitHorizontal(float leftProportion);

    // TODO: Now this is is really just a horizontal split viewport class. I forgot to take into
    // account that all the child viewports would need to be resized, which I can't do generically
    // without inheritance.
    /**
     * Set the width of the viewport and adjust all child viewports accordingly.
     */
    void setWidth(float width);

    /**
     * Set the height of the viewport and adjust all child viewports accordingly.
     */
    void setHeight(float height);

    /**
     * Set the x coordinate of the viewport and adjust all child viewports accordingly.
     */
    void setX(float x);

    /**
     * Set the y coordinate of the viewport and adjust all child viewports accordingly.
     */
    void setY(float y);

protected:
    
    float x, y, width, height;
    // TODO: remove this member once there are proper child classes.
    float leftProportion;
    std::vector<std::shared_ptr<Viewport>> children;
    std::function<void()> displayFunction;
};
