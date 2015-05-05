/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2015
 * @copyright VaL Doroshchuk
 * @license GKU GPL v2
 * @package bvh3
 */

#ifndef BVH3_SPLITTERBYCENTER
#define BVH3_SPLITTERBYCENTER

#include "Splitter.hpp"
#include <iostream>
namespace NBvh3
{

/**
 * Template class to split vertices by a center of bounding volume created on submitted vertices.
 */
template<class TBv>
class SplitterByCenter : public Splitter
{
public:

    /**
     * Default constructor.
     *
     * @param Applied vertices to split.
     * @param Bounding volume of applied vertices.
     */
    SplitterByCenter(const TVertices& vertices, const TBv& bv);

    /**
     * @copydoc Splitter::split()
     */
    virtual void split(TVertices& left, TVertices& right) const;

private:

    /**
     * Checks if sumbitted vertex located right from the center.
     */
    bool isRight(const SVertex& vertex) const;

    /**
     * Submitted vertices.
     */
    const TVertices& mVertices;

    /**
     * Produces bounding volume of vertices.
     */
    const TBv& mBv;

    /**
     * Number of axis that is used to find a center.
     */
    unsigned mAxis;

    /**
     * Value of center axis.
     */
    float mAxisValue;
};

template<class TBv>
SplitterByCenter<TBv>::SplitterByCenter(
    const TVertices& vertices,
    const TBv& bv
    )
    : mVertices(vertices)
    , mBv(bv)
    , mAxis(2)
    , mAxisValue(0)
{
    SVertex center = mBv.getCenter();

    if (mBv.getWidth() >= mBv.getHeight() && mBv.getWidth() >= mBv.getDepth())
    {
        mAxis = 0;
    }
    else if (mBv.getHeight() >= mBv.getWidth() && mBv.getHeight() >= mBv.getDepth())
    {
        mAxis = 1;
    }

    mAxisValue = center[mAxis];
}

template<class TBv>
bool SplitterByCenter<TBv>::isRight(const SVertex& vertex) const
{
    return vertex[mAxis] > mAxisValue;
}

template<class TBv>
void SplitterByCenter<TBv>::split(TVertices& left, TVertices& right) const
{
    for (unsigned i = 0; i < mVertices.size(); ++i)
    {
        if (isRight(mVertices[i]))
        {
            right.push_back(mVertices[i]);
        }
        else
        {
            left.push_back(mVertices[i]);
        }
    }
}

} // namespace NBvh3

#endif // BVH3_SPLITTERBYCENTER