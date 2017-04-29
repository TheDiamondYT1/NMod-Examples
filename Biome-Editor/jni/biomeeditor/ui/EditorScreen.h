#pragma once

#include <memory>

#include "mcpe/util/BlockPos.h"
#include "mcpe/client/gui/screen/Screen.h"

class BlockSource;
class Button;
class Screen;
class Label;
class Biome;

namespace Touch{class TButton;}
namespace Touch{class THeader;}

class EditorScreen : public Screen
{
protected:
	std::shared_ptr<Label> labelDefaultBiome;
	std::shared_ptr<Label> labelCurrentBiome;
	std::shared_ptr<Label> labelToBiome;
	std::shared_ptr<Label> labelEditType;
	std::shared_ptr<Touch::TButton> buttonEditType;
	std::shared_ptr<Touch::TButton> buttonToBiome;
	std::shared_ptr<Touch::TButton> buttonDone;
	std::shared_ptr<Touch::TButton> buttonCancel;
	std::shared_ptr<Touch::THeader> header;
protected:
	static Biome* toBiome;
	static bool editFullChunk;
protected:
	BlockSource*source;
	BlockPos pos;
public:
	EditorScreen(MinecraftGame&,ClientInstance&,BlockSource&,BlockPos const&);
	~EditorScreen()=default;
public:
	virtual std::string getScreenName()const;
	virtual std::string getScreenNameW()const;
	virtual void _buttonClicked(Button&);
	virtual void init();
	virtual void render(int,int,float);
protected:
	static std::string biomeToString(Biome&);
	static Biome* nextBiome(Biome&);
protected:
	void edit();
};
