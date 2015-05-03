/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2015
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 * @package bvh3
 */

#ifndef BVH3_SVERTEX
#define BVH3_SVERTEX

namespace NBvh3
{

/**
 * Defines coordinates of a vertex.
 */
struct SVertex
{
    /**
     * Default constructor.
     *
     * @param x Axis
     * @param y Axis
     * @param z Axis
     */
    SVertex(float xx, float yy, float zz) : x(xx), y(yy), z(zz)
    {
    }

    /**
     * Axis x.
     */
    float x;

    /**
     * Axis y.
     */
    float y;

    /**
     * Axis z.
     */
    float z;
};

} // namespace NBvh3

#endif // BVH3_SVERTEX