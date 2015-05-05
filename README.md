# Bounding Volume Hierarchy Tree

[Введение в дискретно-ориентированные многогранники для задачи определения столкновений.](http://habrahabr.ru/post/257339/)

Simple implementation of BVH-tree using k-DOP as a bounding volume for Collision Detection task.

# k-DOP

Discrete Orientation Polytopes - the bounding volume that provides an ability to reduce cost of collision detection.
Is a convex polytope bounded by k hyperplanes with fixed orientations.

Only 16, 18 and 24 planes have been implemented there.

# Build

    $ cmake .; make

Run a test:

    $ ./bvh3/tests/NodeTest
  
# Example

