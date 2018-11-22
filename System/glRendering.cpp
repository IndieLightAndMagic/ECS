#include <tuple>
#include <vector>
#include <utility>
#include <ECS/System/glRendering.h>

using namespace ECS;

namespace ECS{

    //A struct which binds a mtx component with a material component
    struct DrawingPair {

        void* pmtx{nullptr};
        void* pmat{nullptr};
    };
    using DrawingVector = std::vector<ECS::DrawingPair>; 
    
    struct DrawingTree {

        unsigned int vao;
        DrawingVector pairvec{};
        ECS::DrawingPair* ppairvec{pairvec.data()};
    
    }; 

    using VaoVector = std::vector<ECS::DrawingTree>;


    struct  {
    
        ECS::VaoVector vv{};
        ECS::DrawingTree* pvv{vv.data()};

    }drawingdata; 
}

ECS::RenderingSystem::Result SubscribeEntity(unsigned int eid){

    auto& componentmanager     = ECS::ComponentManager::GetInstance();
    auto& informationcomponent = componentmanager.GetInformationComponent(eid);
    
    //Check by component type what is the entity.
    if (informationcomponent.IsGeometry()){

        auto [matrixid, vaoarrayid] = informationcomponent.GetGlGeometryTuple();
        auto matrixptr              = componentmanager.GetComponentRaw<ECS::MatrixComponent_>(matrixid);
        auto vaoarrayptr            = componentmanager.GetComponentRaw<ECS::VaoArrayComponent_>(vaoarrayid);
        

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
    
    //For each vao
    auto itlimit0 = drawingdata.pvv + drawingdata.vv.size();
    for ( auto pdrawingtree = drawingdata.pvv; pdrawingtree < itlimit0; ++pdrawingtree ){

        auto vao = pdrawingtree->vao;

        //For each matrix,material pair
        auto itlimit1 = pdrawingtree->ppairvec + pdrawingtree->pairvec.size();
        for ( auto ppair = pdrawingtree->ppairvec; ppair < itlimit1; ++ppair) {

            auto pmtx = reinterpret_cast<glm::mat4*>(ppair->pmtx);
            auto pmat = reinterpret_cast<ECS::ShaderMaterialHeaderComponent_*>(ppair->pmat);

            if (!pmtx || !pmat) continue;



        } 


    }    

}

ECS::RenderingSystem::Result RemoveEntity(unsigned int eid){

    
    return ECS::RenderingSystem::Result::OK;
}

ECS::RenderingSystem::Result Stop(){

    return ECS::RenderingSystem::Result::OK;
}