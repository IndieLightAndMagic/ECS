#include <string>
#include <memory>
#include <ECS/System/glRendering_vaomap.h>

std::shared_ptr<unsigned int> ECS::VaoMap::CreateVaoEntry (std::string& nodefullindexedname, std::string& resname, const GTech::Mesh& rMesh ){
    
    auto trianglearraysz = (int) rMesh.triangleArray.size();
    auto meshdataptr     = reinterpret_cast<const GLvoid*>(rMesh.floatVector.data()) ;

    //Check if it already exists.
    auto meshresourcename_alreadyexists = vaoMap.find(nodefullindexedname) != vaoMap.end() ? true : false;
    if (meshresourcename_alreadyexists) return vaoMap[nodefullindexedname];

    
    //Ok it does not exist. Create the array in memory
    //It was not registered before... tell opengl a new memory array is ready to generate a vertex array
    auto ptr   = std::shared_ptr<unsigned int>(new unsigned int[trianglearraysz], std::default_delete<unsigned int[]>());
    auto apair = std::make_pair(nodefullindexedname, ptr);
    vaoMap.insert(apair);

    //Generate the vertex array
    auto rawvaoptr = ptr.get();
    glGenVertexArrays(trianglearraysz, rawvaoptr);
    

   

    auto meshsourcemap = rMesh.meshSourceMap;

    //Loop through triangle array
    for (int triangleArrayIndex = 0; triangleArrayIndex < trianglearraysz; ++triangleArrayIndex){
        
        auto triangleheaderptr = rMesh.triangleArray[triangleArrayIndex];

        glBindVertexArray(rawvaoptr[triangleArrayIndex]);
        
        //Ok Create VBOs and EBOs
        unsigned int vbo, ebo;
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        //Source VBO
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * rMesh.floatVector.size(), meshdataptr, GL_STATIC_DRAW);
        
        //Source EBo
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * triangleheaderptr->indexArray.size(), triangleheaderptr->indexArray.data(), GL_STATIC_DRAW);

        auto& triangledataptr  = rMesh.triangleArray[triangleArrayIndex];
        
        for (auto& trianglesemantic : triangledataptr->meshTrianglesInput){

            const auto& semantictype    = (unsigned int)trianglesemantic.semanticType;
            const auto& meshsourceptr   = meshsourcemap[trianglesemantic.source];
            auto* const offsetaddress   = &(reinterpret_cast<const float*>(meshdataptr))[meshsourceptr->index];
            const auto triangledatasize = (int)(meshsourceptr->pointsCount * 3 *sizeof(float));

            glVertexAttribPointer(semantictype, triangledatasize, GL_FLOAT, GL_FALSE, 0, offsetaddress);

        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

    }
    

    return vaoMap[nodefullindexedname];

}