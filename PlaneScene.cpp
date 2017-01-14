#include "PlaneScene.h"



Scene* PlaneScene::createScene()
{
    auto scene = Scene::create();
	auto layer = PlaneScene::create();
    scene->addChild(layer);
    return scene;
}

bool PlaneScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
   visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Plane/plane.plist");
	
	//starGame();
	gameControl(Welcome);

    return true;
}

void PlaneScene::createPlayer(){
	m_player = Sprite::create();
	Vector<SpriteFrame*> playerSF;
	playerSF.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("Hero1.png"));
	playerSF.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("Hero2.png"));
	m_player->runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(playerSF, 0.1f))));
	m_player->setScale(0.3);
	m_player->setPosition(visibleSize / 2);
	addChild(m_player, 1);

	isLive = true;

	schedule(schedule_selector(PlaneScene::playerBullet), player_bullet_rate);

}
	
void PlaneScene::movePlayer(){
	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = [=](Touch *pTouch, Event *pEvent){return true; };
	listener->onTouchMoved = [=](Touch *pTouch, Event *pEvent){
		auto newPos = m_player->getPosition() + pTouch->getDelta();
		Size planeSize = m_player->boundingBox().size;
		if (newPos.x > visibleSize.width - planeSize.width / 2)
		{
			newPos.x = visibleSize.width - planeSize.width / 2;
		}
		else if (newPos.x < planeSize.width / 2)
		{
			newPos.x = planeSize.width / 2;
		}

		if (newPos.y > visibleSize.height - planeSize.height / 2) {
			newPos.y = visibleSize.height - planeSize.height / 2;
		}
		else if (newPos.y < planeSize.height / 2)
		{
			newPos.y = planeSize.height / 2;
		}
		m_player->setPosition(newPos);
	};
	listener->onTouchEnded = [=](Touch *pTouch, Event *pEvent){};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void PlaneScene::playerBullet(float dt){
	if (isLive)
	{
		Sprite *playerBul = Sprite::createWithSpriteFrameName("bullet1.png");
		playerBul->setPosition(m_player->getPosition());
		addChild(playerBul, 0);
		float t = (visibleSize.height - playerBul->getPositionY()) / player_bullet_speed;
		MoveTo*mov = MoveTo::create(t, Point(playerBul->getPositionX(), visibleSize.height));
		Action*act = Sequence::create(mov, CallFunc::create([=](){playerBul->removeFromParent(); }), NULL);
		playerBul->runAction(act);

		allplayerBul.pushBack(playerBul);
	}
}

void PlaneScene::createEnemy(float dt){
	Sprite *m_enemy = Sprite::create();
	Vector<SpriteFrame*> enemySF;
	enemySF.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("SmallEnemy1.png"));
	enemySF.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("SmallEnemy2.png"));
	m_enemy->runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(enemySF, 0.1f))));

	

	Size enemySize = m_enemy->boundingBox().size;
	int enemyX = rand() % (int)(visibleSize.width - enemySize.width) + enemySize.width / 2;
	int num = CCRANDOM_0_1() * 7;
	m_enemy->setPosition(100 + num * 100, visibleSize.height);
	float t = visibleSize.height / enemy_speed;
	MoveTo*mov1 = MoveTo::create(t, Point(m_enemy->getPositionX(), visibleSize.height*3/4));
	MoveTo*mov2 = MoveTo::create(t/2, Point(m_enemy->getPositionX(), 0));
	
	Action*act = Sequence::create(mov1, mov2,
		CallFunc::create([=](){m_enemy->removeFromParent(); }), NULL);
	m_enemy->runAction(act);

	addChild(m_enemy, 1);

	allEnemy.pushBack(m_enemy);
	createEnemyBullet();
	//m_enemy->schedule(schedule_selector(PlaneScene::createEnemyBullet), enemy_bullet_rate);
}

void  PlaneScene::moveEnemy(float dt){
	
}

void  PlaneScene::createEnemyBullet(){
	for (int i = 0; i < allEnemy.size();i++)
	{
		auto currentEnemy = allEnemy.at(i);
		Sprite * enemyBullet = Sprite::createWithSpriteFrameName("bullet2.png");
		enemyBullet->setPosition(currentEnemy->getPosition());
		addChild(enemyBullet, 0);
		float t = (enemyBullet->getPositionY()) / enemy_bullet_speed;
		MoveTo*mov = MoveTo::create(t, Point(enemyBullet->getPositionX(), 0));
		Action*act = Sequence::create(mov, CallFunc::create([=](){enemyBullet->removeFromParent(); }), NULL);
		enemyBullet->runAction(act);
		allEnemyBul.pushBack(enemyBullet);
	}

	//Sprite * enemyBullet = Sprite::createWithSpriteFrameName("bullet2.png");

	//	enemyBullet->setPosition(this->getPosition());

	//	getParent()->addChild(enemyBullet, 0);
	//	float t = (enemyBullet->getPositionY()) / enemy_bullet_speed;
	//	MoveTo*mov = MoveTo::create(t, Point(enemyBullet->getPositionX(), 0));
	//	Action*act = Sequence::create(mov, CallFunc::create([=](){enemyBullet->removeFromParent(); }), NULL);
	//	enemyBullet->runAction(act);
	
}

void PlaneScene::createBoom(int x, int y){
	Vector<SpriteFrame*>allFrame;
	for (int a = 0; a < 5; a++)
	{
		SpriteFrame*sf = SpriteFrame::create("Plane/boom.png", Rect(0, 98 * a, 126, 98));
		allFrame.pushBack(sf);
	}
	Animation*ani = Animation::createWithSpriteFrames(allFrame, 0.05f);
	auto sp = Sprite::create();
	Action *act = Sequence::create(
		Animate::create(ani),
		CallFuncN::create(sp, callfuncN_selector(PlaneScene::endBoom)), nullptr);


	sp->runAction(act);
	sp->setPosition(x, y);
	addChild(sp, 2);
}
void PlaneScene::endBoom(Node*node){
	node->removeFromParentAndCleanup(true);
}

void PlaneScene::update(float dt){

	if (isLive)
	{
		for (int i = 0; i < allplayerBul.size(); i++)
		{
			auto currentPlayerBul = allplayerBul.at(i);
			auto bulSize = currentPlayerBul->boundingBox().size;
			Rect playerBulRect(currentPlayerBul->getPositionX(), currentPlayerBul->getPositionY(),
				bulSize.width, bulSize.height);

			for (int j = 0; j < allEnemy.size(); j++)
			{
				auto currentEnemy = allEnemy.at(j);
				auto eneSize = currentEnemy->boundingBox().size;
				Rect enemyRect(currentEnemy->getPositionX(), currentEnemy->getPositionY(),
					eneSize.width, eneSize.height);
				if (playerBulRect.intersectsRect(enemyRect))
				{
					if (currentPlayerBul != nullptr)
					{
						currentPlayerBul->removeFromParent();
						allplayerBul.eraseObject(currentPlayerBul);

						createBoom(currentEnemy->getPositionX(), currentEnemy->getPositionY());
						currentEnemy->removeFromParent();
						allEnemy.eraseObject(currentEnemy);
						
						addScore();
					}
				}
			}
			/*for (int j = 0; j < allEnemyBul.size(); j++)
			{
			auto currentEnemyBul = allEnemyBul.at(j);
			auto eneBulSize = currentEnemyBul->boundingBox().size;
			Rect enemyBulRect(currentEnemyBul->getPositionX(), currentEnemyBul->getPositionY(),
			eneBulSize.width, eneBulSize.height);
			if (playerBulRect.intersectsRect(enemyBulRect))
			{
			if (currentPlayerBul != nullptr)
			{
			currentPlayerBul->removeFromParent();
			allplayerBul.eraseObject(currentPlayerBul);

			currentEnemyBul->removeFromParent();
			allEnemy.eraseObject(currentEnemyBul);
			}

			}
			}*/
		}
		//���ײ�ϵл�
		for (int i = 0; i < allEnemy.size(); i++)
		{
			auto currentEnemy = allEnemy.at(i);
			auto eneSize = currentEnemy->boundingBox().size;
			Rect enemyRect(currentEnemy->getPositionX(), currentEnemy->getPositionY(),
				eneSize.width, eneSize.height);

			auto playerSize = m_player->boundingBox().size;
			Rect playerRext(m_player->getPositionX(), m_player->getPositionY(),
				playerSize.width, playerSize.height);

			if (playerRext.intersectsRect(enemyRect))
			{

				createBoom(currentEnemy->getPositionX(), currentEnemy->getPositionY());
				currentEnemy->removeFromParent();
				allEnemy.eraseObject(currentEnemy);

				createBoom(m_player->getPositionX(), m_player->getPositionY());
				damagePlayer();
			}
		}
		//���ײ�ϵл��ӵ�
		for (int i = 0; i < allEnemyBul.size(); i++)
		{
			auto currentEnemyBul = allEnemyBul.at(i);
			auto eneBulSize = currentEnemyBul->boundingBox().size;
			Rect enemyBulRect(currentEnemyBul->getPositionX(), currentEnemyBul->getPositionY(),
				eneBulSize.width, eneBulSize.height);

			auto playerSize = m_player->boundingBox().size;
			Rect playerRext(m_player->getPositionX(), m_player->getPositionY(),
				playerSize.width, playerSize.height);

			if (enemyBulRect.intersectsRect(playerRext))
			{
				
				createBoom(currentEnemyBul->getPositionX(), currentEnemyBul->getPositionY());
				currentEnemyBul->removeFromParent();
				currentEnemyBul->setPosition(0, 0);
				//���������һ�¾��������⡣����
				//�ӵ�����û���� ֻ�ǿ������� ..���Ծ������������ʱ����
				//���������Ӧ��ֻ�ǴӸ��ڵ��Ƴ��� �����Ǹ�λ��
				//�л�Ҳ��������⣬�����¿�ʼʱ Ϊ����ĵл���Ȼ�����Ǹ�λ�ã�ֻ�ǿ������� ���ᷢ���ӵ�ɶ��
				allEnemy.eraseObject(currentEnemyBul);
				//i--;

				
				//m_player->setVisible(false);
				damagePlayer();
				
				//isLive = false;
				//

			}
		}
	}
	else
	{
		/*auto gameOverDlg = GUIReader::getInstance()->widgetFromJsonFile("Plane/GameOverDlg/GameOverDlg.json");
		gameOverDlg->setAnchorPoint(Point(0.5,0.5));
		gameOverDlg->setPosition(visibleSize / 2);
		gameOverDlg->setScale(0.5);
		addChild(gameOverDlg, 3);
		
		auto restart = dynamic_cast<Widget*>(gameOverDlg->getChildByName("background")->getChildByName("Restart"));
		auto exit = dynamic_cast<Widget*>(gameOverDlg->getChildByName("background")->getChildByName("Exit"));

		restart->addTouchEventListener(this, toucheventselector(PlaneScene::touchButton));
		exit->addTouchEventListener(this, toucheventselector(PlaneScene::touchButton));*/
		
		//restart->addTouchEventListener(CC_CALLBACK_1(PlaneScene::touchButton, this));
		//exit->addTouchEventListener(CC_CALLBACK_1(PlaneScene::touchButton, this));
	//this->unscheduleUpdate();  
		//�ڼ���void gameControl(GameState gamestate);�� 
	//��this->unscheduleAllSelectors();����result����ȥ�� Ҳ�� ���������һ��
	}
}

void PlaneScene::createBG(){
	bg1 = Sprite::create("Plane/background.png");
	bg2 = Sprite::create("Plane/background.png");
	bg1->setPosition(visibleSize / 2);
	bg1->setAnchorPoint(Point(0.5, 0.5));
	bg1->setScaleX(visibleSize.width / bg1->getContentSize().width );
	bg1->setScaleY(visibleSize.height / bg1->getContentSize().height);
	bg2->setPosition(visibleSize.width / 2, visibleSize.height * 3 / 2);
	bg2->setScaleX(visibleSize.width / bg1->getContentSize().width);
	bg2->setScaleY(visibleSize.height / bg1->getContentSize().height);
	addChild(bg1, -1);
	addChild(bg2, -1);
	schedule(schedule_selector(PlaneScene::moveBG), 0.01f);
}

void PlaneScene::moveBG(float dt){
	if (bg1->getPositionY()<-visibleSize.height/2)
	{
		bg2->setPositionY(visibleSize.height * 3 / 2);
		bg1->setPositionY(visibleSize.height / 2);
	}
	bg1->setPositionY(bg1->getPositionY() - 5);
	bg2->setPositionY(bg2->getPositionY() - 5);
}



void PlaneScene::starGame(){
	createPlayer();
	createBG();
	movePlayer();
	
	schedule(schedule_selector(PlaneScene::createEnemy), 1.0f);

	scheduleUpdate();

	m_gameState = Game;
	//---------------------------------------HUD
	auto hud = GUIReader::getInstance()->widgetFromJsonFile("Plane/HUD/HUD.json");
	auto offset = Point(0, visibleSize.height - hud->getContentSize().height);//�����������Ȥ��
	hud->setPosition(offset);
	//addChild(hud, 0);//����ƺ��ᵲס�ҵĴ������ư�
	//hud->setAnchorPoint(Point(0.5,0.5));
	//hud->setPosition(visibleSize / 2);
	addChild(hud, 3);

	m_lifeBar = dynamic_cast<LoadingBar*>(hud->getChildByName("LifeBar"));
	m_scoreNum = dynamic_cast<TextAtlas*>(hud->getChildByName("ScoreNum"));

	m_scoreNum->setStringValue("0");
	m_lifeBar->setPercent(100);
}
//-------------------------------------------------------------------
void PlaneScene::damagePlayer(){
	if (m_gameState == Game)
	{
		int precent = m_lifeBar->getPercent();
		precent -= 20;
		m_lifeBar->setPercent(precent);
		if (precent <= 0)
		{
			isLive = false;
			m_player->setVisible(false);
			//
			gameControl(Result);
		}
	}
}

void PlaneScene::addScore(){
	if (m_gameState == Game)
	{
		auto  str = m_scoreNum->getStringValue();
		int num = atoi(str.c_str());
		num += 15;
		char add_str[256] = { 0 };
		sprintf(add_str, "%d", num);
		m_scoreNum->setStringValue(add_str);
	}
}

void PlaneScene::gameControl(GameState gamestate){
	switch (gamestate)
	{
	case GameState::Welcome:{
								m_gameState = Welcome;
								
								setWelcome();
								break;
	}
	case GameState::Game:{
							 m_gameState = Game;//�������б�Ҫд��
							 removeAllChildren();
							 Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
							
							 starGame();
							 break;
	}

	case GameState::HighScore:{
								  m_gameState = HighScore;
								  //CCLOG("HighScore");

								  setHighScore();

								  break;
	}
	case GameState::Result:{
							   m_gameState = Result;
							   //����ͣ����Ϸ��һЩ����
							   this->unscheduleAllSelectors();
							   Director::getInstance()->getEventDispatcher()
								   ->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);

							   setResult();

							   break;
	}

	default:{
				break;
	}
	}
}

void PlaneScene::setWelcome(){
	auto welcome = GUIReader::getInstance()->widgetFromJsonFile("Plane/StarGame/StarGame.json");
	welcome->setAnchorPoint(Point::ANCHOR_MIDDLE);
	welcome->setPosition(visibleSize / 2);

	addChild(welcome, 3);

	auto btnStart = dynamic_cast<Widget*>(welcome->getChildByName("StartGame"));
	auto btnHighScore = dynamic_cast<Widget*>(welcome->getChildByName("HighScore"));

	btnStart->addTouchEventListener(this, toucheventselector(PlaneScene::touchButton));
	btnHighScore->addTouchEventListener(this, toucheventselector(PlaneScene::touchButton));
}

void PlaneScene::setResult(){
	if (m_player)
	{
		m_player->removeFromParent();
	}

	m_resultDlg = GUIReader::getInstance()->widgetFromJsonFile("Plane/Result/Result.json");
	//m_resultDlg->setScale(0.5);
	m_resultDlg->setAnchorPoint(Point::ANCHOR_MIDDLE);
	m_resultDlg->setPosition(visibleSize / 2);
	addChild(m_resultDlg, 3);

	int score = UserDefault::getInstance()->getIntegerForKey("High_Score");
	int currentScore = atoi(m_scoreNum->getStringValue().c_str());
	if (score < currentScore)
	{
		setResultDlg(true);
	}
	else
	{
		setResultDlg(false);
	}
}

void PlaneScene::setResultDlg(bool isBreakResult){
	auto btnStart = dynamic_cast<Widget*>(m_resultDlg->getChildByName("StartGame"));
	auto btnSave = dynamic_cast<Widget*>(m_resultDlg->getChildByName("SaveName"));
	auto btnHighScore = dynamic_cast<Widget*>(m_resultDlg->getChildByName("HighScore"));

	auto lblBreakResult = m_resultDlg->getChildByName("BreakResult");
	auto lblUnBreakResult = m_resultDlg->getChildByName("UnBreakResult");

	auto scoreNum = m_resultDlg->getChildByName("ScoreNum");
	dynamic_cast<TextAtlas*>(scoreNum)->setStringValue(m_scoreNum->getStringValue());
	
	m_input = dynamic_cast<TextField*>(m_resultDlg->getChildByName("Input"));

	if (isBreakResult)
	{
		lblBreakResult->setVisible(true);
		lblUnBreakResult->setVisible(false);
		btnHighScore->setVisible(false);
		btnStart->setVisible(false);
		btnSave->setVisible(true);

		m_input->setVisible(true);
		m_input->setEnabled(true);
		
		btnSave->addTouchEventListener(this, toucheventselector(PlaneScene::touchButton));
	}
	else
	{
		btnSave->setVisible(false);
		m_input->setVisible(false);
		btnSave->setEnabled(false);
		m_input->setEnabled(false);

		lblBreakResult->setVisible(false);
		lblUnBreakResult->setVisible(true);
		btnHighScore->setVisible(true);
		btnStart->setVisible(true);
		
		
		btnStart->addTouchEventListener(this, toucheventselector(PlaneScene::touchButton));
		btnHighScore->addTouchEventListener(this, toucheventselector(PlaneScene::touchButton));
	}
}

void PlaneScene::setHighScore(){
	//removeAllChildren();
	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	auto highScore = GUIReader::getInstance()->widgetFromJsonFile("Plane/HighScore/HighScore.json");
	//highScore->setScale(0.5);
	highScore->setPosition(visibleSize / 2);
	highScore->setAnchorPoint(Point::ANCHOR_MIDDLE);
	addChild(highScore, 3);

	auto num = dynamic_cast<TextAtlas*>(highScore->getChildByName("HighScoreBk")->getChildByName("ScoreNum"));
	auto name = dynamic_cast<Text*>(highScore->getChildByName("HighScoreBk")->getChildByName("UserName"));
	auto start = dynamic_cast<Widget*>(highScore->getChildByName("HighScoreBk")->getChildByName("StartGame"));

	name->setText(UserDefault::getInstance()->getStringForKey("High_Name", "no people"));
	// UserDefault::getStringForKey(const char* pKey, const std::string & defaultValue)
	//Ĭ��ֵ�� û�ҵ�����Ĭ��ֵ
	int scoreNum = UserDefault::getInstance()->getIntegerForKey("High_Score", 0);
	char scoreStr[256] = { 0 };
	sprintf(scoreStr, "%d", scoreNum);
	num->setStringValue(scoreStr);
	
	 start->addTouchEventListener(this, toucheventselector(PlaneScene::touchButton));

}

void PlaneScene::touchButton(Ref *object, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		auto widget = dynamic_cast<Widget*>(object);
		auto name = widget->getName();
		if (name == "Exit")
			//  if (name.compare("ExitGame") == 0) ���ϵ�ʵ�ַ�ʽ
		{
			Director::getInstance()->end();//��Щ��ʵ�����Է�����Ϸ��������ȥ

		}
		//else if (name == "Restart")
		//{
		//	removeAllChildren();//����±���� ���򲻻Ὺʼ
		//	//û�����������Ӧ��ܹ���
		//	//����Ҫ�о��°� ��� �������� ����
		//	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
		//	//��ʵ���ڰ�׿����Ȼ��һ������ 
		//	//1���ӵ���û��������λ����Ȼ��
		//	//2��������������
		//	starGame();
		//}
		else if (name == "StartGame")
		{
			gameControl(Game);
		}
		else if (name == "HighScore")
		{

			//auto name = m_input->getStringValue();
			//auto score = m_scoreNum->getStringValue();

			//CCLOG("input name is %s,score is %s", name.c_str(), score.c_str());
			////save
			//UserDefault::getInstance()->setStringForKey("High_Name", name);
			////UserDefault::getInstance()->setStringForKey("High_Score", atoi(score.c_str()));
			//UserDefault::getInstance()->setIntegerForKey("High_Score", atoi(score.c_str()));
			//UserDefault::getInstance()->flush();
			gameControl(HighScore);

			//setResultDlg(false);
		}
		else if (name == "SaveName")
		{
			auto name = m_input->getStringValue();
			auto score = m_scoreNum->getStringValue();

			CCLOG("input name is %s,score is %s", name.c_str(), score.c_str());
			//save
			UserDefault::getInstance()->setStringForKey("High_Name", name);
			//UserDefault::getInstance()->setStringForKey("High_Score", atoi(score.c_str()));
			UserDefault::getInstance()->setIntegerForKey("High_Score", atoi(score.c_str()));
			UserDefault::getInstance()->flush();
			gameControl(HighScore);

			setResultDlg(false);
		}
	}
}

