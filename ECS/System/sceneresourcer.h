#ifndef __SCENERESOURCER_H__
#define __SCENERESOURCER_H__

#include <AssetManager/meshvaoarraymap.h>
#include <ECS/System/glRendering_loader.h>
#include <ECS/System/glRendering_material.h>

#include <collader/collader.h>

#include <tuple>
#include <memory>

/* Some definitions:
    - Resource Name is the Name of a resource.
    - Resource File is a File (usually a collada file) holding one or more resources.

    - Example of resourcename: Cube.
    - Example of resourcefile: ../Scene.dae.

*/
namespace GTech {

    using PairDocVisitor = std::pair<tinyxml2::XMLDocument, GTech::ColladaVisitor>;
    using PairDocVisitorPtr = std::shared_ptr<PairDocVisitor>;
    
    
    class ResourceManager : public  {


        GTech::SceneResourceManagerMap sceneresourcemanagermap;

        unsigned int LoadMesh(const GTech::Scene&, const GTech::Node&);
    
    public:
        unsigned int Load(const std::string& resourceName);
        void UnLoad(const std::string& resourceName);
        void ClearCache();

        static ResourceManager& GetInstance(){
            static ResourceManager instance;
            return instance;
        }
    };

}

#endif //__SCENERESOURCER_H__