#ifndef __GLRENDERING_MATERIAL_H__
#define __GLRENDERING_MATERIAL_H__



#include <map>
#include <memory>
#include <string>
#include <vector>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#else 
#include <OpenGL/gl.h>
#endif /*__APPLE__*/
#include <collader/collader.h>
#include <collader/idname.h>
#include <collader/node.h>
#include <collader/scene.h>

namespace ECS {
    
    class ShaderMaterialMap {

    public:
        std::vector<ECS::ShaderMaterialHeaderComponent> RegisterShaderMaterialHeaderEntries(std::string& fullindexedname, const GTech::Mesh& rMesh, GTech::IdMap& idmap);
        static std::map<std::string, ECS::ShaderMaterialHeaderComponent> materialMap;


    };
    
}

#endif