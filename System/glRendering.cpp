#include <tuple>
#include <vector>
#include <utility>
#include <ECS/System/glRendering.h>
#include <SDLWrapper/sdlwrapper.h>

using namespace ECS;

namespace ECS{

    struct NodeDrawingData {

        glm::mat4* pmatrix{nullptr};
        
        std::shared_ptr<unsigned int> vaoarrayptr;
        unsigned int* vaoarrayptr_; //VAO0....VAON-1

        ECS::ShaderMaterialHeaderComponent pmaterialcomponent;
        ECS::ShaderMaterialHeaderComponent_* pmaterialcomponent_; //MAT0....MATN-1
        
        unsigned int* psz; // Ptr to the Value of N in VAON-1 and MATN-1.    
    }; 
    using NodeDrawingDataVector = std::vector<ECS::NodeDrawingData>;

    NodeDrawingDataVector nodedrawingdatavector{};
}

ECS::RenderingSystem::Result SubscribeEntity(unsigned int eid){

    auto& componentmanager     = ECS::ComponentManager::GetInstance();
    auto& informationcomponent = componentmanager.GetInformationComponent(eid);
    
    //Check by component type what is the entity.
    if (informationcomponent.IsGeometry()){

        auto [matrixid, vaoarrayid] = informationcomponent.GetGlGeometryTuple();
        auto matrixptr              = componentmanager.GetComponentRaw<ECS::MatrixComponent_>(matrixid);
        auto vaoarrayptr            = componentmanager.GetComponentRaw<ECS::VaoArrayComponent_>(vaoarrayid);

        ECS::NodeDrawingData nodedrawingdata{};
        
        //Pointer to matrix
        nodedrawingdata.pmatrix          = &matrixptr->matrix;

        //Pointer to array of VAO0..VAON
        nodedrawingdata.vaoarrayptr      = vaoarrayptr->wkptr_vaoarray.lock();
        nodedrawingdata.vaoarrayptr_     = nodedrawingdata.vaoarrayptr.get();
        if (nodedrawingdata.vaoarrayptr_ == nullptr) return ECS::RenderingSystem::Result::ERROR;
        
        //Pointer to array of MAT0..MATN
        nodedrawingdata.pmaterialcomponent      = vaoarrayptr->wkptr_materialheadercomponent.lock();
        nodedrawingdata.pmaterialcomponent_     = nodedrawingdata.pmaterialcomponent.get();
        if (nodedrawingdata.pmaterialcomponent_ == nullptr) return ECS::RenderingSystem::Result::ERROR;

        //Pointer to the size of both arrays
        nodedrawingdata.psz = &vaoarrayptr->size;        

        ECS::nodedrawingdatavector.push_back(nodedrawingdata);

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

void ECS::RenderingSystem::Draw(){
    
    auto const elementstodraw = ECS::nodedrawingdatavector.size();
    auto nodedrawingdatavector_ = nodedrawingdatavector.data();
    auto endofelementstodraw = nodedrawingdatavector_ + elementstodraw;


    for(;nodedrawingdatavector_ < endofelementstodraw; ++nodedrawingdatavector_){

        //Get the element shader 
        auto pmatrix            = nodedrawingdatavector_->pmatrix;

        //For each (VAO MATERIAL PAIR try to draw the VAO using the MATERIAL's registered shader).
        auto endofiteration = nodedrawingdatavector_->vaoarrayptr_ + *nodedrawingdatavector_->psz;
        auto vaoptr         = nodedrawingdatavector_->vaoarrayptr_;
        auto matptr         = nodedrawingdatavector_->pmaterialcomponent_;
        auto lastshdr       = 0;
        for (;vaoptr < endofiteration; ++vaoptr, ++matptr){
            
            //Select shader for use.
            auto shaderptr_ = matptr->pshaderprog.lock().get();
            if (!shaderptr_) continue;
            auto actualshdr = (*shaderptr_)();
            if (actualshdr != lastshdr){
                lastshdr = actualshdr;
                shaderptr_->use();
                matptr->pshaderprog_ = shaderptr_;
            }

            //Attach material to shader. 
            //TODO: User should bring her/his own shader and his/her own ShaderHeaderComponent. This is not trivial.
            matptr->materialattachmentfunction(matptr);    
            //Set Camera.
            //TODO: User should bring her/his own camera attaching to vertex shader. Again this is not trivial 

            //Select which geometry to draw
            glBindVertexArray(*vaoptr);

        }


    }

    //Splash.
    GTech::SDLGlSwapWindow();

}

ECS::RenderingSystem::Result RemoveEntity(unsigned int eid){

    
    return ECS::RenderingSystem::Result::OK;
}

ECS::RenderingSystem::Result Stop(){

    return ECS::RenderingSystem::Result::OK;
}