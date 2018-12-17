
#include <utility>
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
    
    //Get pointer to scene               
    auto pscene    = sceneresourcemanagermap[absrespath];
    auto scene     = *pscene;

    auto nodefound = scene.nodes.find(resname) != scene.nodes.end();
                                                                                                                                                                                                                                                                                                                                                         
    if (!nodefound) return 0;

    // Ok node was found... start creating components.
    auto pmap  = pscene->nodes;
    auto pnode = pmap[resname];
    
    // Create Mtx Component and assign the node's transform matrix (a copy).
    // Invoke component manager.
    auto [componentmngr, entitymngr] = std::make_pair(ECS::ComponentManager::GetInstance(), ECS::EntityManager::GetInstance()) ;

    //Create entity and get its info component
    auto eid              = entitymngr.CreateEntity(); //entityid
    auto infocomponentptr = componentmngr.GetComponentRaw<ECS::EntityInformationComponent_>(entitymngr.GetComponentsIds(eid)[0]);

    // Create Mtx Component using the component manager.
    auto mtxcomponentid     = componentmngr.CreateComponent<ECS::MatrixComponent_>();
    auto mtxcomponentptr    = componentmngr.GetComponentRaw<ECS::MatrixComponent_>(mtxcomponentid);
    mtxcomponentptr->matrix = pnode->transform;

    entitymngr.AddComponent(eid, mtxcomponentid);    //Ok now we have a transform matrix. 
    
    // Ok 
    if (pnode->nodeType == GTech::Node::NodeType::MESH){

        //Check if VAO (the geometry describing the mesh node) is already registered by the rendering system.
        auto vao_sp = meshvaoarraymap[nodefullindexedname];
        if (vao_sp) return eid;


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

    //map_SResourceName_PairDocVisitor.clear();

}

