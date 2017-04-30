#pragma once

#include "mcpe/client/gui/screen/Screen.h"

class ExtendsScreen : public Screen
{
public:
	ExtendsScreen(MinecraftGame&,ClientInstance&);
protected:
	std::shared_ptr<Touch::TButton>btn;
public:
    virtual std::string getScreenName() const;
    virtual std::string getScreenNameW() const;
    virtual void init();
    virtual void _buttonClicked(Button&);
};
