#include <tuple>
#include <ECS/System/glRendering.h>

using namespace ECS;



ECS::RenderingSystem::Result SubscribeEntity(unsigned int eid){

    auto& componentmanager     = ECS::ComponentManager::GetInstance();
    auto& informationcomponent = componentmanager.GetInformationComponent(eid);
    
    //Check by component type what is the entity.
    if (informationcomponent.IsGeometry()){

        auto [matrixid, vaoarrayid] = informationcomponent.GetGlGeometryTuple();
        auto matrixptr              = componentmanager.GetComponentRaw<ECS::MatrixComponent_>(matrixid);
        auto vaoarrayptr            = componentmanager.GetComponentRaw<ECS::MatrixComponent_>(vaoarrayid);

 
    } else if (informationcomponent.IsCamera()){

        auto [matrixid, projectionmatrixid] = informationcomponent.GetGlCamTuple();
        auto matrixptr                      = componentmanager.GetComponentRaw<ECS::MatrixComponent_>(matrixid);
        auto proejctionmatrixptr            = componentmanager.GetComponentRaw<ECS::MatrixComponent_>(projectionmatrixid);


    } else if (informationcomponent.IsLamp()) {

        auto [matrixid, shaderlightheaderid] = informationcomponent.GetGlLightTuple();
        auto matrixptr = componentmanager.GetComponentRaw<ECS::MatrixComponent_>(matrixid);
        auto shaderlightheaderptr = componentmanager.GetComponentRaw<ECS::ShaderLightHeaderComponent_>(shaderlightheaderid);

    } else return ECS::RenderingSystem::Result::ERROR;

    return ECS::RenderingSystem::Result::OK;
}

void Draw(){

}

ECS::RenderingSystem::Result RemoveEntity(unsigned int eid){


    return ECS::RenderingSystem::Result::OK;
}

ECS::RenderingSystem::Result Stop(){

    return ECS::RenderingSystem::Result::OK;
}