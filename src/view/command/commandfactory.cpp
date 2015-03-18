/*
  This file is part of the KDAB State Machine Editor Library.

  Copyright (C) 2014-2015 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com.
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

#include "commandfactory_p.h"

#include "command.h"
#include "createelementcommand.h"
#include "deleteelementcommand.h"
#include "layoutsnapshotcommand.h"
#include "modifyelementcommand.h"
#include "modifypropertycommand.h"
#include "modifytransitioncommand.h"
#include "reparentelementcommand.h"
#include "changestatemachinecommand.h"
#include "state.h"
#include "transition.h"

#include "debug.h"

using namespace KDSME;

CommandFactory::CommandFactory(QObject* parent)
    : QObject(parent)
    , m_model(nullptr)
{
    qRegisterMetaType<CreateElementCommand*>();
    qRegisterMetaType<DeleteElementCommand*>();
    qRegisterMetaType<ModifyElementCommand*>();
    qRegisterMetaType<ModifyPropertyCommand*>();
    qRegisterMetaType<ModifyTransitionCommand*>();
    qRegisterMetaType<ReparentElementCommand*>();
    qRegisterMetaType<ChangeStateMachineCommand*>();
}

StateModel* CommandFactory::model() const
{
    return m_model;
}

void CommandFactory::setModel(StateModel* model)
{
    if (m_model == model)
        return;

    m_model = model;
    emit modelChanged(m_model);
}

CreateElementCommand* CommandFactory::createElement(StateModel* model, Element::Type type, Element* parentElement) const
{
    auto cmd = new CreateElementCommand(model, type);
    cmd->setParentElement(parentElement);
    return cmd;
}

DeleteElementCommand* CommandFactory::deleteElement(KDSME::StateMachineScene* view, KDSME::Element* element) const
{
    return new DeleteElementCommand(view, element);
}

LayoutSnapshotCommand* CommandFactory::layoutSnapshot(KDSME::StateMachineScene* view, const QString& text) const
{
    return new LayoutSnapshotCommand(view, text);
}

ModifyPropertyCommand* CommandFactory::modifyProperty(QObject* object, const char* property, const QVariant& value, const QString& text)
{
    return new ModifyPropertyCommand(object, property, value, text);
}

ModifyPropertyCommand* CommandFactory::modifyProperty(QObject* object, const QJsonObject& propertyMap, const QString& text)
{
    return new ModifyPropertyCommand(object, propertyMap, text);
}

ModifyElementCommand* CommandFactory::modifyElement(Element* item)
{
    return new ModifyElementCommand(item);
}

KDSME::ModifyTransitionCommand* CommandFactory::modifyTransition(Transition* transition)
{
    return new ModifyTransitionCommand(transition);
}

ReparentElementCommand* CommandFactory::reparentElement(StateMachineScene* view, Element* element)
{
    return new ReparentElementCommand(view, element);
}

ChangeStateMachineCommand* CommandFactory::changeStateMachineElement(KDSME::StateMachineScene* view, KDSME::StateMachine* statemachine)
{
    ChangeStateMachineCommand *cmd = new ChangeStateMachineCommand(view);
    cmd->setStateMachine(statemachine);
    return cmd;
}
