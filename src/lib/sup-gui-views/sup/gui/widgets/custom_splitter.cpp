/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "custom_splitter.h"

#include <QChildEvent>
#include <QDebug>
#include <QSettings>

namespace sup::gui
{

QString GetMainStateName(const QString &setting_name)
{
  return setting_name + "_state";
}

CustomSplitter::CustomSplitter(const QString &setting_name, QWidget *parent_widget)
    : QSplitter(parent_widget), m_setting_name(setting_name)
{
}

CustomSplitter::~CustomSplitter()
{
  WriteSettings();
}

void CustomSplitter::ReadSettings()
{
  if (m_setting_name.isEmpty())
  {
    return;
  }

  const QSettings settings;

  if (settings.contains(GetMainStateName(m_setting_name)))
  {
    restoreState(settings.value(GetMainStateName(m_setting_name)).toByteArray());
  }
}

void CustomSplitter::childEvent(QChildEvent *event)
{
  if (event->added())
  {
    qDebug() << "added child" << (this) << event->child() << event->child()->parent();
    event->child()->installEventFilter(this);
  }
}

bool CustomSplitter::eventFilter(QObject *obj, QEvent *event)
{
  if (event->type() == QEvent::Show)
  {
    qDebug() << "eventFilter show" << obj << event;
  }
  else if (event->type() == QEvent::Hide)
  {
    qDebug() << "eventFilter hide" << obj << event;
  }

  return false;  // let the parent handle it normally
}

void CustomSplitter::showEvent(QShowEvent *event)
{
  qDebug() << "XXX showEvent " << event << this;
}

void CustomSplitter::hideEvent(QHideEvent *event)
{
  qDebug() << "XXX hideEvent " << event << this;
}

void CustomSplitter::WriteSettings()
{
  if (m_setting_name.isEmpty())
  {
    return;
  }

  QSettings settings;
  settings.setValue(GetMainStateName(m_setting_name), saveState());
}

}  // namespace sup::gui
