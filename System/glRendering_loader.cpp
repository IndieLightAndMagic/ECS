#include <string>
#include <memory>

#include <ECS/System/glRendering_loader.h>

#include <FS/path.h>
#include <FS/resolver.h>


std::map<std::string, std::shared_ptr<unsigned int>>ECS::VaoMap::vaoMap{};


void UnbindVertexArray(){

    glBindVertexArray(0);

}

void BindVertexData(const GTech::Mesh& rMesh){
        
    //Get raw pointer   
    auto meshdataptr = reinterpret_cast<const GLvoid*>(rMesh.floatVector.data()) ;

    //Ok Create VBOs and EBOs
    unsigned int vbo, ebo;
    glGenBuffers(1, &vbo);

    //Source VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * rMesh.floatVector.size(), meshdataptr, GL_STATIC_DRAW);

}

void UnbindVertexData(){
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BindVertexIndexData(const GTech::Mesh& rMesh, unsigned int triangleArrayIndex){

    //Source EBo
    unsigned int ebo;
    
    auto triangleheaderptr = rMesh.triangleArray[triangleArrayIndex];

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * triangleheaderptr->indexArray.size(), triangleheaderptr->indexArray.data(), GL_STATIC_DRAW);


}

void UnbindVertexIndexData(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SetVertexAttribPointers(const GTech::Mesh& rMesh, int triangleArrayIndex){

    auto meshsourcemap    = rMesh.meshSourceMap;
    auto meshdataptr      = reinterpret_cast<const GLvoid*>(rMesh.floatVector.data());
    auto& triangledataptr = rMesh.triangleArray[triangleArrayIndex];
    
    for (auto& trianglesemantic : triangledataptr->meshTrianglesInput){
        
        const auto& semantictype    = (unsigned int)trianglesemantic.semanticType;
        const auto& meshsourceptr   = meshsourcemap[trianglesemantic.source];
        auto* const offsetaddress   = &(reinterpret_cast<const float*>(meshdataptr))[meshsourceptr->index];
        const auto triangledatasize = (int)(meshsourceptr->pointsCount * 3 *sizeof(float));
        glVertexAttribPointer(semantictype, triangledatasize, GL_FLOAT, GL_FALSE, 0, offsetaddress);
    
    }

}
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
    
    //Generate the vertexes array
    auto rawvaoptr = ptr.get();
    glGenVertexArrays(trianglearraysz, rawvaoptr);

    //Loop through triangle array
    for (int triangleArrayIndex = 0; triangleArrayIndex < trianglearraysz; ++triangleArrayIndex){
        
        glBindVertexArray(rawvaoptr[triangleArrayIndex]);
        
        BindVertexData(rMesh);
        SetVertexAttribPointers(rMesh, triangleArrayIndex);
        UnbindVertexData();

        BindVertexIndexData(rMesh, triangleArrayIndex);
        UnbindVertexIndexData();

        UnbindVertexArray();
    }
    
    vaoMap.insert(apair);
    return vaoMap[a_Node_fullindexedname];

}