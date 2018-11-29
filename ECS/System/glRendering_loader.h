#ifndef __GLRENDERING_DATA_H__
#define __GLRENDERING_DATA_H__



#include <map>
#include <memory>
#include <string>


#include <ECS/Component/componentmanager.h>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#else 
#include <OpenGL/gl.h>
#endif /*__APPLE__*/
#include <collader/collader.h>
#include <collader/idname.h>
#include <collader/node.h>
#include <collader/scene.h>

namespace GTECH {
    
    template <typename V, typename K, typename C>
    class SolvableMap {
        
        std::map<K, std::weak_ptr<V>> m {};
        C& context;
        std::function<std::shared_ptr<V>(K, C&)> fresolution;
        
    public:
        std::shared_ptr<V> operator[](const K& key){
                
            auto wp_v = m[key];
            auto sp_v = wp_v.lock();
                
            if (!sp_v){
                sp_v = fresolution(key, context);
                m[key] = sp_v;
            }
            return sp_v;
                
        }
        AssetMap(C& c, std::function<std::shared_ptr<V>(K, C&)>f ) : context(c), fresolution(f){}
            
             
    };

}
namespace ECS {
    


    class RenderingDataManager {


        static std::map<std::string, std::weak_ptr<unsigned int>> meshname_vaoarray_map; 
        static std::map<std::string, std::weak_ptr<ECS::MaterialComponent>> meshname_materialarray_map;

    public:

        static RenderingDataManager& GetInstance(){

            static RenderingDataManager rdm;
            return rdm; 

        }



    };
}

#endif