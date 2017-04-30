#include <jni.h>
#include "Substrate.h"

#include "mcpe/client/ClientInstance.h"
#include "mcpe/client/MinecraftGame.h"
#include "mcpe/client/gui/TButton.h"
#include "mcpe/client/gui/screen/Screen.h"
#include "mcpe/client/gui/screen/ScreenChooser.h"
#include "mcpe/level/Explosion.h"

#include "screen/ExtendsScreen.h"

//create screen when explosion happened

MinecraftGame* mcGame = 0;

static void (*explode_)();
static void explode()
{
	explode_();
	
	static bool mode=false;
	
	if(mode&&mcGame)
	{
		//define screen directly
		std::shared_ptr<Screen>scr=std::make_shared<Screen>(*mcGame,*mcGame->getPrimaryClientInstance());
		
		//demo button
		std::shared_ptr<Touch::TButton>btn=std::make_shared<Touch::TButton>(0,"customscreen.button.demo",scr->mcGame,false,0);
		btn->xPosition=0;
		btn->yPosition=0;
		btn->width=100;
		btn->height=20;
		scr->buttonList.push_back(btn);
		
		mcGame->getScreenChooser()->_pushScreen(scr,false);
	}
	else
	{
		//define screen by extending
		std::shared_ptr<ExtendsScreen>scr=std::make_shared<ExtendsScreen>(*mcGame,*mcGame->getPrimaryClientInstance());
		mcGame->getScreenChooser()->_pushScreen(scr,false);
	}
	mode=!mode;
}

static void (*initMCGame_)(MinecraftGame*);
static void initMCGame(MinecraftGame*c)
{
	initMCGame_(c);
	
	mcGame=c;
}

JNIEXPORT jint JNI_OnLoad(JavaVM*,void*)
{
	MSHookFunction((void*)&Explosion::explode,(void*)&explode,(void**)&explode_);
	MSHookFunction((void*)&MinecraftGame::_initMinecraftGame,(void*)&initMCGame,(void**)&initMCGame_);
	return JNI_VERSION_1_6;
}
