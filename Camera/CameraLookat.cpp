#include "CameraLookat.h"
#include "Utils/UtilsMath.h"
#include <queue>
#include <SpaceGame.h>
#include <SpaceApp.h>
#include <SpaceVisualizer.h>

CameraLookat::CameraLookat()
{
    m_Type = CAMERA_LOOKAT;
    m_Distance = 1;
    m_MinDistance = .02;
    m_MaxDistance = 15;
}

std::shared_ptr<CameraLookat> CameraLookat::create()
{
    std::shared_ptr<CameraLookat > instance(new CameraLookat());
    return instance;
}

void CameraLookat::init()
{
    Space::ControlsApp::get()->setEnabled(true);
    Space::ControlsApp::get()->setMouseEnabled(true);
}

void CameraLookat::update()
{
    float xDirection = Space::ControlsApp::get()->getMouseDX();
    float yDirection = Space::ControlsApp::get()->getMouseDY();
    float xdistance = 0;
    float ydistance = 0;
    float xMovedistance = 0;
    float yMovedistance = 0;
    auto position = m_Player->getWorldPosition();
    Space::Math::Vec3 moveOffset;
    if(xDirection != 0 ||
            yDirection != 0)
    {
        m_AutoRotation = false;
    }
    if (Space::App::get()->getMouseButtonState(m_RotationButton))
    {
        Space::ControlsApp::get()->setEnabled(true);
        Space::ControlsApp::get()->setMouseEnabled(true);
        xdistance = xDirection;
        ydistance = Space::ControlsApp::get()->getMouseDY();
        m_IsSliding = false;
    }
    else if(Space::App::get()->getMouseButtonState(m_MoveButton))
    {
        auto xdir = m_Player->getWorldDirection(Space::Node::AXIS_NX);
        moveOffset += Space::Math::Vec3(xdir) * (m_Distance / 140*xDirection);
        auto ydir = m_Player->getWorldDirection(Space::Node::AXIS_Y);
        ydir.z = 0;
        ydir = Space::Math::normalize(ydir);
        moveOffset += Space::Math::Vec3(ydir) * (m_Distance/140*yDirection);
        m_TargetPoint += moveOffset;
        m_Player->setWorldPosition(position+moveOffset);
    }
    else
    {
        if(m_IsSliding == false)
        {
            m_IsSliding = true;
        }
        else
        {
            m_VerticalTellValue = m_VerticalTellValue * Space::Math::clamp(1 - Space::Game::get()->getIFps()*m_SlidingDamping, 0.f, 1.f);
            m_HorizontalTellValue = m_HorizontalTellValue * Space::Math::clamp(1 - Space::Game::get()->getIFps()*m_SlidingDamping, 0.f, 1.f);
            xdistance = m_HorizontalTellValue;
            ydistance = m_VerticalTellValue;
        }
    }

    horizontal(xdistance);
    vertical(ydistance);
    if(!m_IsSliding)
    {
        m_VerticalTellValue = (m_VerticalTellValue + ydistance) / 2.0;
        m_HorizontalTellValue = (m_HorizontalTellValue + xdistance) / 2.0;
    }

    int middle = Space::App::get()->getMouseAxis(Space::App::AXIS_Y);
    zoom(middle);

    //  自动旋转
    if(m_AutoRotationEnabled)
    {
        if(m_AutoRotation)
        {
            horizontal(m_AutoRotationVelocity * Space::Game::get()->getIFps());
        }
        else
        {
            if(m_AutoRotationTime >= m_AutoRotationTimeMax)
            {
                m_AutoRotationTime = 0;
                m_AutoRotation = true;
            }
            else
            {
                m_AutoRotationTime += Space::Game::get()->getIFps();
            }
        }
    }

//    auto p1 = m_TargetPoint;
//    p1.z+=10000;
//    Space::Visualizer::get()->renderLine3D(m_TargetPoint, p1, Space::Math::vec4(1,0,0,1));
}

void CameraLookat::shutdown()
{

}

void CameraLookat::setTarget(Space::NodePtr node)
{
    Camera::setTarget(node);
    if(node)
    {
        SPACE_BOUND_SPHERE sphere = node->getWorldBoundSphere();
        m_TargetPoint = sphere.getCenter();
        m_Distance = Space::Math::length(m_Player->getWorldPosition() - m_TargetPoint);
        refresh();
    }
}

void CameraLookat::setTarget(Space::Math::Vec3 targetPosition, float distance)
{
    Camera::setTarget(Space::NodePtr());
    if(1)
    {
       m_Player->setWorldPosition(m_Player->getWorldPosition() + targetPosition - m_TargetPoint);
    }
    m_TargetPoint = targetPosition;
    m_Distance = distance;
    refresh();
}

void CameraLookat::zoom(float value)
{
    if (value != 0 && m_IsZoom)
    {
        float delta = Space::Game::get()->getIFps() * value * (m_Distance);
        Space::Math::Vec3 target = m_TargetPoint;
        Space::Math::Vec3 pos = m_Player->getWorldPosition();
        m_Distance = Space::Math::length(pos - target);
        if(m_Distance<m_MinDistance && value > 0)
            delta = -Space::Game::get()->getIFps() *(m_MinDistance - m_Distance);
        if(m_Distance > m_MaxDistance && value <0)
            delta = Space::Game::get()->getIFps() *(m_Distance - m_MaxDistance);
        pos = pos + Space::Math::normalize(target - pos) * delta;
        m_Player->setWorldPosition(pos);

    }
}

void CameraLookat::setZoomEnabled(bool enabled)
{
    m_IsZoom= enabled;
}

void CameraLookat::horizontal(float value)
{
    if(value == 0)
    {
        return;
    }
    Space::Math::Vec3 pos = m_Player->getWorldPosition();
    Space::Math::Vec3 dir = pos - m_TargetPoint;
    m_Distance = dir.length();
    dir = dir.normalize();
    dir =  Space::Math::rotate(0, 0, 1, -value) * dir;
    m_Player->setWorldPosition(m_TargetPoint + dir * m_Distance);
    m_Player->setWorldDirection(Space::Math::vec3(-dir), Space::Math::vec3(0, 0, 1));
}

void CameraLookat::vertical(float value)
{
    if(value == 0)
    {
        return;
    }
    Space::Math::Vec3 pos = m_Player->getWorldPosition();
    Space::Math::Vec3 dir = pos - m_TargetPoint;
    m_Distance = dir.length();
    dir = dir.normalize();
    dir =  Space::Math::rotate(m_Player->getWorldTransform().getColumn3(0), -value) * dir;

    //  计算垂直滑动时的旋转上限，禁止旋转过头
    Space::Math::vec3 calcDir(dir.x, dir.y, dir.z);
    float angle = Space::Math::getAngle(Space::Math::vec3(0, 0, 1), calcDir);
    if(angle<m_MinVerticalAngle)
    {
        value = -(m_MinVerticalAngle - angle);
        dir =  Space::Math::rotate(m_Player->getWorldTransform().getColumn3(0), -value) * dir;
    }
    if(angle > m_MaxVerticalAngle)
    {
        value = (angle - m_MaxVerticalAngle);
        dir =  Space::Math::rotate(m_Player->getWorldTransform().getColumn3(0), -value) * dir;
    }

    m_Player->setWorldPosition(m_TargetPoint + dir * m_Distance);
    m_Player->setWorldDirection(Space::Math::vec3(-dir), Space::Math::vec3(0, 0, 1));
}

void CameraLookat::setVertical(float value)
{
    Space::Math::Vec3 pos = m_Player->getWorldPosition();

    auto currentAngle = getVertical();
    auto rotAngle = value - currentAngle;
    vertical(rotAngle);
}

float CameraLookat::getVertical()
{
    auto currentAngle = Space::Math::getAngle(Space::Math::vec3(0,0,1), m_Player->getWorldDirection());
    return currentAngle-90;
}

void CameraLookat::setSlidingDamping(float damping)
{
    m_SlidingDamping = damping;
}

float CameraLookat::getSlidingDamping()
{
    return m_SlidingDamping;
}

void CameraLookat::setAutoRotationEnabled(bool autoRotation)
{
    m_AutoRotationEnabled = autoRotation;
}

bool CameraLookat::isAutoRotationEnabled()
{
    return m_AutoRotationEnabled;
}

void CameraLookat::refresh()
{
    auto dir = Space::Math::normalize(m_Player->getWorldPosition() - m_TargetPoint);
    auto pos = m_TargetPoint + dir *m_Distance;
    m_Player->setWorldPosition(pos);
    horizontal(0.0001 * Space::Game::get()->getIFps());
}
