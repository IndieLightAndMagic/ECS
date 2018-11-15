#include <string>
#include <memory>

#include <ECS/System/glRendering_material.h>

#include <FS/path.h>
#include <FS/resolver.h>


std::map<std::string, ECS::ShaderMaterialHeaderComponent> ECS::ShaderMaterialMap::materialMap{};


std::vector<ECS::ShaderMaterialHeaderComponent> ECS::ShaderMaterialMap::RegisterShaderMaterialHeaderEntries(std::string& a_Node_Fullindexedname, const GTech::Mesh& rMesh, GTech::IdMap& idmap){
    
    auto [absrespath, relrespath, resname] = GTech::filesystem::resolver::ResourceNameResolution(a_Node_Fullindexedname);
    auto trianglearraysz                   = (int) rMesh.triangleArray.size();
    std::vector<ECS::ShaderMaterialHeaderComponent> v{};

    //Loop through triangle array
    for (int triangleArrayIndex = 0; triangleArrayIndex < trianglearraysz; ++triangleArrayIndex){
        
        auto a_Material_Full_IndexedName        = absrespath + "/" + rMesh.triangleArray[triangleArrayIndex]->material;
        auto materialresourcename_alreadyexists = materialMap.find(a_Material_Full_IndexedName) != materialMap.end();
        
        if (materialresourcename_alreadyexists) {
            v.push_back(materialMap[a_Material_Full_IndexedName]);
            continue;
        }

        auto materialurl = rMesh.triangleArray[triangleArrayIndex]->material;
        auto materialptr = std::dynamic_pointer_cast<GTech::Material>(idmap[materialurl]);
        
        auto effecturl = materialptr->effectUrl;
        auto effectptr = std::dynamic_pointer_cast<GTech::Effect>(idmap[effecturl]);

        auto shadermaterialheadercomponentptr = std::make_shared<ECS::ShaderMaterialHeaderComponent_>();
        shadermaterialheadercomponentptr->fShininess = effectptr->shininess;
        shadermaterialheadercomponentptr->frefractionindex = effectptr->refractionIndex;
        shadermaterialheadercomponentptr->emissionColor = effectptr->emission;
        shadermaterialheadercomponentptr->diffuseColor = effectptr->diffuse;
        shadermaterialheadercomponentptr->specularColor = effectptr->specular;
        shadermaterialheadercomponentptr->reflectiveColor = effectptr->reflective;

        //Bind the Shaders. 
        auto apair = std::make_pair(a_Material_Full_IndexedName, shadermaterialheadercomponentptr);
        materialMap.insert(apair);
        
        v.push_back(materialMap[a_Material_Full_IndexedName]);

    
    }

    return v;

}