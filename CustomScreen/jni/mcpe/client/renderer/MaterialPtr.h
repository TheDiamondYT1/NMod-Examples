#pragma once

#include <string>

class RenderMaterialGroup;

namespace mce{
class MaterialPtr
{
public:
	char filler1[12];	// 0

	MaterialPtr(MaterialPtr &&);
	MaterialPtr(RenderMaterialGroup &, const std::string &);
	MaterialPtr();
};
}
