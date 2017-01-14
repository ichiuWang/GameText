#ifndef __PIANO_H__
#define __PIANO_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;

class Piano : public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	void menuCloseCallback(Ref* pSender);
	CREATE_FUNC(Piano);
	void starPiano();

	void readWIdgt();
	void touchButton(Ref*object,Widget::TouchEventType type);
	void addParticle();
	void menuButton();

	//void textInput(Ref*object, TextFiledEventType type);
private:
	Size visibleSize ;
	Vec2 origin;
};

#endif 