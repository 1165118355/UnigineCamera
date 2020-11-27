////////////////////////////
//
//  Wirter: David.zhu
//  Date:   2020-04-27
//  brief:  xxx
//

#ifndef CAMERAFIXED_H
#define CAMERAFIXED_H
#include "Camera.h"
#include <memory>


class CameraFixed:public Camera
{
public:
    /// \brief  相机的初始化
    virtual void init();

    /// \brief  相机状态的刷新
    virtual void update();

    /// \brief  终结相机Good Luck  d=====(￣▽￣*)b
    virtual void shutdown();

    /// \brief  生个相机
    static std::shared_ptr<CameraFixed> create();
private:
    CameraFixed();
};

#endif // CAMERAFIXED_H
