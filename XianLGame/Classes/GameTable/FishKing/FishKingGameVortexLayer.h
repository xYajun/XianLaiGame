#ifndef __YZ_FishKing__VORTEXLAYER_H
#define __YZ_FishKing__VORTEXLAYER_H

#include "cocos2d.h"
#include "3d/CCMesh.h"
#include "YZUIExport.h"
#include "YZBaseType.h"

USING_NS_CC;

namespace FishKing
{

//旋涡显示类
class VortexLayer : public  YZ::YZLayer
{
	//顶点结构
	struct VortexVertexStruct
	{
		Vec3 Position;
		Vec2 TexUV;
	};

public:
	VortexLayer();
	~VortexLayer();
	//初始化当前层
    virtual bool init();  
    CREATE_FUNC(VortexLayer);
	//重载渲染
	virtual void draw(Renderer* renderer, const Mat4 &transform, uint32_t flags);
    // a selector callback
    void vortexCloseCallback(float dt);
public:
	//设置渲染目标层
	void setTargetLayer(Layer*	pLayer)
	{
		m_TargetLayer = pLayer;
		if(m_pTarget)
		{
			m_pTarget->addChild(m_TargetLayer);
		}
	}
	//设置旋转的角度
	void setAngle( float f )
	{
		m_angle = f;
	}
	float getAngel() const
	{
		return m_angle;
	}
	//设置旋转的半径
	void setRadius( float f )
	{
		m_radius = f;
	}
	float getRadius()
	{
		return m_radius;
	}

	float						m_angleSpeed;	//角度速度
	float						m_radiusSpeed;	//半径速度
	float						m_Time;			//当前时间
	float						m_maxTime;		//最大时间

private:

	float						m_angle;		//角度
	float						m_radius;		//半径
	Mesh*						m_RenderMesh;	//渲染模型
	MeshCommand*				m_pMeshCommand;	//渲染队列
	BlendFunc					m_BlendFunc;	//渲染状态
    VortexVertexStruct*			m_VertexArray;	//所用的模型顶点
	GLushort*					m_IndiceArray;	//索引
	GLProgramState*			    m_ProgramState;	//Shader管理器
	Layer*						m_TargetLayer;	//目标
    RenderTexture*				m_pTarget;		//渲染目标
	timeval						m_lasttime;		//记录上次时间

	GLuint						m_vertexBuffer;
	GLuint						m_indexBuffer;
	GLenum						m_primitiveType;
	GLenum						m_indexFormat;
	ssize_t						m_indexCount;
	//RenderState::StateBlock*	m_stateBlock;

public:
	typedef std::function<void ()> CloseCallBack;
	CloseCallBack  onCloseCallBack;
};
}
#endif //__FishKing__VORTEXLAYER_H