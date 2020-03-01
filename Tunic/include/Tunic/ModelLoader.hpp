#pragma once

#include "Tunic/Rendering/Renderables/Model.hpp"

namespace tnc::ModelLoader {
	rnd::Model loadModel(std::string_view filePath);
}