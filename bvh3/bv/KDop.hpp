/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2015
 * @copyright VaL Doroshchuk
 * @license GKU GPL v2
 * @package bvh3
 */

#ifndef BVH3_KDOP
#define BVH3_KDOP

#include <bvh3/types/SVertex.hpp>

namespace NBvh3
{

/**
 * Template class of KDop bounding valume structure.
 * K is set as the template parameter, which should be 16, 18, or 24.
 *
 * For K = 16, the planes are 6 AABB planes and 10 diagonal planes that cut off some space of the edges:
 * Stores min and max distance values. Directions defined by following indeces:
 * INDEX : DIRECTION VECTOR
 * 0     : (1,0,0)
 * 1     : (0,1,0)
 * 2     : (0,0,1)
 * 3     : (1,1,0)
 * 4     : (1,0,1)
 * 5     : (0,1,1)
 * 6     : (1,-1,0)
 * 7     : (1,0,-1)
 *
 * For K = 18, the planes are 6 AABB planes and 12 diagonal planes that cut off some space of the edges:
 * INDEX : DIRECTION VECTOR
 * 8     : (0,1,-1)
 *
 * For K = 24, the planes are 6 AABB planes and 18 diagonal planes that cut off some space of the edges:
 * INDEX : DIRECTION VECTOR
 * 9     : (1,1,-1)
 * 10    : (1,-1,1)
 * 11    : (-1,1,1)
 */
template<unsigned K>
class KDop
{
public:

    /**
     * Default constructor to create empty object.
     */
    KDop() throw();

    /**
     * Creates KDop for one point/vertex.
     */
    KDop(const SVertex& vertex) throw();

    /**
     * Checks if current KDop object overlabs other.
     */
    bool overlaps(const KDop<K>& other) const;

    KDop<K>& operator += (const SVertex& vertex);
    KDop<K>& operator += (const KDop<K>& other);
    KDop<K> operator + (const KDop<K>& other) const;

    /**
     * Returns min distance by index of axis.
     */
    float getMin(unsigned axisIndex) const;

    /**
     * Returns max distance by index of axis.
     */
    float getMax(unsigned axisIndex) const;

private:

    /**
     * Stores minimum distances for axes [0, K/2).
     */     
    float mMin[K / 2];

    /**
     * Stores maximum distances for axes [0, K/2).
     */     
    float mMax[K / 2];
};

} // namespace NBvh3

#endif // BVH3_KDOP