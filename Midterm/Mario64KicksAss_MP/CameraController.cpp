#include "CameraController.h"

CameraController::CameraController(const Camera& camera):
    camera(camera)
{

}

Camera CameraController::getCamera()
{
    return camera;
}
