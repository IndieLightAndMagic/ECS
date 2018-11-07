
#include <ECS/System/glRendering_vaomap.h>

std::shared_ptr<unsigned int> ECS::VaoMap::CreateVaoEntry (std::string& nodefullindexedname, std::string& resname, const GTech::Node& rNode, const GTech::Scene& rScene ){
    
    /*
    auto trianglearraysz     = 19;


    //Check if it already exists.
    auto meshresourcename_alreadyexists = vaoMap.find(nodefullindexedname) != vaoMap.end() ? true : false;
    if (meshresourcename_alreadyexists) return vaoMap[nodefullindexedname];

    
    //Ok it does not exist. Create the array in memory
    auto ptr = std::shared_ptr<unsigned int>(new unsigned int[sz], std::default_delete<unsigned int[]>());
    auto apair = std::make_pair(meshresourcename, ptr);

    //It was not registered before... tell opengl a new memory array is ready to generate a vertex array
    glGenVertexArrays(sz, ptr.get());
    vaoMap.insert(apair);

    //Ok Create VBOs
    */

    return vaoMap[nodefullindexedname];

}