#include "EditorScreen.h"

#include "mcpe/client/MinecraftGame.h"
#include "mcpe/client/gui/GuiData.h"
#include "mcpe/client/gui/THeader.h"
#include "mcpe/client/gui/TButton.h"
#include "mcpe/client/gui/Label.h"
#include "mcpe/client/gui/screen/Screen.h"
#include "mcpe/client/renderer/ScreenRenderer.h"
#include "mcpe/client/settings/Options.h"
#include "mcpe/util/Color.h"
#include "mcpe/level/biome/Biome.h"
#include "mcpe/level/BlockSource.h"
#include "mcpe/level/chunk/LevelChunk.h"

Biome* EditorScreen::toBiome=0;
bool EditorScreen::editFullChunk=false;

EditorScreen::EditorScreen(MinecraftGame&game,ClientInstance&instance,BlockSource&s,BlockPos const&ps):Screen(game,instance)
{
	source=&s;
	pos=ps;
	if(!toBiome)toBiome=Biome::beaches;
}
void EditorScreen::init()
{
	Screen::init();
	
	int currentID=0;
	int width=mcGame->getGuiData()->getScreenWidth();
	int height=mcGame->getGuiData()->getScreenHeight();
	
	header=std::make_shared<Touch::THeader>(++currentID,"gui.biomeeditor.name");
	header->xPosition=header->yPosition=0;
	header->width=width;
	header->height=25;
	buttonList.push_back(header);
	
	buttonCancel=std::make_shared<Touch::TButton>(++currentID,"gui.back",mcGame,false,0);
	buttonCancel->xPosition=5;
	buttonCancel->yPosition=height-5-30;
	buttonCancel->width=width/2-10;
	buttonCancel->height=30;
	buttonList.push_back(buttonCancel);
	
	buttonDone=std::make_shared<Touch::TButton>(++currentID,"gui.done",mcGame,false,0);
	buttonDone->xPosition=5+width/2;
	buttonDone->yPosition=height-5-30;
	buttonDone->width=width/2-10;
	buttonDone->height=30;
	buttonList.push_back(buttonDone);
	
	labelDefaultBiome=std::make_shared<Label>(*mcGame,"gui.biome.current_biome",Color::WHITE,0,0,0,0,0);
	labelDefaultBiome->xPosition=5;
	labelDefaultBiome->yPosition=40;
	labelDefaultBiome->width=width;
	labelDefaultBiome->height=10;
	tabElementList.push_back(labelDefaultBiome);
	
	labelCurrentBiome=std::make_shared<Label>(*mcGame,biomeToString(*source->getBiome(pos)),Color::WHITE,0,0,0,0,0);
	labelCurrentBiome->xPosition=15;
	labelCurrentBiome->yPosition=55;
	labelCurrentBiome->width=width;
	labelCurrentBiome->height=10;
	tabElementList.push_back(labelCurrentBiome);
	
	labelToBiome=std::make_shared<Label>(*mcGame,"gui.biomeeditor.transfer_to",Color::WHITE,0,0,0,0,0);
	labelToBiome->xPosition=5;
	labelToBiome->yPosition=70;
	labelToBiome->width=width;
	labelToBiome->height=10;
	tabElementList.push_back(labelToBiome);
	
	buttonToBiome=std::make_shared<Touch::TButton>(++currentID,biomeToString(*toBiome),mcGame,false,0);
	buttonToBiome->xPosition=15;
	buttonToBiome->yPosition=85;
	buttonToBiome->width=width-30;
	buttonToBiome->height=15;
	buttonList.push_back(buttonToBiome);
	
	labelEditType=std::make_shared<Label>(*mcGame,"gui.biomeeditor.transfer_type",Color::WHITE,0,0,0,0,0);
	labelEditType->xPosition=5;
	labelEditType->yPosition=105;
	labelEditType->width=width;
	labelEditType->height=10;
	tabElementList.push_back(labelEditType);
	
	buttonEditType=std::make_shared<Touch::TButton>(++currentID,editFullChunk?"biomeeditor.transfer_type.chunk":"biomeeditor.transfer_type.block",mcGame,false,0);
	buttonEditType->xPosition=15;
	buttonEditType->yPosition=120;
	buttonEditType->width=width-30;
	buttonEditType->height=15;
	buttonList.push_back(buttonEditType);
}
void EditorScreen::render(int i1,int i2,float f)
{
	Screen::render(i1,i2,f);
	
	ScreenRenderer::singletonPtr->drawCenteredString(mcGame->getFont(),"Copyright © 2016 MCAL Team",width/2,height-45,Color(1.0,1.0,1.0,0.5));
}
std::string EditorScreen::getScreenName()const
{
	return "biome_editor_screen";
}
std::string EditorScreen::getScreenNameW()const
{
	return "biome_editor_screen";
}
void EditorScreen::_buttonClicked(Button&id)
{
	Screen::_buttonClicked(id);
	
	if(id.id==buttonCancel->id)
		mcGame->schedulePopScreen(1);
	else if(id.id==buttonDone->id)
	{
		mcGame->schedulePopScreen(1);
		edit();
	}
	else if(id.id==buttonToBiome->id)
	{
		toBiome=nextBiome(*toBiome);
		buttonToBiome->msg=biomeToString(*toBiome);
		buttonToBiome->setMsg(buttonToBiome->msg);
	}
	else if(id.id==buttonEditType->id)
	{
		editFullChunk=!editFullChunk;
		buttonEditType->msg=editFullChunk?"biomeeditor.transfer_type.chunk":"biomeeditor.transfer_type.block";
		buttonEditType->setMsg(buttonEditType->msg);
	}
}
std::string EditorScreen::biomeToString(Biome&b)
{
	if(&b==Biome::beaches)
		return "gui.biomeeditor.Beaches";
	if(&b==Biome::birchForest)
		return "gui.biomeeditor.Birch_Forest";
	if(&b==Biome::birchForestHills)
		return "gui.biomeeditor.Birch_Forest_Hills";
	if(&b==Biome::birchForestHillsMutated)
		return "gui.biomeeditor.Birch_Forest_Hills_Mutated";
	if(&b==Biome::birchForestMutated)
		return "gui.biomeeditor.Birch_Forest_Mutated";
	if(&b==Biome::coldBeach)
		return "gui.biomeeditor.Cold_Beach";
	if(&b==Biome::deepOcean)
		return "gui.biomeeditor.Deep_Ocean";
	if(&b==Biome::desert)
		return "gui.biomeeditor.Desert";
	if(&b==Biome::desertHills)
		return "gui.biomeeditor.Desert_Hills";
	if(&b==Biome::desertMutated)
		return "gui.biomeeditor.Desert_Mutated";
	if(&b==Biome::extremeHills)
		return "gui.biomeeditor.Extreme_Hills";
	if(&b==Biome::extremeHillsMutated)
		return "gui.biomeeditor.Extreme_Hills_Mutated";
	if(&b==Biome::extremeHillsWithTrees)
		return "gui.biomeeditor.Extreme_Hills_With_Trees";
	if(&b==Biome::extremeHillsWithTreesMutated)
		return "gui.biomeeditor.Extreme_Hills_With_Trees_Mutated";
	if(&b==Biome::forest)
		return "gui.biomeeditor.Forest";
	if(&b==Biome::forestHills)
		return "gui.biomeeditor.Forest_Hills";
	if(&b==Biome::forestMutated)
		return "gui.biomeeditor.Forest_Mutated";
	if(&b==Biome::frozenOcean)
		return "gui.biomeeditor.Frozen_Ocean";
	if(&b==Biome::frozenRiver)
		return "gui.biomeeditor.Frozen_River";
	if(&b==Biome::hell)
		return "gui.biomeeditor.Hell";
	if(&b==Biome::iceFlats)
		return "gui.biomeeditor.Ice_Flats";
	if(&b==Biome::iceFlatsMutated)
		return "gui.biomeeditor.Ice_Flats_Mutated";
	if(&b==Biome::iceMountains)
		return "gui.biomeeditor.Ice_Mountains";
	if(&b==Biome::jungle)
		return "gui.biomeeditor.Jungle";
	if(&b==Biome::jungleEdge)
		return "gui.biomeeditor.Jungle_Edge";
	if(&b==Biome::jungleEdgeMutated)
		return "gui.biomeeditor.Jungle_Edge_Mutated";
	if(&b==Biome::jungleHills)
		return "gui.biomeeditor.Jungle_Hills";
	if(&b==Biome::jungleMutated)
		return "gui.biomeeditor.Jungle_Mutated";
	if(&b==Biome::mesa)
		return "gui.biomeeditor.Mesa";
	if(&b==Biome::mesaClearRock)
		return "gui.biomeeditor.Mesa_Clear_Rock";
	if(&b==Biome::mesaClearRockMutated)
		return "gui.biomeeditor.Mesa_Clear_Rock_Mutated";
	if(&b==Biome::mesaMutated)
		return "gui.biomeeditor.Mesa_Mutated";
	if(&b==Biome::mesaRock)
		return "gui.biomeeditor.Mesa_Rock";
	if(&b==Biome::mesaRockMutated)
		return "gui.biomeeditor.Mesa_Rock_Mutated";
	if(&b==Biome::mushroomIsland)
		return "gui.biomeeditor.Mushroom_Island";
	if(&b==Biome::mushroomIslandShore)
		return "gui.biomeeditor.Mushroom_Island_Shore";
	if(&b==Biome::ocean)
		return "gui.biomeeditor.Ocean";
	if(&b==Biome::plains)
		return "gui.biomeeditor.Plains";
	if(&b==Biome::plainsMutated)
		return "gui.biomeeditor.Plains Mutated";
	if(&b==Biome::redwoodTaiga)
		return "gui.biomeeditor.Redwood Taiga";
	if(&b==Biome::redwoodTaigaHills)
		return "gui.biomeeditor.Redwood_Taiga_Hills";
	if(&b==Biome::redwoodTaigaHillsMutated)
		return "gui.biomeeditor.Redwood_Taiga_Hills_Mutated";
	if(&b==Biome::redwoodTaigaMutated)
		return "gui.biomeeditor.Redwood_Taiga_Mutated";
	if(&b==Biome::river)
		return "gui.biomeeditor.River";
	if(&b==Biome::roofedForest)
		return "gui.biomeeditor.Roofed_Forest";
	if(&b==Biome::roofedForestMutated)
		return "gui.biomeeditor.Roofed_Forest_Mutated";
	if(&b==Biome::savanna)
		return "gui.biomeeditor.Savanna";
	if(&b==Biome::savannaMutated)
		return "gui.biomeeditor.Savanna_Mutated";
	if(&b==Biome::savannaRock)
		return "gui.biomeeditor.Savanna_Rock";
	if(&b==Biome::savannaRockMutated)
		return "gui.biomeeditor.Savanna_Rock_Mutated";
	if(&b==Biome::sky)
		return "gui.biomeeditor.Sky";
	if(&b==Biome::smallerExtremeHills)
		return "gui.biomeeditor.Smaller_Extreme_Hills";
	if(&b==Biome::stoneBeach)
		return "gui.biomeeditor.Stone_Beach";
	if(&b==Biome::swampland)
		return "gui.biomeeditor.Swampland";
	if(&b==Biome::swamplandMutated)
		return "gui.biomeeditor.Swampland_Mutated";
	if(&b==Biome::taiga)
		return "gui.biomeeditor.Taiga";
	if(&b==Biome::taigaCold)
		return "gui.biomeeditor.Taiga_Cold";
	if(&b==Biome::taigaColdHills)
		return "gui.biomeeditor.Taiga_Cold_Hills";
	if(&b==Biome::taigaColdMutated)
		return "gui.biomeeditor.Taiga_Cold_Mutated";
	if(&b==Biome::taigaHills)
		return "gui.biomeeditor.Taiga_Hills";
	if(&b==Biome::taigaMutated)
		return "gui.biomeeditor.Taiga_Mutated";
	
	/*
	if(&b==Biome::beaches)return "沙滩";
	if(&b==Biome::birchForest)return "白桦森林";
	if(&b==Biome::birchForestHills)return "白桦森林山丘";
	if(&b==Biome::birchForestHillsMutated)return "白桦森林山丘M";
	if(&b==Biome::birchForestMutated)return "白桦森林M";
	if(&b==Biome::coldBeach)return "寒冷沙滩";
	if(&b==Biome::deepOcean)return "深海";
	if(&b==Biome::desert)return "沙漠";
	if(&b==Biome::desertHills)return "沙漠山丘";
	if(&b==Biome::desertMutated)return "沙漠M";
	if(&b==Biome::extremeHills)return "峭壁";
	if(&b==Biome::extremeHillsMutated)return "峭壁M";
	if(&b==Biome::extremeHillsWithTrees)return "峭壁+";
	if(&b==Biome::extremeHillsWithTreesMutated)return "峭壁+M";
	if(&b==Biome::forest)return "森林";
	if(&b==Biome::forestHills)return "森林山丘";
	if(&b==Biome::forestMutated)return "森林M";
	if(&b==Biome::frozenOcean)return "冻洋";
	if(&b==Biome::frozenRiver)return "冻河";
	if(&b==Biome::hell)return "地狱";
	if(&b==Biome::iceFlats)return "雪原";
	if(&b==Biome::iceFlatsMutated)return "雪原M";
	if(&b==Biome::iceMountains)return "冰刺之地";
	if(&b==Biome::jungle)return "雨林";
	if(&b==Biome::jungleEdge)return "丛林边缘";
	if(&b==Biome::jungleEdgeMutated)return "丛林边缘M";
	if(&b==Biome::jungleHills)return "丛林山丘";
	if(&b==Biome::jungleMutated)return "丛林M";
	if(&b==Biome::mesa)return "平顶山";
	if(&b==Biome::mesaClearRock)return "平顶山高原";
	if(&b==Biome::mesaClearRockMutated)return "平顶山高原M";
	if(&b==Biome::mesaMutated)return "平顶山M";
	if(&b==Biome::mesaRock)return "平顶山岩石";
	if(&b==Biome::mesaRockMutated)return "平顶山岩石M";
	if(&b==Biome::mushroomIsland)return "蘑菇岛";
	if(&b==Biome::mushroomIslandShore)return "蘑菇岛边缘";
	if(&b==Biome::ocean)return "海";
	if(&b==Biome::plains)return "平原";
	if(&b==Biome::plainsMutated)return "平原M";
	if(&b==Biome::redwoodTaiga)return "红木森林";
	if(&b==Biome::redwoodTaigaHills)return "红木森林山丘";
	if(&b==Biome::redwoodTaigaHillsMutated)return "红木森林山丘M";
	if(&b==Biome::redwoodTaigaMutated)return "红木森林M";
	if(&b==Biome::river)return "河流";
	if(&b==Biome::roofedForest)return "黑森林";
	if(&b==Biome::roofedForestMutated)return "黑森林M";
	if(&b==Biome::savanna)return "热带草原";
	if(&b==Biome::savannaMutated)return "热带草原M";
	if(&b==Biome::savannaRock)return "热带高草原";
	if(&b==Biome::savannaRockMutated)return "热带高草原M";
	if(&b==Biome::sky)return "天空";
	if(&b==Biome::smallerExtremeHills)return "小峭壁";
	if(&b==Biome::stoneBeach)return "石山";
	if(&b==Biome::swampland)return "沼泽";
	if(&b==Biome::swamplandMutated)return "沼泽M";
	if(&b==Biome::taiga)return "针叶林";
	if(&b==Biome::taigaCold)return "冷针叶林";
	if(&b==Biome::taigaColdHills)return "冷针叶林山丘";
	if(&b==Biome::taigaColdMutated)return "冷针叶林M";
	if(&b==Biome::taigaHills)return "针叶林山丘";
	if(&b==Biome::taigaMutated)return "针叶林M";
	*/
	return "gui.biomeeditor.unknow";
}
Biome* EditorScreen::nextBiome(Biome&b)
{
	if(&b==Biome::beaches)return Biome::birchForest;
	if(&b==Biome::birchForest)return Biome::birchForestHills;
	if(&b==Biome::birchForestHills)return Biome::birchForestHillsMutated;
	if(&b==Biome::birchForestHillsMutated)return Biome::birchForestMutated;
	if(&b==Biome::birchForestMutated)return Biome::coldBeach;
	if(&b==Biome::coldBeach)return Biome::deepOcean;
	if(&b==Biome::deepOcean)return Biome::desert;
	if(&b==Biome::desert)return Biome::desertHills;
	if(&b==Biome::desertHills)return Biome::desertMutated;
	if(&b==Biome::desertMutated)return Biome::extremeHills;
	if(&b==Biome::extremeHills)return Biome::extremeHillsMutated;
	if(&b==Biome::extremeHillsMutated)return Biome::extremeHillsWithTrees;
	if(&b==Biome::extremeHillsWithTrees)return Biome::extremeHillsWithTreesMutated;
	if(&b==Biome::extremeHillsWithTreesMutated)return Biome::forest;
	if(&b==Biome::forest)return Biome::forestHills;
	if(&b==Biome::forestHills)return Biome::forestMutated;
	if(&b==Biome::forestMutated)return Biome::frozenOcean;
	if(&b==Biome::frozenOcean)return Biome::frozenRiver;
	if(&b==Biome::frozenRiver)return Biome::hell;
	if(&b==Biome::hell)return Biome::iceFlats;
	if(&b==Biome::iceFlats)return Biome::iceFlatsMutated;
	if(&b==Biome::iceFlatsMutated)return Biome::iceMountains;
	if(&b==Biome::iceMountains)return Biome::jungle;
	if(&b==Biome::jungle)return Biome::jungleEdge;
	if(&b==Biome::jungleEdge)return Biome::jungleEdgeMutated;
	if(&b==Biome::jungleEdgeMutated)return Biome::jungleHills;
	if(&b==Biome::jungleHills)return Biome::jungleMutated;
	if(&b==Biome::jungleMutated)return Biome::mesa;
	if(&b==Biome::mesa)return Biome::mesaClearRock;
	if(&b==Biome::mesaClearRock)return Biome::mesaClearRockMutated;
	if(&b==Biome::mesaClearRockMutated)return Biome::mesaMutated;
	if(&b==Biome::mesaMutated)return Biome::mesaRock;
	if(&b==Biome::mesaRock)return Biome::mesaRockMutated;
	if(&b==Biome::mesaRockMutated)return Biome::mushroomIsland;
	if(&b==Biome::mushroomIsland)return Biome::mushroomIslandShore;
	if(&b==Biome::mushroomIslandShore)return Biome::ocean;
	if(&b==Biome::ocean)return Biome::plains;
	if(&b==Biome::plains)return Biome::plainsMutated;
	if(&b==Biome::plainsMutated)return Biome::redwoodTaiga;
	if(&b==Biome::redwoodTaiga)return Biome::redwoodTaigaHills;
	if(&b==Biome::redwoodTaigaHills)return Biome::redwoodTaigaHillsMutated;
	if(&b==Biome::redwoodTaigaHillsMutated)return Biome::redwoodTaigaMutated;
	if(&b==Biome::redwoodTaigaMutated)return Biome::river;
	if(&b==Biome::river)return Biome::roofedForest;
	if(&b==Biome::roofedForest)return Biome::roofedForestMutated;
	if(&b==Biome::roofedForestMutated)return Biome::savanna;
	if(&b==Biome::savanna)return Biome::savannaMutated;
	if(&b==Biome::savannaMutated)return Biome::savannaRock;
	if(&b==Biome::savannaRock)return Biome::savannaRockMutated;
	if(&b==Biome::savannaRockMutated)return Biome::sky;
	if(&b==Biome::sky)return Biome::smallerExtremeHills;
	if(&b==Biome::smallerExtremeHills)return Biome::stoneBeach;
	if(&b==Biome::stoneBeach)return Biome::swampland;
	if(&b==Biome::swampland)return Biome::swamplandMutated;
	if(&b==Biome::swamplandMutated)return Biome::taiga;
	if(&b==Biome::taiga)return Biome::taigaCold;
	if(&b==Biome::taigaCold)return Biome::taigaColdHills;
	if(&b==Biome::taigaColdHills)return Biome::taigaColdMutated;
	if(&b==Biome::taigaColdMutated)return Biome::taigaHills;
	if(&b==Biome::taigaHills)return Biome::taigaMutated;
	if(&b==Biome::taigaMutated)return Biome::beaches;
	return &b;
}
void EditorScreen::edit()
{
	if(editFullChunk)
		for(int x=pos.x-pos.x%16;x<=pos.x-pos.x%16+16;++x)
			for(int z=pos.z-pos.z%16;z<=pos.z-pos.z%16+16;++z)
				source->getChunkAt(pos)->setBiome(*toBiome,ChunkBlockPos(BlockPos(x,0,z)));
	else
		source->getChunkAt(pos)->setBiome(*toBiome,ChunkBlockPos(pos));
}
