#ifndef __PLANESCENE_H__
#define __PLANESCENE_H__

#include "cocos2d.h"
USING_NS_CC;

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocostudio;
using namespace ui;

const float player_bullet_speed = 300;
const float player_bullet_rate = 0.7f;

const float enemy_speed = 200;
const float enemy_second_speed = 200 * 2;
const float enemy_bullet_speed = 450;
const float enemy_bullet_rate = 0.8f;

enum GameState
{
	Welcome = 0,
	Game,
	HighScore,
	Result
};


class PlaneScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();  
	CREATE_FUNC(PlaneScene);

	void createPlayer();
	void movePlayer();
	void playerBullet(float dt);

	void createEnemy(float dt);
	void moveEnemy(float dt);
	void createEnemyBullet();

	void createBoom(int x,int y);
	void endBoom(Node*node);

	void createBG();
	void moveBG(float dt);

	void update(float dt);

	void starGame();//通过把所有写在一个开始里面来方便重新加载

	void touchButton(Ref *object, TouchEventType type);

	//--------
	void gameControl(GameState gamestate);

	void damagePlayer();

	void addScore();
	//游戏结束跳转结果界面
	void setResult();
	//是否打破结果
	void setResultDlg(bool isBreakResult);
	//开始界面
	void setWelcome();
	//最高分界面
	void setHighScore();

private:
	Size visibleSize;

	Sprite*m_player;

	bool isLive;

	Vector<Sprite*>allEnemy;
	Vector<Sprite*>allEnemyBul;
	Vector<Sprite*>allplayerBul;

	Sprite*bg1;
	Sprite*bg2;

	//
	GameState m_gameState;

	LoadingBar*m_lifeBar;
	TextAtlas*m_scoreNum;

	Widget*m_resultDlg;

	TextField*m_input;
};

#endif
