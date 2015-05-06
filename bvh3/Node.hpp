/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2015
 * @copyright VaL Doroshchuk
 * @license GKU GPL v2
 * @package bvh3
 */

#ifndef BVH3_NODE
#define BVH3_NODE

#include <bvh3/types/SVertex.hpp>
#include <bvh3/splitters/SplitterByCenter.hpp>
#include <vector>
#include <utility>

namespace NBvh3
{

/**
 * Represents Bounding Volume Binary Tree Node.
 *
 * @param TBv Type of bounding volume.
 */
template<class TBv>
class Node
{
public:

    /**
     * Defines matched pair of nodes after quering.
     */
    typedef std::pair<const Node<TBv>*, const Node<TBv>*> TMatchedNodes;

    /**
     * Array of matched pairs.
     */
    typedef std::vector<TMatchedNodes> TCollidedNodes;

    /**
     * @param Computed bounding volume.
     * @param Source vertices.
     * @param Left subtree.
     * @param Right subtree.
     */
    Node(const TBv& bv, const TVertices& vertices, Node* left, Node* right);

    /**
     * @param Computed bounding volume.
     * @param Source vertices.
     */
    Node(const TBv& bv, const TVertices& vertices);
    ~Node();

    /**
     * Checks if current node is leaf.
     */
    bool isLeaf() const;

    /**
     * Returns submitted vertices to current node.
     */
    const TVertices& getVertices() const;

    /**
     * Returns submitted bounding volume.
     */
    const TBv& getBoundingVolume() const;

    /**
     * Returns left subtree.
     */
    const Node* getLeft() const;

    /**
     * Returns right subtree.
     */
    const Node* getRight() const;

    /**
     * Checks if current node overlapped other.
     */
    bool overlapped(const Node<TBv>& other) const;

    /**
     * Checks if current node overlapped other.
     */
    bool overlapped(const Node<TBv>* other) const;

    /**
     * Checks if current and query node collided.
     * Returns matched pairs of nodes.
     * Traverses the tree recursivelly.
     *
     * @param Query node.
     * @param[out] Container to store matched pairs of nodes.
     * @return true If collided.
     */
    bool collided(const Node<TBv>* query, TCollidedNodes& output) const;

private:

    /**
     * Bounding volume.
     */
    TBv mBv;

    /**
     * Original vertices submitted to the node.
     * @todo Avoid copying. Use indices instead?
     */
    TVertices mVertices;

    /**
     * Left subtree.
     */
    Node* mLeft;

    /**
     * Right subtree.
     */
    Node* mRight;
};

template<class TBv>
Node<TBv>::Node(const TBv& bv, const TVertices& vertices, Node* left, Node* right)
    : mBv(bv)
    , mVertices(vertices)
    , mLeft(left)
    , mRight(right)
{
}

template<class TBv>
Node<TBv>::Node(const TBv& bv, const TVertices& vertices)
    : mBv(bv)
    , mVertices(vertices)
    , mLeft(0)
    , mRight(0)
{
}

template<class TBv>
const Node<TBv>* Node<TBv>::getLeft() const
{
    return mLeft;
}

template<class TBv>
const Node<TBv>* Node<TBv>::getRight() const
{
    return mRight;
}

template<class TBv>
Node<TBv>::~Node()
{
    delete mLeft;
    delete mRight;
}

template<class TBv>
bool Node<TBv>::overlapped(const Node<TBv>& other) const
{
    return mBv.overlapped(other.mBv);
}

template<class TBv>
bool Node<TBv>::overlapped(const Node<TBv>* other) const
{
    return other != 0 ? overlapped(*other) : false;
}

template<class TBv>
bool Node<TBv>::isLeaf() const
{
    return mLeft == 0 && mRight == 0;
}

template<class TBv>
const TVertices& Node<TBv>::getVertices() const
{
    return mVertices;
}

template<class TBv>
const TBv& Node<TBv>::getBoundingVolume() const
{
    return mBv;
}

template<class TBv>
bool Node<TBv>::collided(const Node<TBv>* query, TCollidedNodes& output) const
{
    bool result = false;
    if (overlapped(query))
    {
        /// Stores any pairs that overlapped.
        output.push_back(std::make_pair(this, query));

        result = true;
        auto leftQuery = query->getLeft();
        auto rightQuery = query->getRight();
        auto left = getLeft();
        auto right = getRight();

        if (left != 0 && left->overlapped(query))
        {
            output.push_back(std::make_pair(left, query));

            left->collided(leftQuery, output);
            left->collided(rightQuery, output);
        }

        if (right != 0 && right->overlapped(query))
        {
            output.push_back(std::make_pair(right, query));

            right->collided(leftQuery, output);
            right->collided(rightQuery, output);
        }
    }

    return result;
}

/**
 * Creates a binary tree based on bounding volume and splitter.
 *
 * @tparam Bounding volume type.
 * @tparam TSplitter Splits submitted vertices by some logic.
 * @param Original vertices.
 * @return Pointer to Node. Should be freed by user.
 */
template<class TBv, class TSplitter = SplitterByCenter<TBv> >
Node<TBv>* buildTree(const TVertices& vertices)
{
    TBv bv = createBoundingVolume<TBv>(vertices);
    Node<TBv>* result = 0;
    Node<TBv>* nodeLeft = 0;
    Node<TBv>* nodeRight = 0;
    auto size = vertices.size();
    if (size > 1)
    {
        TSplitter splitter(vertices, bv);
        TVertices left;
        TVertices right;
        splitter.split(left, right);

        nodeLeft = buildTree<TBv, TSplitter>(left);
        nodeRight = buildTree<TBv, TSplitter>(right);
    }

    if (size > 0)
    {
        result = new Node<TBv>(bv, vertices, nodeLeft, nodeRight);
    }

    return result;
}

} // namespace NBvh3

#endif // BVH3_NODE