#include "Piano.h"




Scene* Piano::createScene()
{
	auto scene = Scene::create();
	auto layer = Piano::create();
	scene->addChild(layer);
	return scene;
}

bool Piano::init()
{
	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	starPiano();
	//auto input = dynamic_cast<TextField*>(widget->getChildByTag(101));
	//input->addEventListenerTextField(this,textfieldeventselector(Piano::textInput));
	return true;
}

void Piano::starPiano(){
	readWIdgt();
	addParticle();
	menuButton();
}

void Piano::readWIdgt(){
	auto widget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("Piano/Piano.json");
	addChild(widget, 0);

	auto main = widget->getChildByName("Main");
	for (int i = 10; i < 70; ++i)
	{
		auto buttonWidge = dynamic_cast<Button*>(main->getChildByTag(i));
		if (buttonWidge)
		{
			buttonWidge->addTouchEventListener(CC_CALLBACK_2(Piano::touchButton, this));
		}
	}
}

void Piano::touchButton(Ref*object, Widget::TouchEventType type){

	auto button = dynamic_cast<Button*>(object);
	if (button && type==Widget::TouchEventType::BEGAN)
	{
		char buf[32] = { 0 };
		std::string name = button->getName();
		sprintf(buf, "Piano/sound/%s.wav", name.c_str());
		log("sound name is:%s", buf);		
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(buf);
	}
}

void Piano::addParticle(){

	ParticleSystemQuad*particle = ParticleSystemQuad::create("Piano/Particle/slowLight.plist");
	addChild(particle, 1);
}

void Piano::menuButton(){
	auto returnMenu = MenuItemImage::create(
		"Piano/backA.png",
		"Piano/backB.png",
		CC_CALLBACK_1(Piano::menuCloseCallback, this));
	returnMenu->setPosition(Vec2(origin.x + visibleSize.width - returnMenu->getContentSize().width / 2,
		origin.y + returnMenu->getContentSize().height / 2));

	auto menu = Menu::create(returnMenu, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
}

void Piano::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();
}

//void Piano::textInput(Ref*object, ui::TextFiledEventType type){
//	switch (type)
//	{
//	case TEXTFIELD_EVENT_ATTACH_WITH_IME:
//	{
//	TextField*textField = dynamic_cast<TextField*>(object);
//	std::string words = textField->getStringValue();
//	TextBMFont*label = dynamic_cast<TextBMFont*>(textField->getParent()->getChildByTag(100));
//	label->setText(words.c_str());
//	textField->setText("");
//	}
//		break;
//	default:break;
//	}
//}