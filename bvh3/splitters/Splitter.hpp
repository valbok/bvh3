/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2015
 * @copyright VaL Doroshchuk
 * @license GKU GPL v2
 * @package bvh3
 */

#ifndef BVH3_SPLITTER
#define BVH3_SPLITTER

#include <bvh3/types/SVertex.hpp>

namespace NBvh3
{

/**
 * Base abstract splitter class.
 */
class Splitter
{
public:

    /**
     * Destructor.
     */
    ~Splitter()
    {
    }

    /**
     * Splits original vertices to 2 vectors.
     *
     * @param[out] Vertices that are located left.
     * @param[out] Vertices that are located right.
     */    
    virtual void split(TVertices& left, TVertices& right) const = 0;
};

} // namespace NBvh3

#endif // BVH3_SPLITTER