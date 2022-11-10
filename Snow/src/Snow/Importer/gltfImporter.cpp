#include <spch.h>

#include "Snow/Importer/gltfImporter.h"

#include "Snow/Utils/StringUtils.h"

namespace Snow {
	GLTFFile GLTFImporter::Import(std::filesystem::path& filepath) {
		GLTFFile File;

        std::string result;
        std::ifstream inFile(filepath, std::ios::in | std::ios::binary);
        if (inFile.is_open()) {
            inFile.seekg(0, std::ios::end);
            result.resize(inFile.tellg());
            inFile.seekg(0, std::ios::beg);
            inFile.read(&result[0], result.size());
        }
        else {
            SNOW_CORE_ERROR("Could not read file. (Path):{0}", filepath.string());
        }
        inFile.close();


        
        return File;
	}
}