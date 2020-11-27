////////////////////////////
//
//  Wirter: David.zhu
//  Date:   2020-04-27
//  brief:  xxx
//

#ifndef CAMERAEXHIBITION_H
#define CAMERAEXHIBITION_H
#include "Camera.h"
#include <memory>

class CameraExhibition:public Camera
{
public:
    enum
    {
        AXIS_X=1,
        AXIS_Y=2,
    };
public:
    static std::shared_ptr<CameraExhibition> create();

    /// \brief  相机的初始化
    virtual void init();

    /// \brief  相机状态的刷新
    virtual void update();

    /// \brief  终结相机Good Luck  d=====(￣▽￣*)b
    virtual void shutdown();

    /// \brief  TTTTTTTarget!!!
    void setTarget(Space::NodePtr node);

    /// \brief  重置固定相机改变Target的变换矩阵
    void recordExhibitionTransform();
    void resetExhibitionTransform();

    /// \brief  设置可以操作的轴
    void setAxis(int axis);
    int getAxis();
private:
    CameraExhibition();
private:
    Space::Math::Mat4 m_ExhibitioTransformFront;
    int m_Axis;
};

#endif // CAMERAEXHIBITION_H
