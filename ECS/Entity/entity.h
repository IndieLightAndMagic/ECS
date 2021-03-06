#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <vector>
#include <memory>

namespace ECS {


    class Entity_;
    using Entity = std::shared_ptr<Entity_>;
    class EntityManager;
    
    class Entity_ {
        std::vector<unsigned int> m_componentIds;
    public:
        unsigned int m_id;

        friend class EntityManager;

    };

}

#endif /*__ENTITY_H__*/

