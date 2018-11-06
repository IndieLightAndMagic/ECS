#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <cxxabi.h>
#include <string>

#include <glm/glm.hpp>
#include <SDL2/SDL_system.h>
#include <SDL2/SDL_assert.h>
#include <ECS/Entity/entity.h>


namespace ECS {


    class Component_ {

    protected:
        unsigned int    m_id;
        unsigned int    m_parentId;
        bool            m_dirty;
        unsigned int    reuse{0};

    public:
        Component_(const Component_&) = default;
        Component_() :
                m_parentId(0),
                m_id(0),
                m_dirty(true){}

        friend class ComponentFactory;
        friend class ComponentManager;

        inline bool IsDirty(){

            return m_dirty;

        }

        virtual std::string GetType() {

            auto name = typeid(*this).name();
            auto status = 4;
            std::unique_ptr<char, void (*)(void *)> res{
                    abi::__cxa_demangle(name, nullptr, nullptr, &status),
                    std::free
            };
            return (status == 0 ? res.get() : name);

        }

        virtual ~Component_(){}
    };

    using Component = std::shared_ptr<Component_>;
    using _Component = std::weak_ptr<Component_>;



    class SpeedComponent_;
    using SpeedComponent = std::shared_ptr<SpeedComponent_>;
    class SpeedComponent_ : public Component_{
    public:
        glm::vec3 speed{0.0f, 0.0f, 0.0f};
    };

    class AccelerationComponent_;
    using AccelerationComponent = std::shared_ptr<AccelerationComponent_>;
    class AccelerationComponent_ : public Component_{
    public:
        glm::vec3 acceleration{0.0f, 0.0f, 0.0f};
    };

    class PositionComponent_;
    using PositionComponent = std::shared_ptr<PositionComponent_>;
    class PositionComponent_ : public Component_ {

    public:
        glm::vec3 position{0.0f, 0.0f, 0.0f};
        bool isDirty;
    };

    class AnchorPointComponent_;
    using AnchorpointComponent = std::shared_ptr<AnchorPointComponent_>;
    class AnchorPointComponent_ : public Component_ {
        public:
            glm::vec3 m_correctionVector{0.0f, 0.0f, 0.0f};
            glm::vec3 m_anchorPoint{0.0f, 0.0f, 0.0f};
            void SetAnchorPoint(glm::vec3 anchorPoint, glm::vec3 box);
    };


    class MatrixComponent_;
    using MatrixComponent = std::shared_ptr<MatrixComponent_>;
    class MatrixComponent_ : public Component_ {

    public:
        glm::mat4 matrix;

    };
    
    class VaoArrayComponent_;
    using VaoArrayComponent = std::shared_ptr<VaoArrayComponent_>;
    class VaoArrayComponent_ : public Component_ {
    public:
        std::weak_ptr<unsigned int>vaoarray;

    };

}


#endif /*__COMPONENT_H__*/