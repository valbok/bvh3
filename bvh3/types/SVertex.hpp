/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2015
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 * @package bvh3
 */

#ifndef BVH3_SVERTEX
#define BVH3_SVERTEX

#include <vector>

namespace NBvh3
{

/**
 * Defines coordinates of a vertex.
 */
struct SVertex
{
    /**
     * Default constructor.
     */
    SVertex() : SVertex(0, 0, 0)
    {
    }

    /**
     * @param x Axis
     * @param y Axis
     * @param z Axis
     */
    SVertex(float xx, float yy, float zz) 
        : x(xx), y(yy), z(zz)
    {
    }

    /**
     * Multiples coordinates.
     */
    inline SVertex operator * (float t) const 
    { 
        return SVertex(x * t, y * t, z * t); 
    }

    /**
     * Returns axis value.
     */
    inline float operator [] (unsigned i) const 
    {
        float result = 0;
        switch (i)
        {
            case 0:
                result = x;
            break;
            case 1:
                result = y;
            break;
            case 2:
                result = z;
            break;
        }

        return result;
    }

    /**
     * Checks if it equals to.
     */
    inline bool operator == (const SVertex& other) const 
    {
        return x == other.x && y == other.y && z == other.z;
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

/**
 * Vector of vertices.
 */
typedef std::vector<SVertex> TVertices;

} // namespace NBvh3

#endif // BVH3_SVERTEX