#include "ExtendsScreen.h"

#include "mcpe/client/gui/TButton.h"
#include "mcpe/client/gui/screen/ScreenChooser.h"
#include "mcpe/client/ClientInstance.h"
#include "mcpe/client/MinecraftGame.h"

ExtendsScreen::ExtendsScreen(MinecraftGame&g,ClientInstance&c):Screen(g,c)
{
	

}
std::string ExtendsScreen::getScreenName() const
{
	return "custom_extends_screen";
}
std::string ExtendsScreen::getScreenNameW() const
{
	return "custom_extends_screen";
}
void ExtendsScreen::init()
{
	Screen::init();
	
	btn=std::make_shared<Touch::TButton>(0,"customscreen.button.exit",mcGame,false,0);
	btn->xPosition=0;
	btn->yPosition=0;
	btn->width=150;
	btn->height=50;
	
	buttonList.emplace_back(btn);
}
void ExtendsScreen::_buttonClicked(Button&button)
{
	if(&button==btn.get())
		mcGame->getScreenChooser()->schedulePopScreen(1);
}
