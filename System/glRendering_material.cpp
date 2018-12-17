#include <string>
#include <memory>

#include <ECS/System/glRendering_material.h>

#include <FS/path.h>
#include <FS/resolver.h>




std::shared_ptr<ECS::ShaderMaterialHeaderComponent_> ECS::ShaderMaterialMap::RegisterShaderMaterialHeaderEntriesArray(std::string& a_Node_Fullindexedname, const GTech::Mesh& rMesh, GTech::IdMap& idmap){
    
    auto [absrespath, relrespath, resname] = GTech::filesystem::resolver::ResourceNameResolution(a_Node_Fullindexedname);
    auto trianglearraysz                   = (int) rMesh.triangleArray.size();
    
    auto ptr  = std::shared_ptr<ECS::ShaderMaterialHeaderComponent_>(new ECS::ShaderMaterialHeaderComponent_[trianglearraysz], std::default_delete<ECS::ShaderMaterialHeaderComponent_[]>());
    auto ptr_ = ptr.get();
    
    //Loop through triangle array
    for (int triangleArrayIndex = 0; triangleArrayIndex < trianglearraysz; ++triangleArrayIndex){
        
        auto a_Material_Full_IndexedName        = absrespath + "/" + rMesh.triangleArray[triangleArrayIndex]->material;
        auto materialresourcename_alreadyexists = false;
        
        if (!materialresourcename_alreadyexists) {

            //CREATE MAP ENTRY:

            //Retrieve material from visitor.
            auto materialurl                                   = rMesh.triangleArray[triangleArrayIndex]->material;
            auto materialptr                                   = std::dynamic_pointer_cast<GTech::Material>(idmap[materialurl]);
            
            auto effecturl                                     = materialptr->effectUrl;
            auto effectptr                                     = std::dynamic_pointer_cast<GTech::Effect>(idmap[effecturl]);
            
            //Create a Dummy Component
            auto shadermaterialheadercomponentptr              = std::make_shared<ECS::ShaderMaterialHeaderComponent_>();
            shadermaterialheadercomponentptr->fShininess       = effectptr->shininess;
            shadermaterialheadercomponentptr->frefractionindex = effectptr->refractionIndex;
            shadermaterialheadercomponentptr->emissionColor    = effectptr->emission;
            shadermaterialheadercomponentptr->diffuseColor     = effectptr->diffuse;
            shadermaterialheadercomponentptr->specularColor    = effectptr->specular;
            shadermaterialheadercomponentptr->reflectiveColor  = effectptr->reflective;
            shadermaterialheadercomponentptr->effecturl        = absrespath + "/" + effecturl;
            shadermaterialheadercomponentptr->shadertype       = static_cast<ECS::ShaderMaterialHeaderComponent_::ShaderType>(static_cast<unsigned int>(effectptr->shaderType));

             

            //Bind the Indexed Name with the material. 
            auto apair = std::make_pair(a_Material_Full_IndexedName, shadermaterialheadercomponentptr);

            //Register the map entry.
            //materialMap.insert(apair);

        }
        
        //ptr_[triangleArrayIndex] = *materialMap[a_Material_Full_IndexedName];
    
    }

    return ptr;

}

