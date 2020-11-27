#ifndef CAMERAFREE_H
#define CAMERAFREE_H

#include "Camera.h"

class CameraFree:public Camera
{
public:
    static std::shared_ptr<CameraFree> create();
    /// \brief  相机的初始化
    virtual void init();

    /// \brief  相机状态的刷新
    virtual void update();

    /// \brief  终结相机Good Luck  d=====(￣▽￣*)b
    virtual void shutdown();
protected:
    CameraFree();

    float m_mouseWheelSpeed = 1.5;
};

#endif // CAMERAFREE_H
