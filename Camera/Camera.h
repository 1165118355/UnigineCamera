////////////////////////////
//
//  Wirter: David.zhu
//  Date:   2020-04-27
//  brief:  xxx
//

#ifndef CAMERA_H
#define CAMERA_H
#include <vector>
#include <shared_mutex>
#include <SpaceNodes.h>
#include <SpacePlayers.h>

class Camera
{
public:
    enum
    {
        CAMERA_NONE,
        CAMERA_EXHIBITION,
        CAMERA_FIXED,
        CAMERA_LOOKAT,
    };
public:
    /// \brief  狮子变猫的函数
    template<class T>
    static std::shared_ptr<Camera> cast(std::shared_ptr<T> value)
    {
        return std::dynamic_pointer_cast<Camera>(value);
    }

    /// \brief  相机的初始化
    virtual void init()=0;

    /// \brief  相机状态的刷新
    virtual void update()=0;

    /// \brief  终结相机Good Luck  d=====(￣▽￣*)b
    virtual void shutdown()=0;

    /// \brief  设置目标，相机将围绕这目标旋转（如果设置了固定模式，相机将不再旋转）
    virtual void setTarget(Space::NodePtr node);

    /// \brief  给这个相机策略设置相机
    void setPlayer(Space::PlayerPtr player);
    Space::PlayerPtr getPlayer();

    /// \brief  设置启用
    void setEnabled(bool enabled);
    bool isEnabled();

    /// \brief
    virtual void clear();
protected:
    /// \brief  猥琐的构造函数藏在子继承属性中
    Camera();
protected:
    int m_Type;
    Space::PlayerPtr m_Player;
    Space::NodePtr m_TargetNode;
    bool m_Enabled = true;
};
typedef std::shared_ptr<Camera> CameraPtr;

#endif // CAMERA_H
