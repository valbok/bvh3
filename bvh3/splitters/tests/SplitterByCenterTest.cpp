/**
 * @author VaL Doroshchuk
 * @license GNU GPL v2
 */

#include <bvh3/bv/all.hpp>
#include <bvh3/splitters/SplitterByCenter.hpp>
#include <gtest/gtest.h>

using namespace NBvh3;
using namespace std;

TEST(SplitterByCenter, testSplit)
{
    TVertices vertices = 
    {
        SVertex(3, 1, 0),
        SVertex(1, 5, 0),
        SVertex(5, 4, 0)
    };

    SplitterByCenter<KDop<16> > s(vertices, createBoundingVolume<KDop<16> >(vertices));
    TVertices left, right;
    s.split(left, right);

    EXPECT_EQ(2, left.size());
    EXPECT_EQ(1, right.size());
    EXPECT_EQ(SVertex(3, 1, 0), left[0]);
    EXPECT_EQ(SVertex(1, 5, 0), left[1]);
    EXPECT_EQ(SVertex(5, 4, 0), right[0]);
}
