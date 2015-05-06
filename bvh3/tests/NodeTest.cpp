/**
 * @author VaL Doroshchuk
 * @license GNU GPL v2
 */

#include <bvh3/bv/all.hpp>
#include <bvh3/Node.hpp>
#include <gtest/gtest.h>

using namespace NBvh3;
using namespace std;

typedef KDop<16> TKDop16;
typedef Node<TKDop16> TNodeKDop16;

std::ostream& operator << (std::ostream& os, const SVertex& obj)
{
    return os << "(" << obj.x << ", " << obj.y << ", " << obj.z << ")";
}

TEST(NodeTest, testBuildTree)
{
    TVertices triangle =
    {
        {3, 1, 0},
        {1, 5, 0},
        {5, 4, 0}
    };

    auto root = buildTree<TKDop16>(triangle);
    EXPECT_EQ(1, root->getBoundingVolume().getMin(0));
    EXPECT_EQ(5, root->getBoundingVolume().getMax(0));
    EXPECT_EQ(1, root->getBoundingVolume().getMin(1));
    EXPECT_EQ(5, root->getBoundingVolume().getMax(1));
    EXPECT_EQ(3, root->getVertices().size());
    EXPECT_EQ(SVertex(3, 1, 0), root->getVertices()[0]);
    EXPECT_EQ(SVertex(1, 5, 0), root->getVertices()[1]);
    EXPECT_EQ(SVertex(5, 4, 0), root->getVertices()[2]);

    EXPECT_FALSE(root->isLeaf());
    auto left = root->getLeft();
    auto right = root->getRight();

    EXPECT_TRUE(left != 0);
    EXPECT_FALSE(left->isLeaf());
    EXPECT_EQ(2, left->getVertices().size());
    EXPECT_EQ(SVertex(3, 1, 0), left->getVertices()[0]);
    EXPECT_EQ(SVertex(1, 5, 0), left->getVertices()[1]);
    EXPECT_EQ(1, left->getBoundingVolume().getMin(0));
    EXPECT_EQ(3, left->getBoundingVolume().getMax(0));
    EXPECT_EQ(1, left->getBoundingVolume().getMin(1));
    EXPECT_EQ(5, left->getBoundingVolume().getMax(1));

    EXPECT_TRUE(right != 0);
    EXPECT_TRUE(right->isLeaf());
    EXPECT_EQ(1, right->getVertices().size());
    EXPECT_EQ(SVertex(5, 4, 0), right->getVertices()[0]);
    EXPECT_EQ(5, right->getBoundingVolume().getMin(0));
    EXPECT_EQ(5, right->getBoundingVolume().getMax(0));
    EXPECT_EQ(4, right->getBoundingVolume().getMin(1));
    EXPECT_EQ(4, right->getBoundingVolume().getMax(1));

    // Go left
    auto left2 = left->getLeft();
    auto right2 = left->getRight();
    EXPECT_TRUE(left2 != 0);
    EXPECT_TRUE(left2->isLeaf());
    EXPECT_EQ(1, left2->getVertices().size());
    EXPECT_EQ(SVertex(3, 1, 0), left2->getVertices()[0]);
    EXPECT_EQ(3, left2->getBoundingVolume().getMin(0));
    EXPECT_EQ(3, left2->getBoundingVolume().getMax(0));
    EXPECT_EQ(1, left2->getBoundingVolume().getMin(1));
    EXPECT_EQ(1, left2->getBoundingVolume().getMax(1));

    EXPECT_TRUE(right2 != 0);
    EXPECT_TRUE(right2->isLeaf());
    EXPECT_EQ(1, right2->getVertices().size());
    EXPECT_EQ(SVertex(1, 5, 0), right2->getVertices()[0]);
    EXPECT_EQ(1, right2->getBoundingVolume().getMin(0));
    EXPECT_EQ(1, right2->getBoundingVolume().getMax(0));
    EXPECT_EQ(5, right2->getBoundingVolume().getMin(1));
    EXPECT_EQ(5, right2->getBoundingVolume().getMax(1));

    EXPECT_EQ(0, left2->getLeft());
    EXPECT_EQ(0, left2->getRight());
    EXPECT_EQ(0, right2->getLeft());
    EXPECT_EQ(0, right2->getRight());

    // Go right
    EXPECT_EQ(0, right->getLeft());
    EXPECT_EQ(0, right->getRight());

    delete root;
}

TEST(NodeTest, testBuildTreeoverlapped)
{
    TVertices triangle1 =
    {
        {3, 1, 0},
        {1, 5, 0},
        {5, 4, 0}
    };

    TVertices triangle2 =
    {
        {3, 1, 0},
        {4, 2, 0},
        {6, 1, 0}
    };

    auto root1 = buildTree<TKDop16>(triangle1);
    auto root2 = buildTree<TKDop16>(triangle2);

    EXPECT_TRUE(root1->overlapped(*root2));
    EXPECT_TRUE(root2->overlapped(*root1));

    delete root1;
    delete root2;
}

TEST(NodeTest, testBuildTreeoverlappedNeg)
{
    TVertices triangle1 =
    {
        {3, 1, 0},
        {1, 5, 0},
        {5, 4, 0}
    };

    TVertices triangle2 =
    {
        {4, 1, 0},
        {5, 2, 0},
        {6, 1, 0}
    };

    auto root1 = buildTree<TKDop16>(triangle1);
    auto root2 = buildTree<TKDop16>(triangle2);

    EXPECT_FALSE(root1->overlapped(*root2));
    EXPECT_FALSE(root2->overlapped(*root1));

    delete root1;
    delete root2;
}

TEST(NodeTest, testBuildTreeoverlappedInside)
{
    TVertices triangle1 =
    {
        {3, 1, 0},
        {1, 5, 0},
        {5, 4, 0}
    };

    TVertices triangle2 =
    {
        {3, 3, 0},
        {3, 4, 0},
        {4, 3, 0}
    };

    auto root1 = buildTree<TKDop16>(triangle1);
    auto root2 = buildTree<TKDop16>(triangle2);

    EXPECT_TRUE(root1->overlapped(*root2));
    EXPECT_TRUE(root2->overlapped(*root1));

    delete root1;
    delete root2;
}

TEST(NodeTest, testCollidedDotsNeg)
{
    TVertices vertex1 =
    {
        {3, 1, 0}
    };

    TVertices vertex2 =
    {
        {3, 3, 0}
    };

    auto root1 = buildTree<TKDop16>(vertex1);
    auto root2 = buildTree<TKDop16>(vertex2);
    TNodeKDop16::TFoundNodes output;
    bool found = root1->collided(root2, output);

    EXPECT_FALSE(found);
    EXPECT_EQ(0, output.size());

    delete root1;
    delete root2;
}

TEST(NodeTest, testCollidedDots)
{
    TVertices vertex1 =
    {
        {3, 1, 0}
    };

    auto root1 = buildTree<TKDop16>(vertex1);
    TNodeKDop16::TFoundNodes output;
    bool found = root1->collided(root1, output);

    EXPECT_TRUE(found);
    EXPECT_EQ(1, output.size());

    delete root1;
}

TEST(NodeTest, testCollidedLineVsDot)
{
    TVertices vertices1 =
    {
        {3, 1, 0},
        {1, 5, 0},
    };

    TVertices vertices2 =
    {
        {3, 3, 0}
    };

    auto root1 = buildTree<TKDop16>(vertices1);
    auto root2 = buildTree<TKDop16>(vertices2);

    {
        TNodeKDop16::TFoundNodes output;
        bool found = root1->collided(root2, output);

        EXPECT_TRUE(found);
        EXPECT_EQ(1, output.size());
        EXPECT_EQ(2, output[0].first->getVertices().size());
        EXPECT_EQ(SVertex(3, 1, 0), output[0].first->getVertices()[0]);
        EXPECT_EQ(SVertex(1, 5, 0), output[0].first->getVertices()[1]);
        EXPECT_EQ(1, output[0].second->getVertices().size());
        EXPECT_EQ(SVertex(3, 3, 0), output[0].second->getVertices()[0]);
    }

    {
        TNodeKDop16::TFoundNodes output;
        bool found = root2->collided(root1, output);

        EXPECT_TRUE(found);
        EXPECT_EQ(1, output.size());
        EXPECT_EQ(1, output[0].first->getVertices().size());
        EXPECT_EQ(SVertex(3, 3, 0), output[0].first->getVertices()[0]);

        EXPECT_EQ(2, output[0].second->getVertices().size());
        EXPECT_EQ(SVertex(3, 1, 0), output[0].second->getVertices()[0]);
        EXPECT_EQ(SVertex(1, 5, 0), output[0].second->getVertices()[1]);
    }
}

TEST(NodeTest, testCollidedTriangleVsDot)
{
    TVertices vertices1 =
    {
        {3, 1, 0},
        {1, 5, 0},
        {5, 4, 0}
    };

    TVertices vertices2 =
    {
        {4, 5, 0}
    };

    auto root1 = buildTree<TKDop16>(vertices1);
    auto root2 = buildTree<TKDop16>(vertices2);

    TNodeKDop16::TFoundNodes output;
    bool found = root1->collided(root2, output);

    EXPECT_TRUE(found);
    EXPECT_EQ(1, output.size());
    EXPECT_EQ(3, output[0].first->getVertices().size());
    EXPECT_EQ(SVertex(3, 1, 0), output[0].first->getVertices()[0]);
    EXPECT_EQ(SVertex(1, 5, 0), output[0].first->getVertices()[1]);
    EXPECT_EQ(SVertex(5, 4, 0), output[0].first->getVertices()[2]);
    EXPECT_EQ(1, output[0].second->getVertices().size());
    EXPECT_EQ(SVertex(4, 5, 0), output[0].second->getVertices()[0]);
}

TEST(NodeTest, testcollidedTriangles)
{

    TVertices triangle1 =
    {
        {3, 1, 0},
        {1, 5, 0},
        {5, 4, 0}
    };

    TVertices triangle2 =
    {
        {3, 1, 0},
        {4, 2, 0},
        {6, 1, 0}
    };

    auto root1 = buildTree<TKDop16>(triangle1);
    auto root2 = buildTree<TKDop16>(triangle2);

    TNodeKDop16::TFoundNodes output;
    bool found = root1->collided(root2, output);

    EXPECT_TRUE(found);
    EXPECT_EQ(5, output.size());

    EXPECT_EQ(3, output[0].first->getVertices().size());
    EXPECT_EQ(SVertex(3, 1, 0), output[0].first->getVertices()[0]);
    EXPECT_EQ(SVertex(1, 5, 0), output[0].first->getVertices()[1]);
    EXPECT_EQ(SVertex(5, 4, 0), output[0].first->getVertices()[2]);
    EXPECT_EQ(3, output[0].second->getVertices().size());
    EXPECT_EQ(SVertex(3, 1, 0), output[0].second->getVertices()[0]);
    EXPECT_EQ(SVertex(4, 2, 0), output[0].second->getVertices()[1]);
    EXPECT_EQ(SVertex(6, 1, 0), output[0].second->getVertices()[2]);

    EXPECT_EQ(2, output[1].first->getVertices().size());
    EXPECT_EQ(SVertex(3, 1, 0), output[1].first->getVertices()[0]);
    EXPECT_EQ(SVertex(1, 5, 0), output[1].first->getVertices()[1]);
    EXPECT_EQ(3, output[1].second->getVertices().size());
    EXPECT_EQ(SVertex(3, 1, 0), output[1].second->getVertices()[0]);
    EXPECT_EQ(SVertex(4, 2, 0), output[1].second->getVertices()[1]);
    EXPECT_EQ(SVertex(6, 1, 0), output[1].second->getVertices()[2]);

    EXPECT_EQ(2, output[2].first->getVertices().size());
    EXPECT_EQ(SVertex(3, 1, 0), output[2].first->getVertices()[0]);
    EXPECT_EQ(SVertex(1, 5, 0), output[2].first->getVertices()[1]);
    EXPECT_EQ(2, output[2].second->getVertices().size());
    EXPECT_EQ(SVertex(3, 1, 0), output[2].second->getVertices()[0]);
    EXPECT_EQ(SVertex(4, 2, 0), output[2].second->getVertices()[1]);

    EXPECT_EQ(1, output[3].first->getVertices().size());
    EXPECT_EQ(SVertex(3, 1, 0), output[3].first->getVertices()[0]);
    EXPECT_EQ(2, output[3].second->getVertices().size());
    EXPECT_EQ(SVertex(3, 1, 0), output[3].second->getVertices()[0]);
    EXPECT_EQ(SVertex(4, 2, 0), output[3].second->getVertices()[1]);

    EXPECT_EQ(1, output[4].first->getVertices().size());
    EXPECT_EQ(SVertex(3, 1, 0), output[4].first->getVertices()[0]);
    EXPECT_EQ(1, output[4].second->getVertices().size());
    EXPECT_EQ(SVertex(3, 1, 0), output[4].second->getVertices()[0]);

    delete root1;
    delete root2;
}

TEST(NodeTest, testCollidedTriangles)
{
    TVertices triangle1 =
    {
        {3, 1, 0},
        {1, 5, 0},
        {5, 4, 0}
    };

    TVertices triangle2 =
    {
        {4, 1, 0},
        {4, 2, 0},
        {6, 1, 0}
    };

    auto root1 = buildTree<TKDop16>(triangle1);
    auto root2 = buildTree<TKDop16>(triangle2);

    {
        TNodeKDop16::TFoundNodes output;
        bool found = root1->collided(root2, output);

        EXPECT_TRUE(found);
        EXPECT_EQ(1, output.size());

        EXPECT_EQ(3, output[0].first->getVertices().size());
        EXPECT_EQ(SVertex(3, 1, 0), output[0].first->getVertices()[0]);
        EXPECT_EQ(SVertex(1, 5, 0), output[0].first->getVertices()[1]);
        EXPECT_EQ(SVertex(5, 4, 0), output[0].first->getVertices()[2]);

        EXPECT_EQ(3, output[0].second->getVertices().size());
        EXPECT_EQ(SVertex(4, 1, 0), output[0].second->getVertices()[0]);
        EXPECT_EQ(SVertex(4, 2, 0), output[0].second->getVertices()[1]);
        EXPECT_EQ(SVertex(6, 1, 0), output[0].second->getVertices()[2]);
    }

    {
        TNodeKDop16::TFoundNodes output;
        bool found = root2->collided(root1, output);

        EXPECT_TRUE(found);
        EXPECT_EQ(2, output.size());

        EXPECT_EQ(2, output[1].first->getVertices().size());
        EXPECT_EQ(SVertex(4, 1, 0), output[1].first->getVertices()[0]);
        EXPECT_EQ(SVertex(4, 2, 0), output[1].first->getVertices()[1]);

        EXPECT_EQ(3, output[1].second->getVertices().size());
        EXPECT_EQ(SVertex(3, 1, 0), output[1].second->getVertices()[0]);
        EXPECT_EQ(SVertex(1, 5, 0), output[1].second->getVertices()[1]);
        EXPECT_EQ(SVertex(5, 4, 0), output[1].second->getVertices()[2]);
    }

    delete root1;
    delete root2;
}

TEST(NodeTest, testBuildTreeNeg2)
{
    TVertices triangle1 =
    {
        {3, 1, 0},
        {1, 5, 0},
        {5, 4, 0}
    };

    TVertices triangle2 =
    {
        {2, 1, 0},
        {0, 2, 0},
        {0, 1, 0}
    };

    auto root1 = buildTree<TKDop16>(triangle1);
    auto root2 = buildTree<TKDop16>(triangle2);

    TNodeKDop16::TFoundNodes output;
    bool found = root1->collided(root2, output);

    EXPECT_FALSE(found);
    EXPECT_EQ(0, output.size());

    delete root1;
    delete root2;
}


TEST(NodeTest, testBuildTreeInside)
{
    TVertices triangle1 =
    {
        {3, 1, 0},
        {1, 5, 0},
        {5, 4, 0}
    };

    TVertices triangle2 =
    {
        {3, 3, 0},
        {3, 4, 0},
        {4, 3, 0}
    };

    auto root1 = buildTree<TKDop16>(triangle1);
    auto root2 = buildTree<TKDop16>(triangle2);

    {
        TNodeKDop16::TFoundNodes output;
        bool found = root1->collided(root2, output);

        EXPECT_TRUE(found);
        EXPECT_EQ(3, output.size());

        EXPECT_EQ(2, output[1].first->getVertices().size());
        EXPECT_EQ(SVertex(3, 1, 0), output[1].first->getVertices()[0]);
        EXPECT_EQ(SVertex(1, 5, 0), output[1].first->getVertices()[1]);
        EXPECT_EQ(3, output[1].second->getVertices().size());
        EXPECT_EQ(SVertex(3, 3, 0), output[1].second->getVertices()[0]);
        EXPECT_EQ(SVertex(3, 4, 0), output[1].second->getVertices()[1]);
        EXPECT_EQ(SVertex(4, 3, 0), output[1].second->getVertices()[2]);

        EXPECT_EQ(2, output[2].first->getVertices().size());
        EXPECT_EQ(SVertex(3, 1, 0), output[2].first->getVertices()[0]);
        EXPECT_EQ(SVertex(1, 5, 0), output[2].first->getVertices()[1]);
        EXPECT_EQ(2, output[2].second->getVertices().size());
        EXPECT_EQ(SVertex(3, 3, 0), output[2].second->getVertices()[0]);
        EXPECT_EQ(SVertex(3, 4, 0), output[2].second->getVertices()[1]);
    }

    {
        TNodeKDop16::TFoundNodes output;
        bool found = root2->collided(root1, output);

        EXPECT_TRUE(found);
        EXPECT_EQ(5, output.size());

        EXPECT_EQ(2, output[1].first->getVertices().size());
        EXPECT_EQ(SVertex(3, 3, 0), output[1].first->getVertices()[0]);
        EXPECT_EQ(SVertex(3, 4, 0), output[1].first->getVertices()[1]);

        EXPECT_EQ(3, output[1].second->getVertices().size());
        EXPECT_EQ(SVertex(3, 1, 0), output[1].second->getVertices()[0]);
        EXPECT_EQ(SVertex(1, 5, 0), output[1].second->getVertices()[1]);
        EXPECT_EQ(SVertex(5, 4, 0), output[1].second->getVertices()[2]);

        EXPECT_EQ(2, output[2].first->getVertices().size());
        EXPECT_EQ(SVertex(3, 3, 0), output[2].first->getVertices()[0]);
        EXPECT_EQ(SVertex(3, 4, 0), output[2].first->getVertices()[1]);

        EXPECT_EQ(2, output[2].second->getVertices().size());
        EXPECT_EQ(SVertex(3, 1, 0), output[2].second->getVertices()[0]);
        EXPECT_EQ(SVertex(1, 5, 0), output[2].second->getVertices()[1]);

        EXPECT_EQ(1, output[3].first->getVertices().size());
        EXPECT_EQ(SVertex(3, 3, 0), output[3].first->getVertices()[0]);

        EXPECT_EQ(2, output[3].second->getVertices().size());
        EXPECT_EQ(SVertex(3, 1, 0), output[3].second->getVertices()[0]);
        EXPECT_EQ(SVertex(1, 5, 0), output[3].second->getVertices()[1]);

        EXPECT_EQ(1, output[4].first->getVertices().size());
        EXPECT_EQ(SVertex(4, 3, 0), output[4].first->getVertices()[0]);

        EXPECT_EQ(3, output[4].second->getVertices().size());
        EXPECT_EQ(SVertex(3, 1, 0), output[4].second->getVertices()[0]);
        EXPECT_EQ(SVertex(1, 5, 0), output[4].second->getVertices()[1]);
        EXPECT_EQ(SVertex(5, 4, 0), output[4].second->getVertices()[2]);
    }

    delete root1;
    delete root2;
}

TEST(NodeTest, testCollidedTrianglesLeft)
{
    TVertices triangle1 =
    {
        {3, 1, 0},
        {1, 5, 0},
        {5, 4, 0}
    };

    TVertices triangle2 =
    {
        {0, 3, 0},
        {0, 4, 0},
        {1, 3, 0}
    };

    auto root1 = buildTree<TKDop16>(triangle1);
    auto root2 = buildTree<TKDop16>(triangle2);

    {
        TNodeKDop16::TFoundNodes output;
        bool found = root1->collided(root2, output);

        EXPECT_TRUE(found);
        EXPECT_EQ(3, output.size());

        EXPECT_EQ(2, output[1].first->getVertices().size());
        EXPECT_EQ(SVertex(3, 1, 0), output[1].first->getVertices()[0]);
        EXPECT_EQ(SVertex(1, 5, 0), output[1].first->getVertices()[1]);

        EXPECT_EQ(3, output[1].second->getVertices().size());
        EXPECT_EQ(SVertex(0, 3, 0), output[1].second->getVertices()[0]);
        EXPECT_EQ(SVertex(0, 4, 0), output[1].second->getVertices()[1]);
        EXPECT_EQ(SVertex(1, 3, 0), output[1].second->getVertices()[2]);

        EXPECT_EQ(2, output[2].first->getVertices().size());
        EXPECT_EQ(SVertex(3, 1, 0), output[2].first->getVertices()[0]);
        EXPECT_EQ(SVertex(1, 5, 0), output[2].first->getVertices()[1]);

        EXPECT_EQ(1, output[2].second->getVertices().size());
        EXPECT_EQ(SVertex(1, 3, 0), output[2].second->getVertices()[0]);
    }

    {
        TNodeKDop16::TFoundNodes output;
        bool found = root2->collided(root1, output);

        EXPECT_TRUE(found);
        EXPECT_EQ(3, output.size());

        EXPECT_EQ(1, output[1].first->getVertices().size());
        EXPECT_EQ(SVertex(1, 3, 0), output[1].first->getVertices()[0]);

        EXPECT_EQ(3, output[1].second->getVertices().size());
        EXPECT_EQ(SVertex(3, 1, 0), output[1].second->getVertices()[0]);
        EXPECT_EQ(SVertex(1, 5, 0), output[1].second->getVertices()[1]);
        EXPECT_EQ(SVertex(5, 4, 0), output[1].second->getVertices()[2]);

        EXPECT_EQ(1, output[2].first->getVertices().size());
        EXPECT_EQ(SVertex(1, 3, 0), output[2].first->getVertices()[0]);

        EXPECT_EQ(2, output[2].second->getVertices().size());
        EXPECT_EQ(SVertex(3, 1, 0), output[2].second->getVertices()[0]);
        EXPECT_EQ(SVertex(1, 5, 0), output[2].second->getVertices()[1]);
    }

    delete root1;
    delete root2;
}
