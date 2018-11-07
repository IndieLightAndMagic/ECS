#include <string>
#include <memory>
#include <ECS/System/glRendering_vaomap.h>

std::shared_ptr<unsigned int> ECS::VaoMap::CreateVaoEntry (std::string& nodefullindexedname, std::string& resname, const GTech::Mesh& rMesh ){
    
    auto trianglearraysz     = (int) rMesh.triangleArray.size();


    //Check if it already exists.
    auto meshresourcename_alreadyexists = vaoMap.find(nodefullindexedname) != vaoMap.end() ? true : false;
    if (meshresourcename_alreadyexists) return vaoMap[nodefullindexedname];

    
    //Ok it does not exist. Create the array in memory
    //It was not registered before... tell opengl a new memory array is ready to generate a vertex array
    auto ptr = std::shared_ptr<unsigned int>(new unsigned int[trianglearraysz], std::default_delete<unsigned int[]>());
    auto apair = std::make_pair(nodefullindexedname, ptr);
    vaoMap.insert(apair);

    //Generate the vertex array
    glGenVertexArrays(trianglearraysz, ptr.get());
    
    //Ok Create VBOs
    

    return vaoMap[nodefullindexedname];

}