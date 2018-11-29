
#include <memory>

#include <ECS/System/sceneresourcer.h>
#include <ECS/Entity/entitymanager.h>
#include <ECS/Component/componentmanager.h>

#include <FS/path.h>
#include <FS/resolver.h>


using namespace tinyxml2;
using namespace GTech;

unsigned int GTech::ResourceManager::Load(const std::string& resourceNameAndFileName){
    
    auto [absrespath, relrespath, resname] = GTech::filesystem::resolver::ResourceNameResolution(resourceNameAndFileName);
    auto nodefullindexedname               = absrespath + std::string{"/"} + resname;
                                        
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
        auto pmesh                                          = std::dynamic_pointer_cast<GTech::Mesh>(scene.urlPtrMap[pnode->url]);
        auto vaoentriesarrayptr                             = vaoMap.RegisterVaoEntriesArray(nodefullindexedname, *pmesh); //Shared ptr to an array of VAO0...VAOI... VAON-1
        auto shaderMaterialPtrVector                        = shaderMaterialMap.RegisterShaderMaterialHeaderEntriesArray(nodefullindexedname, *pmesh, scene.urlPtrMap); //Sharedptr to an array of Mat0...Mat

        //Ok fill out the component (THIS IS HORRIBLE CODE, SHOULD BE ENHANCED SOME TIME)
        vaoarraycomponentptr->wkptr_vaoarray                = vaoentriesarrayptr;
        vaoarraycomponentptr->wkptr_materialheadercomponent = shaderMaterialPtrVector;
        vaoarraycomponentptr->size                          = pmesh->triangleArray.size();

        entitymngr.AddComponent(eid, vaoarraycomponentid);
        infocomponentptr->SetGlGeometryTuple(mtxcomponentid, vaoarraycomponentid);

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
        
    
    } else return 0; 

    return eid;
}

void GTech::ResourceManager::UnLoad(const std::string &resourceName){
    
}

void GTech::ResourceManager::ClearCache(){

    map_SResourceName_PairDocVisitor.clear();

}

