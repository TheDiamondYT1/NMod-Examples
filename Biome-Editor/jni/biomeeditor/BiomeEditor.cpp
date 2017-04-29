#include <jni.h>
#include <dlfcn.h>
#include "Substrate.h"

#include "mcpe/client/ClientInstance.h"
#include "mcpe/client/MinecraftGame.h"
#include "mcpe/client/gui/screen/ScreenChooser.h"
#include "mcpe/client/gui/screen/Screen.h"
#include "mcpe/client/gui/GuiData.h"
#include "mcpe/item/Item.h"
#include "mcpe/entity/player/LocalPlayer.h"
#include "mcpe/util/Util.h"
#include "mcpe/item/ItemInstance.h"

#include "ui/EditorScreen.h"

void (*useItemOn_)(Item*,ItemInstance&, Entity&, int, int, int, signed char, float, float, float);
void useItemOn(Item*self,ItemInstance&item, Entity&entity, int x, int y, int z, signed char side, float pixelX, float pixelY, float pixelZ)
{
	useItemOn_(self,item,entity,x,y,z,side,pixelX,pixelY,pixelZ);
	
	Player& player = static_cast<Player&>(entity);
	
	if(item.getId()==271&&player.isLocalPlayer())
	{
		LocalPlayer* local_player = (LocalPlayer*) &player;
		ClientInstance* client_instance = local_player -> getClientInstance();
		MinecraftGame* minecraft_game = client_instance -> getMinecraftGame();
		minecraft_game->getScreenChooser()->_pushScreen(std::make_shared<EditorScreen>(*minecraft_game,*client_instance,entity.getRegion(),BlockPos(x,y,z)),false);
	}
}

JNIEXPORT jint JNI_OnLoad(JavaVM*,void*)
{
	MSHookFunction((void*)&Item::useOn,(void*)&useItemOn,(void**)&useItemOn_);
	return JNI_VERSION_1_6;
}
