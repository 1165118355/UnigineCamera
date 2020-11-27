////////////////////////////
//
//  Wirter: David.zhu
//  Date:   2020-04-27
//  brief:  xxx
//

#ifndef CAMERALOOKAT_H
#define CAMERALOOKAT_H
#include "Camera.h"
#include <memory>
#include <SpaceNodes.h>
#include <SpacePlayers.h>
#include <SpaceApp.h>


class CameraLookat:public Camera
{
public:
    /// \brief  生个相机
    static std::shared_ptr<CameraLookat> create();

    /// \brief  稚嫩的初始化
    virtual void init();

    /// \brief  疯狂的刷新，每帧刷
    virtual void update();

    /// \brief  终结
    virtual void shutdown();

    /// \brief  给相机设置目标
    void setTarget(Space::NodePtr node);
    void setTarget(Space::Math::Vec3 targetPosition, float distance);
    Space::Math::Vec3 getTargetPoint(){return m_TargetPoint;}

    /// \brief  缩放
    void zoom(float value);
    void setZoomEnabled(bool enabled);

    /// \brief  进行旋转
    void horizontal(float value);
    void vertical(float value);

    /// \brief  设置角度值
    void setVertical(float value);
    float getVertical();

    /// \brief  设置滑动阻尼，松开相机后仍然进行滑动
    void setSlidingDamping(float damping);
    float getSlidingDamping();

    /// \brief  设置自动旋转
    void setAutoRotationEnabled(bool autoRotation);
    bool isAutoRotationEnabled();

    /// \brief  距离相关设置
    void setMinDistance(double minDistance){m_MinDistance = minDistance;}
    void setMaxDistance(double maxDistance){m_MaxDistance = maxDistance;}
    double getDistance(){return m_Distance;}

    /// \brief  设置最大和最小角度
    void setMinVerticalAngle(float angle){m_MinVerticalAngle = angle;}
    void setMaxVerticalAngle(float angle){m_MaxVerticalAngle = angle;}

    void refresh();
private:
    CameraLookat();

    int m_RotationButton = Space::App::BUTTON_RIGHT;
    int m_MoveButton = Space::App::BUTTON_LEFT;
    float m_Distance;
    float m_MinDistance;
    float m_MaxDistance;

    float m_MinVerticalAngle = 10;
    float m_MaxVerticalAngle = 170;

    float m_SlidingDamping = 5;

    Space::Math::Vec3 m_TargetPoint;


    float m_HorizontalTellValue=0;
    float m_VerticalTellValue=0;
    bool m_IsSliding = false;
    bool m_IsZoom = true;
    bool m_AutoRotationEnabled = true;
    bool m_AutoRotation = false;
    float m_AutoRotationTime = 0;
    float m_AutoRotationTimeMax = 6;
    float m_AutoRotationVelocity = 10;
};
typedef std::shared_ptr<CameraLookat> CameraLookatPtr;

#endif // CAMERALOOKAT_H
