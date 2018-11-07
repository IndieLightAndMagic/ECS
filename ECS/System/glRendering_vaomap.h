#ifndef __GLRENDERING_VAOMAP_H__
#define __GLRENDERING_VAOMAP_H__



#include <map>
#include <memory>
#include <string>

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
    
    class VaoMap {
    public:
        std::shared_ptr<unsigned int> CreateVaoEntry(std::string& fullindexedname, std::string& resname, const GTech::Mesh& rMesh);
    protected:
        std::map<std::string, std::shared_ptr<unsigned int>>vaoMap;
    };
}

#endif