#include "CameraFixed.h"
#include "Utils/UtilsEngine.h"
#include <SpaceApp.h>

void CameraFixed::init()
{

}

void CameraFixed::update()
{

    //  拿到鼠标上一帧与当前帧位置的偏移
    Space::ControlsApp::get()->setMouseEnabled(true);
    Space::ControlsApp::get()->setEnabled(true);
    float dx = Space::ControlsApp::get()->getMouseDX();
    float dy = Space::ControlsApp::get()->getMouseDY();

    //  确认轴向
    Space::Math::quat cameraRotation = m_Player->getWorldRotation();

    double ifps = Space::App::get()->getIFps();
    Space::Math::vec3 up = m_Player->getUp();
    Space::Math::Vec3 pos = m_Player->getWorldPosition();

    if(Space::App::get()->getMouseButtonState(Space::App::BUTTON_LEFT))
    {
        //  旋转
        int inclination = 0;
        Space::Math::quat iRotation = Space::Math::conjugate(cameraRotation);
        iRotation = Space::Math::quat(Space::Math::vec3(0,0,1), inclination * ifps) * Space::Math::quat(Space::Math::vec3(1,0,0),dy) * iRotation * Space::Math::quat(up,dx);
        cameraRotation = Space::Math::conjugate(iRotation);
    }

    //  最后的设置
    Space::Math::Mat4 imodelview = Space::Math::translate(pos) * Space::Math::Mat4(Space::Math::inverse(Space::Math::translate(0.0f,0.0f,0) * Space::Math::mat4(Space::Math::conjugate(cameraRotation))));
    m_Player->setWorldTransform(imodelview);
}

void CameraFixed::shutdown()
{

}

std::shared_ptr<CameraFixed> CameraFixed::create()
{
    std::shared_ptr<CameraFixed> instance(new CameraFixed());
    return instance;
}

CameraFixed::CameraFixed()
{
    m_Type = CAMERA_FIXED;
}
