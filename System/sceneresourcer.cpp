
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

    auto trytofindthisresource = map_SResourceName_PairDocVisitor.find(resourceFilenamePathStr);
    auto resourcewasnotfound = map_SResourceName_PairDocVisitor.end();
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
    auto pmesh = std::dynamic_pointer_cast<GTech::Mesh>(scene.urlPtrMap[pnode->url]);

    // Create Mtx Component and assign the node's transform matrix (a copy).
    auto mngr            = ECS::ComponentManager::GetInstance();
    auto mtxcomponentid  = mngr.CreateComponent<ECS::MatrixComponent_>();
    auto mtxcomponentptr = mngr.GetComponentRaw<ECS::MatrixComponent_>(mtxcomponentid);
    mtxcomponentptr->matrix = pnode->transform;

    // Ok 
    if (pnode->nodeType == GTech::Node::NodeType::MESH){

        //VAO
        
        //Allocate Vertex Array Objects VAO and with  VBOs and EBOs
        auto vaoptr = vaoMap.CreateVaoEntry(nodefullindexedname, resname, *pmesh);



    } else if (pnode->nodeType == GTech::Node::NodeType::CAMERA){



    } else if (pnode->nodeType == GTech::Node::NodeType::LIGHT) {



    }

    auto entitymanager   = ECS::EntityManager::GetInstance();
    auto eId             = entitymanager.CreateEntity();

    return 0;
}

void GTech::ResourceManager::UnLoad(const std::string &resourceName){
    
}

void GTech::ResourceManager::ClearCache(){

    map_SResourceName_PairDocVisitor.clear();

}

