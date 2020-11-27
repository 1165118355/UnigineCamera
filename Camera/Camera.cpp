#include "Camera.h"
#include <SpaceNodes.h>

Camera::Camera()
{
    m_Type = CAMERA_NONE;
}

void Camera::setTarget(Space::NodePtr node)
{
    if(node)
    {
        m_TargetNode = node;
    }
}


void Camera::setPlayer(Space::PlayerPtr player)
{
    m_Player = player;
}

Space::PlayerPtr Camera::getPlayer()
{
    return m_Player;
}

void Camera::setEnabled(bool enabled)
{
    m_Enabled = enabled;
}

bool Camera::isEnabled()
{
    return m_Enabled;
}

void Camera::clear()
{
    m_TargetNode.clear();
    m_Player.clear();
}
