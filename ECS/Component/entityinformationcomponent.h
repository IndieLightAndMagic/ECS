#ifndef __ENTITYINFORMATIONCOMPONENT_H__
#define __ENTITYINFORMATIONCOMPONENT_H__

#include <vector>
#include <tuple>
#include <ECS/Component/component.h>

namespace ECS {

    class EntityInformationComponent_ : public Component_ {

        std::vector<std::tuple<unsigned int, unsigned int, unsigned int>> m_kinematicTupleIds{};
        std::tuple<unsigned int, unsigned int, unsigned int, unsigned int> m_renderingTupleIds{0, 0};

        std::tuple<unsigned int, unsigned int> m_glgeometry_tupleids{0, 0};
        std::tuple<unsigned int, unsigned int> m_gllight_tupleids{0, 0};
        std::tuple<unsigned int, unsigned int> m_glcam_tupleids{0, 0};

        public:
        /**
        * @brief      Determines if the entity is a geometry.
        *
        * @return     True if geometry, False otherwise.
        */
        bool IsGeometry() const;
        /**
         * @brief      Determines if the entity is a camera.
         *
         * @return     True if camera, False otherwise.
         */
        bool IsCamera() const;
        /**
         * @brief      Determines if the entity is a lamp.
         *
         * @return     True if lamp, False otherwise.
         */
        bool IsLamp() const;

        
        EntityInformationComponent_() = default;

        /*!
         * @brief Returns a vector of component-id tuples of <positionComponentId, speedComponentId, accelComponentId>, which are the ids of the kinematic compound.
         *
         * @return A tuple with the component Ids of the kinematic triad compound.
         *
         * @code
         *
         * auto [posId, speedId, accelId] = informationComponent.kinematicTupleIds();
         * auto& compoManager = ECS::ComponentManager::GetInstance();
         *
         * auto pPositionComponent = compoManager.GetComponentRaw<ECS::PositionComponent_>(posId);
         * auto pSpeedComponentPtr = compoManager.GetComponentRaw<ECS::SpeedComponent_>(speedId);
         * auto pAccelComponent = compoManager.GetComponentRaw<ECS::AccelComponent_>(accelId);
         *
         * speedComponent->x = 45.0f;
         *
         * @endcode
         */
        std::vector<std::tuple<unsigned int, unsigned int, unsigned int>> GetKinematicTuples() const;
        void SetKinematicTupleIds(unsigned int posId, unsigned int speedId, unsigned int accelId);

        /*!
         * @brief Returns a component id tuple of <positionComponentId, anglePositionId, anchorPointId, textureComponentId>, which are the ids of the rendering compound.
         *
         * @return A tuple with the component Ids of the rendering compound.
         *
         * @code
         *
         * auto [posId, anglePositionId, anchorPointId, textureId] = informationComponent.renderingTupleIds();
         * auto& compoManager = ECS::ComponentManager::GetInstance();
         *
         * auto pPositionComponent      = compoManager.GetComponentRaw<ECS::PositionComponent_>(posId);
         * auto pAngularPosComponent    = compoManager.GetComponentRaw<ECS::PositionComponent_>(anglePositionId);
         * auto pTextureComponent       = compoManager.GetComponentRaw<ECS::TextureComponent_>(textureId);
         * auto pAnchorPointComponent   = compoManager.GetComponentRaw<ECS::AnchorPointComponent_>(anchorPointId);
         *
         * auto [w, h] = pTextureComponent-> GetScaledSize();
         *
         * @endcode
         */
        std::tuple<unsigned int, unsigned int, unsigned int, unsigned int> GetRenderingTupleIds() const;
        void SetRenderingTupleIds(unsigned int posId, unsigned int anglePositionId, unsigned int anchorId, unsigned int textureComponentId);

        /**
         * @brief      Returns a component id tuple of <viewMatrixId, projectionMatrixId>, which are the ids of the matrix that compose the camera projections / positions / rotations. 
         *
         * @return     The gl camera tuple, the first one is the view matrix id, the second one is the projection matrix. 
         */
        std::tuple<unsigned int, unsigned int> GetGlCamTuple()const;
        void SetGlCamTuple(unsigned int viewMatrixId, unsigned int projectionMatrixId);

        /**
         * @brief      Returns a component id tuple of <transformMatrixId, shaderLightHeaderId>, which are the ids of the matrix that compose the light projections / positions / rotations. And the light's data to pass into the shaders.   
         *
         * @return     The gl light tuple, the first one is the transfor matrix id, the second one is the shader light header data. 
         */
        std::tuple<unsigned int, unsigned int> GetGlLightTuple()const;
        void SetGlLightTuple(unsigned int transformMatrixId, unsigned int shaderLightHeaderId);

        /**
         * @brief      Gets the gl geometry tuple. transform matrix component id, vaoarray component id
         *
         * @return     The gl geometry tuple. 
         */
        std::tuple<unsigned int, unsigned int> GetGlGeometryTuple() const;
        void SetGlGeometryTuple(unsigned int transformMatrixId, unsigned int vaoarrayId);

    };

}


#endif //__ENTITYINFORMATIONCOMPONENT_H__
