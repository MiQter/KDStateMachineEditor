/*
  UmlTransition.qml

  This file is part of the KDAB State Machine Editor Library.

  Copyright (C) 2014 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com.
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

import QtQuick 2.0
import QtQuick.Controls 1.0

import com.kdab.kdsme 1.0

import "qrc:///kdsme/qml/util/"

UmlElement {
    id: root

    property rect labelRect: item.labelBoundingRect
    property var path: item.shape

    width: container.width
    height: container.height

    visible: !painterPathItem.isEmpty
    resizable: true

    mouseAreaMask: PainterPathMask {
        path: root.path
    }

    selectionComponent: Item {
    }

    draggingComponent: DragPointGroup {
        dragKeys: ["TransitionType"]
        dragData: root

        points: [
            DragPoint { x: painterPathItem.startPoint.x;    y: painterPathItem.startPoint.y },
            DragPoint { x: painterPathItem.endPoint.x;      y: painterPathItem.endPoint.y }
        ]

        onChanged: {
            var start = pointAt(0);
            var end = pointAt(1);
            var path = Global.createPath(start);
            path.lineTo(end);

            var cmd = CommandFactory.modifyTransitionLayoutItem(root.item);
            cmd.setShape(path.path);
            commandController.push(cmd);
        }
        onDropped: {
            var transition = root.item.element;
            while (target && target.parent && (target.item == undefined || target.item.element == undefined)) { // find the state containing the drop area
                target = target.parent;
            }
            // If target == null => reparent to root state (the state machine object)
            var state = (target ? target.item.element : root.item.element.machine());
            if (index == 0) {
                if (transition.sourceState != state) {
                    var cmd = CommandFactory.reparentElement(view, transition);
                    cmd.setParentElement(state);
                    commandController.push(cmd)
                }
            } else if (transition.targetState != state) {
                var cmd = CommandFactory.modifyTransition(transition);
                cmd.setTargetState(state);
                commandController.push(cmd)
            }
        }
    }

    PainterPath {
        id: painterPathItem
        path: root.path
    }

    Item {
        id: container

        width: childrenRect.width
        height: childrenRect.height

        Primitive {
            id: edge

            geometry: PainterPathGeometry {
                path: root.path
            }

            color: Qt.tint(Theme.transitionEdgeColor, Theme.alphaTint(Theme.transitionEdgeColor_Active, activeness))

            ArrowHead {
                function angle() {
                     // bind to changes to the start/end point
                    painterPathItem.startPoint
                    painterPathItem.endPoint
                    return -painterPathItem.angleAtPercent(1.0)
                }

                transformOrigin: Item.Right
                x: painterPathItem.endPoint.x-width
                y: painterPathItem.endPoint.y-height/2
                rotation: (!painterPathItem.isEmpty ? angle() : 0.)

                width: 6
                height: 6

                color: parent.color
            }
        }

        Text {
            x: labelRect.x
            y: labelRect.y
            width: labelRect.width
            height: labelRect.height
            color: Theme.transitionLabelFontColor
            font.italic: true
            visible: item.element.sourceState.type != Element.PseudoStateType
                && labelRect.height > 0 && root.name != ""

            text: root.name
        }
    }

}
