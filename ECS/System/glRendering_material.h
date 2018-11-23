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
    	/**
    	 * @brief      Creates an Array of Material Components for the mesh node with name fullindexedname. It must be called for a mesh node. It also registers a map for "caching materials". 
    	 *
    	 * @param      fullindexedname  The fullindexedname
    	 * @param[in]  rMesh            A reference to the mesh
    	 * @param      idmap            Map of names to objects within the visitor.
    	 *
    	 * @return     A shared ptr to a vector of material components. Each component in the vector is a copy not a reference.    
    	 */
        std::shared_ptr<ECS::ShaderMaterialHeaderComponent_> RegisterShaderMaterialHeaderEntriesArray(std::string& fullindexedname, const GTech::Mesh& rMesh, GTech::IdMap& idmap);
        static std::map<std::string, ECS::ShaderMaterialHeaderComponent> materialMap;


    };
    
}

#endif