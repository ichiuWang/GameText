#include "Paddle.h"

 Scene* Paddle::createScene(){
	 auto scene = Scene::createWithPhysics();
	 scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	 scene->getPhysicsWorld()->setGravity(Vect(0, -100));

	 auto layer = Paddle::create();
	 scene->addChild(layer);
	 return scene;
}

bool Paddle::init(){
	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();

	initPhysicWorld();

	createBall(visibleSize / 2);

	createPaddle(Point(400, 60));

	loadTaskScene();

	return true;
}

void Paddle::initPhysicWorld(){
	auto edgeSprite = Sprite::create();
	auto body = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
	edgeSprite->setPosition(Point(visibleSize / 2));
	edgeSprite->setPhysicsBody(body);
	addChild(edgeSprite);
}
//思想还是跟不上啊 这里直接(Point pos)
void Paddle::createBall(Point pos){
	m_ball = Sprite::create("Paddle/ball.png");
	int r = m_ball->getContentSize().width / 2;
	auto ballBody = PhysicsBody::createCircle(r, PHYSICS_MATERIAL_BALL_WORLD);
	m_ball->setPhysicsBody(ballBody);
	m_ball->setPosition(pos);
	addChild(m_ball, 1);
	//设定初始速度
	ballBody->setVelocity(Point(300, 300));
	//设定最大速度
	ballBody->setVelocityLimit(600);
}

void Paddle::createPaddle(Point pos){
	auto paddle = Sprite::create("Paddle/paddle.png");
	auto paddleBody = PhysicsBody::createBox(paddle->getContentSize(), PHYSICS_MATERIAL_BALL_WORLD);
	paddleBody->setDynamic(false);

	paddle->setPhysicsBody(paddleBody);
	paddle->setPosition(pos);
	addChild(paddle, 1);


	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *pTouch, Event *pEvent){return true; };
	listener->onTouchMoved = [=](Touch *pTouch, Event *pEvent){
		auto offset = pTouch->getDelta();
		auto newPos = paddle->getPosition() + offset;
		if (newPos.x<paddle->getContentSize().width/2)
		{
			newPos.x = paddle->getContentSize().width / 2;
		}
		else if (newPos.x>visibleSize.width-paddle->getContentSize().width/2)
		{
			newPos.x = visibleSize.width - paddle->getContentSize().width / 2;
		}
		paddle->setPosition(newPos.x, paddle->getPositionY());
	};
	listener->onTouchEnded = [=](Touch *pTouch, Event *pEvent){};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

}

void Paddle::loadTaskScene(){
	auto task1 = GUIReader::getInstance()->widgetFromJsonFile("Paddle/task1.json");
	task1->setAnchorPoint(Point::ANCHOR_MIDDLE);
	task1->setPosition(visibleSize / 2);
	task1->setScale(1.0);
	addChild(task1, 1);

	/*auto taskScene1 = cocostudio::SceneReader::getInstance()
		->createNodeWithSceneFile("Paddle/taskScene1.json");

	addChild(taskScene1, 1);
*/
}