#include "VertexOffsetParameter.h"

VertexOffsetParameter::VertexOffsetParameter(
        WireNetwork::Ptr wire_network, size_t axis)
: PatternParameter(wire_network), m_axis(axis) {
    assert(m_axis < m_wire_network->get_dim());
    const VectorF bbox_min = m_wire_network->get_bbox_min();
    const VectorF bbox_max = m_wire_network->get_bbox_max();
    m_center = 0.5 * (bbox_min + bbox_max);
}

void VertexOffsetParameter::apply(VectorF& results,
        const PatternParameter::Variables& vars) {
    const size_t dim = m_wire_network->get_dim();
    const size_t num_vertices = m_wire_network->get_num_vertices();
    const size_t roi_size = m_roi.size();
    assert(m_axis < dim);
    assert(results.size() == dim * num_vertices);

    if (m_formula != "") evaluate_formula(vars);

    const MatrixFr& vertices = m_wire_network->get_vertices();

    for (size_t i=0; i<roi_size; i++) {
        size_t v_idx = m_roi[i];
        assert(v_idx < num_vertices);
        const VectorF& v = vertices.row(v_idx);

        results[v_idx * dim + m_axis] =
            (v[m_axis] - m_center[m_axis]) * m_value;
    }
}