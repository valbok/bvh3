# Bounding Volume Hierarchy Tree

[Введение в дискретно-ориентированные многогранники для задачи определения столкновений.](http://habrahabr.ru/post/257339/)

Simple implementation of BVH-tree using k-DOP as a bounding volume for Collision Detection task.
Bhv3 is a binary tree that allows to search collided object elements effectively.

# k-DOP

Discrete Orientation Polytopes - the bounding volume that provides an ability to reduce cost of collision detection. Is a convex polytope bounded by k hyperplanes with fixed orientations.

Only 16, 18 and 24 planes have been implemented there.

# Build

    $ cmake .; make

Run a test:

    $ ./bvh3/tests/NodeTest
  
# Example

Two different dots should not collide (16 - is a number of planes):

    KDop<16> bv1({3, 1, 0});
    KDop<16> bv2({1, 5, 0});
    EXPECT_FALSE(bv1.overlapped(bv2));

Creating one k-DOP from two:

    KDop<16> bv1({3, 1, 0});
    KDop<16> bv2({1, 5, 0});
    bv1 += bv2;
    
Creating a tree:

    TVertices vertex1 =
    {
        {3, 1, 0}
    };
    
    TVertices vertex2 =
    {
        {3, 3, 0}
    };
    
    auto root1 = buildTree<KDop<16> >(vertex1);
    auto root2 = buildTree<KDop<16> >(vertex2);
    TNodeKDop16::TCollidedNodes output;
    // No collision found
    bool found = root1->collided(root2, output);
    EXPECT_FALSE(found);
    delete root1;
    delete root2;

Unfortunatelly line collided with just a dot:

    TVertices vertices1 =
    {
        {3, 1, 0},
        {1, 5, 0}
    };
    
    TVertices vertices2 =
    {
        {3, 3, 0}
    };
    
    auto root1 = buildTree<KDop<16> >(vertices1);
    auto root2 = buildTree<KDop<16> >(vertices2);
    
    TNodeKDop16::TCollidedNodes output;
    bool found = root1->collided(root2, output);
    EXPECT_TRUE(found);

    delete root1;
    delete root2;
