#include <string>
#include <memory>

#include <ECS/System/glRendering_loader.h>

#include <FS/path.h>
#include <FS/resolver.h>


std::map<std::string, std::shared_ptr<unsigned int>>ECS::VaoMap::vaoMap{};



std::shared_ptr<unsigned int> ECS::VaoMap::RegisterVaoEntriesArray(std::string& a_Node_fullindexedname, const GTech::Mesh& rMesh ){
    
    auto [absrespath, relrespath, resname] = GTech::filesystem::resolver::ResourceNameResolution(a_Node_fullindexedname);

    auto trianglearraysz = (int) rMesh.triangleArray.size();

    //Check if it already exists.
    auto meshresourcename_alreadyexists = vaoMap.find(a_Node_fullindexedname) != vaoMap.end() ? true : false;
    if (meshresourcename_alreadyexists) return vaoMap[a_Node_fullindexedname];

    
    //Ok it does not exist. Create the array in memory
    //It was not registered before... tell opengl a new memory array is ready to generate a vertex array
    auto ptr   = std::shared_ptr<unsigned int>(new unsigned int[trianglearraysz], std::default_delete<unsigned int[]>());
    auto apair = std::make_pair(a_Node_fullindexedname, ptr);
    
    
    
    vaoMap.insert(apair);
    return vaoMap[a_Node_fullindexedname];

}