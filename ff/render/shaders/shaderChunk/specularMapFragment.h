#pragma once
#include "../../../global/base.h"

namespace ff {
	static const std::string specularMapFragment =
		"float specularStrength = 1.0;\n"\
		"#ifdef USE_SPECULARMAP\n"\
		"	specularStrength = texture(specularMap, fragUV).r;\n"\
		"#endif\n"\
		"\n";
}