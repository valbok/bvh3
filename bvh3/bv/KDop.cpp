/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2015
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 * @package bvh3
 */

#include "KDop.hpp"
#include <limits>

namespace NBvh3
{

/// Returns the distances to planes with normals from KDop vectors.
///
/// @params Vertex with coordinates.
/// @param[out] Result distances.
template<unsigned K>
void getDistances(const SVertex& vertex, float dists[]);

template<>
inline void getDistances<8>(const SVertex& vertex, float dists[])
{
    dists[0] = vertex.x;
    dists[1] = vertex.y;
    dists[2] = vertex.z;
    dists[3] = vertex.x + vertex.y;
    dists[4] = vertex.x + vertex.z;
    dists[5] = vertex.y + vertex.z;
    dists[6] = vertex.x - vertex.y;
    dists[7] = vertex.x - vertex.z;
}

template<>
inline void getDistances<9>(const SVertex& vertex, float dists[])
{
    getDistances<8>(vertex, dists);

    dists[8] = vertex.y - vertex.z;
}

template<>
inline void getDistances<12>(const SVertex& vertex, float dists[])
{
    getDistances<9>(vertex, dists);

    dists[9] = vertex.x + vertex.y - vertex.z;
    dists[10] = vertex.x + vertex.z - vertex.y;
    dists[11] = vertex.y + vertex.z - vertex.x;
}

template<unsigned K>
KDop<K>::KDop() throw()
{
    float max = std::numeric_limits<float>::max();
    for(unsigned i = 0; i < K / 2; ++i)
    {
        mMin[i] = max;
        mMax[i] = -max;
    }
}

template<unsigned K>
KDop<K>::KDop(const SVertex& vertex) throw()
{
    float dists[K / 2];
    getDistances<K / 2>(vertex, dists);
    for (unsigned i = 0; i < K / 2; ++i)
    {
        mMin[i] = mMax[i] = dists[i];
    }
}

static inline void setMinMax(float value, float& minValue, float& maxValue)
{
    if (value > maxValue)
    {
        maxValue = value;
    }

    if (value < minValue)
    {
        minValue = value;
    }
}

template<unsigned K>
KDop<K>& KDop<K>::operator += (const SVertex& vertex)
{
    float dists[K / 2];
    getDistances<K / 2>(vertex, dists);
    for(unsigned i = 0; i < K / 2; ++i)
    {
        setMinMax(dists[i], mMin[i], mMax[i]);
    }

    return *this;
}

template<unsigned K>
KDop<K>& KDop<K>::operator += (const KDop<K>& other)
{
    for (unsigned i = 0; i < K / 2; ++i)
    {
        if (other.mMin[i] < mMin[i])
        {
            mMin[i] = other.mMin[i];
        }

        if (other.mMax[i] > mMax[i])
        {
            mMax[i] = other.mMax[i];
        }
    }

    return *this;
}

template<unsigned K>
KDop<K> KDop<K>::operator + (const KDop<K>& other) const
{
    return KDop<K>(*this) += other;
}

template<unsigned K>
bool KDop<K>::overlaps(const KDop<K>& other) const
{
    for (unsigned i = 0; i < K / 2; ++i)
    {
        if (mMin[i] > other.mMax[i] || mMax[i] < other.mMin[i])
        {
            return false;
        }
    }

    return true;
}

template<unsigned K>
float KDop<K>::getMin(unsigned i) const
{
    return i < K / 2 ? mMin[i] : 0;
}

template<unsigned K>
float KDop<K>::getMax(unsigned i) const
{
    return i < K / 2 ? mMax[i] : 0;
}

// Explicit template class specializations.
template class KDop<16>;
template class KDop<18>;
template class KDop<24>;

} // namespace NBvh3
