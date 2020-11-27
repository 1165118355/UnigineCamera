#include "CameraFree.h"
#include <SpaceApp.h>

std::shared_ptr<CameraFree> CameraFree::create()
{
    return std::shared_ptr<CameraFree>(new CameraFree());
}

void CameraFree::init()
{

}

void CameraFree::update()
{
    if(!m_Enabled)
    {
        return ;
    }

    Space::Math::Vec3 position = m_Player->getWorldPosition();

    //  拿到鼠标上一帧与当前帧位置的偏移
    Space::ControlsApp::get()->setEnabled(true);
    Space::ControlsApp::get()->setMouseEnabled(true);
    float dx = Space::ControlsApp::get()->getMouseDX();
    float dy = Space::ControlsApp::get()->getMouseDY();

    //  确认轴向
    Space::Math::quat cameraRotation = m_Player->getWorldRotation();
    Space::Math::vec3 x = cameraRotation * Space::Math::vec3(1.0f, 0.0f, 0.0f);
    Space::Math::vec3 y = cameraRotation * Space::Math::vec3(0.0f, 1.0f, 0.0f);
    Space::Math::vec3 z = cameraRotation * Space::Math::vec3(0.0f, 0.0f, 1.0f);

    double ifps = Space::App::get()->getIFps();
    Space::Math::vec3 up = m_Player->getUp();
    Space::Math::Vec3 pos = m_Player->getWorldPosition();
    Space::Math::vec3 impulse = Space::Math::vec3();

    float speed = m_mouseWheelSpeed;
    if(Space::App::get()->getKeyState(Space::App::KEY_SHIFT))
    {
        speed*=4;
    }

    static Space::Math::Vec3 rotatePoint;
    if(Space::App::get()->getKeyState(Space::App::KEY_ALT) && Space::App::get()->getMouseButtonState(Space::App::BUTTON_MIDDLE))
    {
        //  中键平移
        impulse += x * (-dx);
        impulse += y * (dy);
    }
    else if(Space::App::get()->getKeyState(Space::App::KEY_ALT) && Space::App::get()->getMouseButtonState(Space::App::BUTTON_RIGHT) )
    {
        //  右键缩放
        impulse -= z * (dy - (-dx));
    }
    else if(Space::App::get()->getMouseButtonState(Space::App::BUTTON_RIGHT) )
    {
        //  旋转
        int inclination = 0;
        if(Space::App::get()->getKeyState(Space::App::KEY_LEFT))
        {
            inclination = -180;
        }
        else if(Space::App::get()->getKeyState(Space::App::KEY_RIGHT))
        {
            inclination = 180;
        }
        Space::Math::quat iRotation = Space::Math::conjugate(cameraRotation);
        iRotation = Space::Math::quat(Space::Math::vec3(0,0,1), inclination * ifps) * Space::Math::quat(Space::Math::vec3(1,0,0),dy / 2) * iRotation * Space::Math::quat(up,dx / 2);
        cameraRotation = Space::Math::conjugate(iRotation);
    }

    //  获取用户输入
    if(Space::App::get()->getKeyState('w'))
    {// 向前
        impulse -= z;
    }
    if(Space::App::get()->getKeyState('s'))
    {// 向后
        impulse += z;
    }
    if(Space::App::get()->getKeyState('a'))
    {// 向左
        impulse -= x;
    }
    if(Space::App::get()->getKeyState('d'))
    {// 向右
        impulse += x;
    }
    if(Space::App::get()->getKeyState('q'))
    {// 向上
        impulse -= y;
    }
    if(Space::App::get()->getKeyState('e'))
    {// 向下
        impulse += y;
    }


    //  鼠标滑轮缩放
    if(Space::App::get()->getMouseAxis(Space::App::AXIS_Y) )
    {
        float distance = Space::App::get()->getMouseAxis(Space::App::AXIS_Y) * Space::App::get()->getIFps() * speed;
        pos += Space::Math::Vec3(Space::Math::normalize(m_Player->getViewDirection()) * distance);
        m_Player->setWorldPosition(pos);
    }

    //  移动
    impulse = Space::Math::normalize(impulse) * speed;
    pos += Space::Math::Vec3(impulse) * ifps;

    //  最后的设置
    Space::Math::Mat4 imodelview = Space::Math::translate(pos) * Space::Math::Mat4(Space::Math::inverse(Space::Math::translate(0.0f,0.0f, 0) * Space::Math::mat4(Space::Math::conjugate(cameraRotation))));
    m_Player->setWorldTransform(imodelview);
}

void CameraFree::shutdown()
{

}

CameraFree::CameraFree()
{

}
