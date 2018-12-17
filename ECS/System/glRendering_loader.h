#ifndef __GLRENDERING_DATA_H__
#define __GLRENDERING_DATA_H__



#include <map>
#include <memory>
#include <string>

#include <AssetManager/meshvaoarraymap.h>
#include <ECS/Component/componentmanager.h>

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
    


    class RenderingDataManager {

    	RenderingDataManager() = default;


    public:

        GTech::MeshVaoArrayMap meshname_vaoarray_map;

        static RenderingDataManager& GetInstance(){

			static RenderingDataManager rdm;
            return rdm;

        }



    };
}

#endif