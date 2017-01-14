#ifndef __PADDLE_H__
#define __PADDLE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;

const PhysicsMaterial PHYSICS_MATERIAL_BALL_WORLD(1.0f, 1.0f, 0.0f);

class Paddle : public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(Paddle);

	void initPhysicWorld();

	void createBall(Point pos);
	
	void createPaddle(Point pos);

	void loadTaskScene();
private:
	Size visibleSize;

	Sprite*m_ball;

};

#endif 