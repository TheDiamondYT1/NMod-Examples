#include <jni.h>
#include <dlfcn.h>
#include "Substrate.h"

#include "mcpe/client/ClientInstance.h"
#include "mcpe/client/MinecraftGame.h"
#include "mcpe/client/gui/screen/ScreenChooser.h"
#include "mcpe/client/gui/screen/Screen.h"
#include "mcpe/client/gui/GuiData.h"
#include "mcpe/gamemode/GameMode.h"
#include "mcpe/entity/player/LocalPlayer.h"
#include "mcpe/util/Util.h"
#include "mcpe/item/ItemInstance.h"

#include "ui/EditorScreen.h"

void (*useItemOn_)(GameMode*,Player&, ItemInstance*, const BlockPos&, signed char, const Vec3&);
void useItemOn(GameMode*self,Player&player, ItemInstance*item, const BlockPos&pos, signed char side, const Vec3&pixel)
{
	useItemOn_(self,player,item,pos,side,pixel);
	if(item&&item->getId()==271&&player.isLocalPlayer())
	{
		LocalPlayer* local_player = (LocalPlayer*) &player;
		ClientInstance* client_instance = local_player -> getClientInstance();
		MinecraftGame* minecraft_game = client_instance -> getMinecraftGame();
		minecraft_game->getScreenChooser()->_pushScreen(std::make_shared<EditorScreen>(*minecraft_game,*client_instance,player.getRegion(),pos),false);
	}
}

JNIEXPORT jint JNI_OnLoad(JavaVM*,void*)
{
	MSHookFunction((void*)&GameMode::useItemOn,(void*)&useItemOn,(void**)&useItemOn_);
	return JNI_VERSION_1_6;
}
