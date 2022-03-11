/*
  This file is part of the KDAB State Machine Editor Library.

  SPDX-FileCopyrightText: 2014-2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  Author: Kevin Funk <kevin.funk@kdab.com>

  SPDX-License-Identifier: LGPL-2.1-only OR LicenseRef-KDAB-KDStateMachineEditor

  Licensees holding valid commercial KDAB State Machine Editor Library
  licenses may use this file in accordance with the KDAB State Machine Editor
  Library License Agreement provided with the Software.

  Contact info@kdab.com if any conditions of this licensing are not clear to you.
*/

import QtQuick 2.0

import com.kdab.kdsme 1.0

DropArea {
    id: root

    // A list of QMimeData.data's the DropArea accepts.
    keys: ["external"]

    // The UmlElement.element this DropArea is attached to.
    property variant element: null

    onEntered: {
        var sender = drag.source.dragData != undefined ? drag.source.dragData.element : null;
        var target = element
        if (!_quickView.editController.sendDragEnterEvent(sender, element, Qt.point(drag.x, drag.y), drag.urls)) {
            drag.accepted = false
        }
    }

    onDropped: {
        var sender = drop.source.dragData != undefined ? drop.source.dragData.element : null;
        if (!_quickView.editController.sendDropEvent(sender, element, Qt.point(drop.x, drop.y), drop.urls)) {
            drop.accepted = false
        }
    }
}
