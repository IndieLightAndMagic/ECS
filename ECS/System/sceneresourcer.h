#ifndef __SCENERESOURCER_H__
#define __SCENERESOURCER_H__

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
    
    
    class ResourceManager  {

        /**
         * Map of names and shared ptrs to XMLDoc scenes.
         */
        std::map<std::string, PairDocVisitorPtr> map_SResourceName_PairDocVisitor;
        PairDocVisitorPtr GetPairDocVisitorPtr(const std::string& resourceFilenamePathStr);

        /**
         * Map of resources to vao arrays
         */
        ECS::VaoMap vaoMap;
        ECS::ShaderMaterialMap shaderMaterialMap;
        /**
         * @brief      Check if a resource file is alredy registered.
         *
         * @param[in]  resourceFilenamePathStr  path to file to be checked if already registered.
         *
         * @return     If file is already registered on system true will be returnerd.
         */
        bool ResourceFileHasPairDocVisitorEntryRegistered(const std::string& resourceFilenamePathStr);

        /**
         * @brief      Register the assets of a filename. It will register the file and preload the assets. 
         *
         * @param[in]  resourceFilenamePathStr  Path str to file with asset resources.        
         *
         * @return     On success will return true otherwise.... 
         */
        bool RegisterPairDocVisitorEntry(const std::string& resourceFilenamePathStr);

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