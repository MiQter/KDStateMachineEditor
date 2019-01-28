/*
  treewalker.h

  This file is part of the KDAB State Machine Editor Library.

  Copyright (C) 2014-2019 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com.
  All rights reserved.
  Author: Kevin Funk <kevin.funk@kdab.com>

  Licensees holding valid commercial KDAB State Machine Editor Library
  licenses may use this file in accordance with the KDAB State Machine Editor
  Library License Agreement provided with the Software.

  This file may be distributed and/or modified under the terms of the
  GNU Lesser General Public License version 2.1 as published by the
  Free Software Foundation and appearing in the file LICENSE.LGPL.txt included.

  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

  Contact info@kdab.com if any conditions of this licensing are not
  clear to you.
*/

#ifndef KDSME_UTIL_TREEWALKER_H
#define KDSME_UTIL_TREEWALKER_H

#include "kdsme_core_export.h"

#include <QObject>

#include <functional>

namespace KDSME {

template<typename T>
struct TreeWalkerTrait
{
    static QList<T> children(T item) { Q_UNUSED(item); return QList<T>(); }
};

/**
 * Performs a DFS walk through the hierarchy of T
 *
 * @note Specialize TreeWalkerTrait for your type T to get TreeWalker support
 */
template<typename T>
class TreeWalker
{
public:
    enum VisitResult {
        ContinueWalk, ///< Continues traversal with the next sibling of the item just visited, without visiting its children
        RecursiveWalk, ///< Traverse the children of this item
        StopWalk ///< Terminate the traversal
    };

    enum TraversalType {
        PreOrderTraversal,
        PostOrderTraversal,
    };

    typedef std::function<VisitResult(T)> VisitFunction;

    explicit TreeWalker(TraversalType type = PreOrderTraversal)
        : m_traversalType(type) {}

    /**
     * Walk through all items including the start item itself
     *
     * @return True in case we walked through all items, false otherwise
     *
     * @param visit Function called each time a LayoutItem instance is encountered
     */
    bool walkItems(T item, const VisitFunction& visit)
    {
        if (!item)
            return false;

        bool continueWalk = true;
        if (m_traversalType == PreOrderTraversal) {
            continueWalk = (visit(item) == TreeWalker::RecursiveWalk);
        }
        Q_ASSERT(item);
        foreach (T child, TreeWalkerTrait<T>::children(item)) {
            if (!walkItems(child, visit))
                return false;
        }
        if (m_traversalType == PostOrderTraversal) {
            continueWalk = (visit(item) == TreeWalker::RecursiveWalk);
        }
        return continueWalk;
    }

    /**
     * Convenience function. Same as walk(), but omits item @p item
     *
     * @sa walk()
     */
    bool walkChildren(T item, const VisitFunction& visit)
    {
        if (!item)
            return false;

        foreach (T child, TreeWalkerTrait<T>::children(item)) {
            if (!walkItems(child, visit))
                return false;
        }
        return true;
    }

private:
    TraversalType m_traversalType;
};

}

#endif // TREEWALKER_H
