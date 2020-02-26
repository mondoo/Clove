#pragma once

#include "Tunic/Rendering/Renderables/Mesh.hpp"

namespace tnc::ModelLoader {
	//Temp: Need a Model class

	struct Model{
		std::vector<rnd::Mesh> meshs;
	};

	Model loadModel(std::string_view filePath);
}