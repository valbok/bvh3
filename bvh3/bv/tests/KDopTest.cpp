/**
 * @author VaL Doroshchuk
 * @license GNU GPL v2
 */

#include <bvh3/bv/KDop.hpp>
#include <gtest/gtest.h>
#include <vector>

using namespace NBvh3;
using namespace std;

TEST(KDopTest, testPointsOverlap)
{
    KDop<16> bv1(SVertex(3, 1, 0));
    KDop<16> bv2(SVertex(1, 5, 0));

    EXPECT_FALSE(bv1.overlapped(bv2));
    EXPECT_TRUE(bv1.overlapped(bv1));
}

void testMinMax(const KDop<16> bv)
{
    // x or (1, 0, 0)
    EXPECT_EQ(1, bv.getMin(0));
    EXPECT_EQ(3, bv.getMax(0));

    // y or (0, 1, 0)
    EXPECT_EQ(1, bv.getMin(1));
    EXPECT_EQ(5, bv.getMax(1));

    // z or (0, 0, 1)
    EXPECT_EQ(0, bv.getMin(2));
    EXPECT_EQ(0, bv.getMax(2));

    // x + y or (1, 1, 0)
    EXPECT_EQ(4, bv.getMin(3));
    EXPECT_EQ(6, bv.getMax(3));

    // x + z or (1, 0, 1)
    EXPECT_EQ(1, bv.getMin(4));
    EXPECT_EQ(3, bv.getMax(4));

    // y + z or (0, 1, 1)
    EXPECT_EQ(1, bv.getMin(5));
    EXPECT_EQ(5, bv.getMax(5));

    // x - y or (1, -1, 0)
    EXPECT_EQ(-4, bv.getMin(6));
    EXPECT_EQ(2, bv.getMax(6));

    // x - z or (1, 0, -1)
    EXPECT_EQ(1, bv.getMin(7));
    EXPECT_EQ(3, bv.getMax(7));
}

TEST(KDopTest, testPlusEqualKDop)
{
    KDop<16> bv1(SVertex(3, 1, 0));
    KDop<16> bv2(SVertex(1, 5, 0));

    bv1 += bv2;
    testMinMax(bv1);
}

TEST(KDopTest, testPlusEqualVertex)
{
    KDop<16> bv1(SVertex(3, 1, 0));
    bv1 += SVertex(1, 5, 0);

    testMinMax(bv1);
}

TEST(KDopTest, testPlusKDop16)
{
    KDop<16> bvSrc(SVertex(3, 1, 0));
    KDop<16> bvDst(SVertex(1, 5, 0));
    KDop<16> bv1 = bvSrc + bvDst;

    testMinMax(bv1);
}

TEST(KDopTest, testKDopTriangle)
{
    TVertices triangle =
    {
        SVertex(3, 1, 0),
        SVertex(1, 5, 0),
        SVertex(5, 4, 0)
    };

    KDop<16> bv;
    for (unsigned i = 0; i < 3; ++i)
    {
        bv += triangle[i];
    }

    // x
    EXPECT_EQ(1, bv.getMin(0));
    EXPECT_EQ(5, bv.getMax(0));

    // y
    EXPECT_EQ(1, bv.getMin(1));
    EXPECT_EQ(5, bv.getMax(1));

    // z
    EXPECT_EQ(0, bv.getMin(2));
    EXPECT_EQ(0, bv.getMax(2));

    // x + y
    EXPECT_EQ(4, bv.getMin(3));
    EXPECT_EQ(9, bv.getMax(3));

    // x - y
    EXPECT_EQ(-4, bv.getMin(6));
    EXPECT_EQ(2, bv.getMax(6));
}

TEST(KDopTest, testoverlapped)
{
    TVertices triangle1 =
    {
        SVertex(3, 1, 0),
        SVertex(1, 5, 0),
        SVertex(5, 4, 0)
    };

    TVertices triangle2 =
    {
        SVertex(3, 1, 0),
        SVertex(4, 2, 0),
        SVertex(6, 1, 0)
    };

    KDop<16> bv1;
    for (unsigned i = 0; i < 3; ++i)
    {
        bv1 += triangle1[i];
    }

    KDop<16> bv2;
    for (unsigned i = 0; i < 3; ++i)
    {
        bv2 += triangle2[i];
    }

    EXPECT_TRUE(bv1.overlapped(bv2));
}

TEST(KDopTest, testoverlappedNeg)
{
    TVertices triangle1 =
    {
        SVertex(3, 1, 0),
        SVertex(1, 5, 0),
        SVertex(5, 4, 0)
    };

    TVertices triangle2 =
    {
        SVertex(4, 1, 0),
        SVertex(5, 2, 0),
        SVertex(7, 1, 0)
    };

    KDop<16> bv1;
    for (unsigned i = 0; i < 3; ++i)
    {
        bv1 += triangle1[i];
    }

    KDop<16> bv2;
    for (unsigned i = 0; i < 3; ++i)
    {
        bv2 += triangle2[i];
    }

    EXPECT_FALSE(bv1.overlapped(bv2));
}

TEST(KDopTest, testoverlappedInside)
{
    TVertices triangle1 =
    {
        SVertex(3, 1, 0),
        SVertex(1, 5, 0),
        SVertex(5, 4, 0)
    };

    TVertices triangle2 =
    {
        SVertex(3, 3, 0),
        SVertex(3, 4, 0),
        SVertex(4, 3, 0)
    };

    KDop<16> bv1;
    for (unsigned i = 0; i < 3; ++i)
    {
        bv1 += triangle1[i];
    }

    KDop<16> bv2;
    for (unsigned i = 0; i < 3; ++i)
    {
        bv2 += triangle2[i];
    }

    EXPECT_TRUE(bv1.overlapped(bv2));
    EXPECT_TRUE(bv2.overlapped(bv1));
}

TEST(KDopTest, testoverlappedLine)
{
    TVertices line =
    {
        SVertex(3, 1, 0),
        SVertex(1, 5, 0),
    };

    TVertices triangle =
    {
        SVertex(3, 3, 0),
        SVertex(3, 4, 0),
        SVertex(4, 3, 0)
    };

    KDop<16> bv1;
    for (unsigned i = 0; i < 3; ++i)
    {
        bv1 += line[i];
    }

    KDop<16> bv2;
    for (unsigned i = 0; i < 3; ++i)
    {
        bv2 += triangle[i];
    }

    EXPECT_TRUE(bv1.overlapped(bv2));
    EXPECT_TRUE(bv2.overlapped(bv1));
}

TEST(KDopTest, testoverlappedDotNeg)
{
    TVertices line =
    {
        SVertex(1, 5, 0)
    };

    TVertices triangle =
    {
        SVertex(3, 3, 0),
        SVertex(3, 4, 0),
        SVertex(4, 3, 0)
    };

    KDop<16> bv1;
    for (unsigned i = 0; i < 3; ++i)
    {
        bv1 += line[i];
    }

    KDop<16> bv2;
    for (unsigned i = 0; i < 3; ++i)
    {
        bv2 += triangle[i];
    }

    EXPECT_FALSE(bv1.overlapped(bv2));
    EXPECT_FALSE(bv2.overlapped(bv1));
}

TEST(KDopTest, testoverlappedDotNeg2)
{
    TVertices line =
    {
        SVertex(3, 1, 0)
    };

    TVertices triangle =
    {
        SVertex(3, 3, 0),
        SVertex(3, 4, 0),
        SVertex(4, 3, 0)
    };

    KDop<16> bv1;
    for (unsigned i = 0; i < 3; ++i)
    {
        bv1 += line[i];
    }

    KDop<16> bv2;
    for (unsigned i = 0; i < 3; ++i)
    {
        bv2 += triangle[i];
    }

    EXPECT_FALSE(bv1.overlapped(bv2));
    EXPECT_FALSE(bv2.overlapped(bv1));
}
