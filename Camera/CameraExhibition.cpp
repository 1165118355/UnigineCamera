#include "CameraExhibition.h"
#include <SpaceApp.h>
#include <SpaceGame.h>
#include <SpacePlayers.h>

std::shared_ptr<CameraExhibition> CameraExhibition::create()
{
    std::shared_ptr<CameraExhibition> instance(new CameraExhibition());
    return instance;
}

void CameraExhibition::init()
{

}

void CameraExhibition::update()
{

    if (Space::App::get()->getMouseButtonState(Space::App::BUTTON_LEFT))
    {
        float distanceX = Space::ControlsApp::get()->getMouseDX();
        float distanceY = Space::ControlsApp::get()->getMouseDY();
        if(m_TargetNode)
        {
            float dx = m_TargetNode->getWorldRotation().getAngle(Space::Math::vec3(0.0, 0.0, 1.0)) + distanceX;
            float dy = m_TargetNode->getWorldRotation().getAngle(Space::Math::vec3(1.0, 0.0, 0.0)) + distanceY;
            if((m_Axis & AXIS_X) == AXIS_X)
            {
                m_TargetNode->setWorldRotation(Space::Math::quat(Space::Math::rotate(Space::Math::quat(0.0, 0.0, 1.0, dx))));
            }
            if((m_Axis & AXIS_Y) == AXIS_Y)
            {
                m_TargetNode->setWorldRotation(m_TargetNode->getWorldRotation() * Space::Math::quat(Space::Math::rotate(Space::Math::quat(1.0, 0.0, 0.0, dy))));
            }
        }
    }
}

void CameraExhibition::shutdown()
{

}

void CameraExhibition::setTarget(Space::NodePtr node)
{
    if(node)
    {
        resetExhibitionTransform();
        Camera::setTarget(node);
        recordExhibitionTransform();
    }
}

CameraExhibition::CameraExhibition()
{
    m_Axis = AXIS_X | AXIS_Y;
    m_Type = CAMERA_EXHIBITION;
}

void CameraExhibition::recordExhibitionTransform()
{
    if(m_TargetNode)
    {
        m_ExhibitioTransformFront = m_TargetNode->getWorldTransform();
    }
}

void CameraExhibition::resetExhibitionTransform()
{
    if(m_TargetNode)
    {
        m_TargetNode->setWorldTransform(m_ExhibitioTransformFront);
    }
}

void CameraExhibition::setAxis(int axis)
{
    m_Axis = axis;
}

int CameraExhibition::getAxis()
{
    return m_Axis;
}
