/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2015
 * @copyright VaL Doroshchuk
 * @license GKU GPL v2
 * @package bvh3
 */

#ifndef BVH3_ALL
#define BVH3_ALL

#include "KDop.hpp"
#include <bvh3/types/SVertex.hpp>
 
namespace NBvh3
{

/**
 * Creates bounding volume based on vertices.
 *
 * @param Applied vertices.
 */
template<class TBv>
TBv createBoundingVolume(const TVertices& vertices)
{
    TBv bv;
    for (unsigned i = 0; i < vertices.size(); ++i)
    {
        bv += vertices[i];
    }

    return bv;
}

} // namespace NBvh3

#endif // BVH3_ALL