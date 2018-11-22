

#include <ECS/Component/entityinformationcomponent.h>

using namespace std;
using namespace ECS;

vector<tuple<unsigned int, unsigned int, unsigned int>>  EntityInformationComponent_::GetKinematicTuples() const{
    return m_kinematicTupleIds;
};


void EntityInformationComponent_::SetKinematicTupleIds(unsigned int posId, unsigned int speedId, unsigned int accelId){
    m_kinematicTupleIds.push_back(tuple<unsigned  int, unsigned int, unsigned  int>{posId, speedId, accelId}) ;
}

tuple<unsigned int, unsigned int, unsigned int, unsigned int> EntityInformationComponent_::GetRenderingTupleIds() const{
    return m_renderingTupleIds;
};

void EntityInformationComponent_::SetRenderingTupleIds(unsigned int posId, unsigned int anglePositionId, unsigned int anchorId, unsigned int textureId) {
    m_renderingTupleIds = tuple<unsigned  int, unsigned int, unsigned int, unsigned int>{posId, anglePositionId, anchorId, textureId};
}

std::tuple<unsigned int, unsigned int> EntityInformationComponent_::GetGlCamTuple() const{
	return m_2ui;
}

void EntityInformationComponent_::SetGlCamTuple(unsigned int viewMatrixId, unsigned int projectionMatrixId){
	m_2ui = tuple<unsigned int, unsigned int>{viewMatrixId, projectionMatrixId};
}

std::tuple<unsigned int, unsigned int> EntityInformationComponent_::GetGlLightTuple() const{
	return m_2ui;
}

void EntityInformationComponent_::SetGlLightTuple(unsigned int transforMatrixId, unsigned int shaderLightHeaderId){
	m_2ui = tuple<unsigned int, unsigned int>{transforMatrixId, shaderLightHeaderId};
}

std::tuple<unsigned int, unsigned int> EntityInformationComponent_::GetGlGeometryTuple() const{
	return m_2ui;
}

void EntityInformationComponent_::SetGlGeometryTuple(unsigned int transformMatrixId, unsigned int vaoArrayId) {
	m_2ui = tuple<unsigned int, unsigned int>{transformMatrixId, vaoArrayId};
}
