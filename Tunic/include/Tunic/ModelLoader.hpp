#pragma once

#include "Tunic/Rendering/Renderables/Mesh.hpp"

namespace tnc::ModelLoader {
	//Temp: Need a Model class

	struct Model{
		std::vector<std::shared_ptr<rnd::Mesh>> meshs;
	};

	Model loadModel(std::string_view filePath);
}