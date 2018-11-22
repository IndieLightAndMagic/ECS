

#include <ECS/Component/entityinformationcomponent.h>

using namespace std;
using namespace ECS;


bool EntityInformationComponent_::IsLamp() const {
	auto [_0, _1] = m_gllight_tupleids;
	if (!_0 || !_1) return false;
	return true;
}

bool EntityInformationComponent_::IsCamera() const {
	auto [_0, _1] = m_glcam_tupleids;
	if (!_0 || !_1) return false;
	return true;
}

bool EntityInformationComponent_::IsGeometry() const {
	auto [_0, _1] = m_glgeometry_tupleids;
	if (!_0 || !_1) return false;
	return true;
}

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
	return m_glcam_tupleids;
}

void EntityInformationComponent_::SetGlCamTuple(unsigned int viewMatrixId, unsigned int projectionMatrixId){
	m_glcam_tupleids = tuple<unsigned int, unsigned int>{viewMatrixId, projectionMatrixId};
}

std::tuple<unsigned int, unsigned int> EntityInformationComponent_::GetGlLightTuple() const{
	return m_gllight_tupleids;
}

void EntityInformationComponent_::SetGlLightTuple(unsigned int transforMatrixId, unsigned int shaderLightHeaderId){
	m_gllight_tupleids = tuple<unsigned int, unsigned int>{transforMatrixId, shaderLightHeaderId};
}

std::tuple<unsigned int, unsigned int> EntityInformationComponent_::GetGlGeometryTuple() const{
	return m_glgeometry_tupleids;
}

void EntityInformationComponent_::SetGlGeometryTuple(unsigned int transformMatrixId, unsigned int vaoArrayId) {
	m_glgeometry_tupleids = tuple<unsigned int, unsigned int>{transformMatrixId, vaoArrayId};
}
