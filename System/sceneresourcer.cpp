
#include <memory>

#include <ECS/System/sceneresourcer.h>
#include <ECS/Entity/entitymanager.h>
#include <ECS/Component/componentmanager.h>

#include <FS/path.h>
#include <FS/resolver.h>


using namespace tinyxml2;
using namespace GTech;



GTech::PairDocVisitorPtr GTech::ResourceManager::GetPairDocVisitorPtr(const std::string &resourceFilenamePathStr){

    if (GTech::ResourceManager::ResourceFileHasPairDocVisitorEntryRegistered(resourceFilenamePathStr)){
        return map_SResourceName_PairDocVisitor[resourceFilenamePathStr];
    }

    return nullptr;
}


bool GTech::ResourceManager::RegisterPairDocVisitorEntry(const std::string& resourceFilenamePathStr){
    
    if (GTech::ResourceManager::ResourceFileHasPairDocVisitorEntryRegistered(resourceFilenamePathStr)) return true;

    //Resource is not registered yet. Register it. 
    auto pair_xmldoc_visitor_ptr                              = GTech::make_PairDocVisitorPtr();
    auto& xmldoc                                              = pair_xmldoc_visitor_ptr->first;
    auto loadfileresult                                       = xmldoc.LoadFile(resourceFilenamePathStr.c_str());
    auto& visitor                                             = pair_xmldoc_visitor_ptr->second;
    auto visitor_ptr                                          = &visitor;

    if (loadfileresult != tinyxml2::XML_SUCCESS) return false;
    
    map_SResourceName_PairDocVisitor[resourceFilenamePathStr] = pair_xmldoc_visitor_ptr; 
    return xmldoc.Accept(visitor_ptr);

}

bool GTech::ResourceManager::ResourceFileHasPairDocVisitorEntryRegistered(const std::string& resourceFilenamePathStr){

    auto trytofindthisresource       = map_SResourceName_PairDocVisitor.find(resourceFilenamePathStr);
    auto resourcewasnotfound         = map_SResourceName_PairDocVisitor.end();
    auto resourceisalreadyregistered = trytofindthisresource != resourcewasnotfound;
    if (resourceisalreadyregistered) return true;

    return false;
}



unsigned int GTech::ResourceManager::Load(const std::string& resourceFileName){
    
    auto [absrespath, relrespath, resname] = GTech::filesystem::resolver::ResourceNameResolution(resourceFileName);
    auto nodefullindexedname               = absrespath + resname;
                                        
    if (!GTech::ResourceManager::ResourceFileHasPairDocVisitorEntryRegistered(absrespath))
        if (GTech::ResourceManager::RegisterPairDocVisitorEntry(absrespath) == false) return 0;
    
    auto pPairDocVisitor = GTech::ResourceManager::GetPairDocVisitorPtr(absrespath);
    auto visitor         = pPairDocVisitor->second;
    auto scene           = visitor.GetScene();
    auto nodefound       = scene.nodes.find(resname) != scene.nodes.end();
                                                                                                                                                                                                                                                                                                                                                         
    if (!nodefound) return 0;

    // Ok node was found... start creating components.
    auto pnode = scene.nodes[resname];
    
    // Create Mtx Component and assign the node's transform matrix (a copy).
    // Invoke component manager.
    auto& componentmngr = ECS::ComponentManager::GetInstance();
    auto& entitymngr   = ECS::EntityManager::GetInstance();

    //Create entity and get its info component
    auto eid = entitymngr.CreateEntity(); //entityid
    auto infocomponentid = entitymngr.GetComponentsIds(eid)[0];
    auto infocomponentptr = componentmngr.GetComponentRaw<ECS::EntityInformationComponent_>(infocomponentid);

    // Create Mtx Component using the component manager.
    auto mtxcomponentid  = componentmngr.CreateComponent<ECS::MatrixComponent_>();
    auto mtxcomponentptr = componentmngr.GetComponentRaw<ECS::MatrixComponent_>(mtxcomponentid);
    mtxcomponentptr->matrix = pnode->transform;

    entitymngr.AddComponent(eid, mtxcomponentid);    //Ok now we have a transform matrix. 
    
    // Ok 
    if (pnode->nodeType == GTech::Node::NodeType::MESH){

        //VAO array component
        auto vaoarraycomponentid  = componentmngr.CreateComponent<ECS::VaoArrayComponent_>();
        auto vaoarraycomponentptr = componentmngr.GetComponentRaw<ECS::VaoArrayComponent_>(vaoarraycomponentid);

        //Allocate Vertex Array Objects VAO and with  VBOs and EBOs. Create a Vector with pointers and assign them to the component.
        auto pmesh                                            = std::dynamic_pointer_cast<GTech::Mesh>(scene.urlPtrMap[pnode->url]);
        auto vaoptr                                           = vaoMap.CreateVaoEntry(nodefullindexedname, *pmesh);
        auto shaderMaterialPtrVector                          = materialMap.RegisterShaderMaterialHeaderEntries(nodefullindexedname, *pmesh, scene.urlPtrMap);
        vaoarraycomponentptr->wkptr_vaoarray                  = vaoptr;
        vaoarraycomponentptr->materialheadercomponentptr_vtor = shaderMaterialPtrVector;

    } else if (pnode->nodeType == GTech::Node::NodeType::CAMERA){

        //We need to create the projection matrix component.
        auto prjcomponentid  = componentmngr.CreateComponent<ECS::MatrixComponent_>();
        auto prjcomponentptr = componentmngr.GetComponentRaw<ECS::MatrixComponent_>(prjcomponentid);


        //Lets assign a proper projection matrix.
        auto pcam               = std::dynamic_pointer_cast<GTech::Camera>(scene.urlPtrMap[pnode->url]);
        auto fovy               = pcam->projection.yfov;
        auto aspect             = pcam->aspect_ratio;
        auto znear              = pcam->znear;
        auto zfar               = pcam->zfar;
        prjcomponentptr->matrix = glm::perspective(glm::radians(fovy), aspect, znear, zfar);

        entitymngr.AddComponent(eid, prjcomponentid); //Projection matrix component for camera.
        infocomponentptr->SetGlCamTuple(mtxcomponentid, prjcomponentid); //Set the appropriate components for your camera entity.
        return eid;

    } else if (pnode->nodeType == GTech::Node::NodeType::LIGHT) {

        auto lightcomponentid = componentmngr.CreateComponent<ECS::ShaderLightHeaderComponent_>();
        auto lightcomponentptr = componentmngr.GetComponentRaw<ECS::ShaderLightHeaderComponent_>(lightcomponentid);

        auto plight = std::dynamic_pointer_cast<GTech::Light>(scene.urlPtrMap[pnode->url]); 

        lightcomponentptr->directional = plight->lightType == GTech::Light::LightType::SUN;
        if (plight->quadratic_attenuation > 0.0f) {

            lightcomponentptr->iAttenuationExponent = 2;
            lightcomponentptr->fAttenuationBase = plight->quadratic_attenuation;

        } else if (plight->linear_attenuaton > 0.0f) {

            lightcomponentptr->iAttenuationExponent = 1;
            lightcomponentptr->fAttenuationBase = plight->linear_attenuaton;

        } else {

            lightcomponentptr->iAttenuationExponent = 0;
            lightcomponentptr->fAttenuationBase = plight->constant_attenuation;
        }
        lightcomponentptr->fAttenuationFactor = glm::pow(lightcomponentptr->fAttenuationBase, lightcomponentptr->iAttenuationExponent);

        entitymngr.AddComponent(eid, lightcomponentid); //Add the shader light header to your lamp entity
        infocomponentptr->SetGlLightTuple(mtxcomponentid, lightcomponentid);
        return eid;
    }

    return 0;
}

void GTech::ResourceManager::UnLoad(const std::string &resourceName){
    
}

void GTech::ResourceManager::ClearCache(){

    map_SResourceName_PairDocVisitor.clear();

}

