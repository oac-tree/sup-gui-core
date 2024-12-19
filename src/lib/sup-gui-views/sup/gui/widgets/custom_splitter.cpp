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

namespace
{

/**
 * @brief Setting name in QSetting file to store QSplitter main state.
 */
QString GetMainStateSettingName(const QString &setting_name)
{
  return setting_name + "_state";
}

/**
 * @brief Setting name in QSetting file to store hidden flags for all widgets.
 */
QString GetHiddenStateSettingName(const QString &setting_name)
{
  return setting_name + "_hidden_widgets";
}

QStringList GetStringList(const QList<int> &vec)
{
  QStringList result;
  for (auto index : vec)
  {
    result.push_back(QString::number(index));
  }
  return result;
}

QList<int> GetArray(const QStringList &string_list)
{
  QList<int> result;
  for (const auto &str : string_list)
  {
    result.push_back(str.toInt());
  }
  return result;
}

}  // namespace

CustomSplitter::CustomSplitter(const QString &setting_name, QWidget *parent_widget)
    : QSplitter(parent_widget), m_group_name(setting_name)
{
}

CustomSplitter::~CustomSplitter()
{
  WriteSettings();
}

void CustomSplitter::ReadSettings()
{
  if (m_group_name.isEmpty())
  {
    return;
  }

  const QSettings settings;

  if (settings.contains(GetMainStateSettingName(m_group_name)))
  {
    restoreState(settings.value(GetMainStateSettingName(m_group_name)).toByteArray());
  }
  if (settings.contains(GetHiddenStateSettingName(m_group_name)))
  {
    auto string_list = settings.value(GetHiddenStateSettingName(m_group_name)).toStringList();
    m_hidden_widget_index = GetArray(string_list);
    for (auto index : m_hidden_widget_index)
    {
      if (auto child_widget = widget(index); child_widget)
      {
        SetVisible(false, child_widget);
      }
    }
  }
}

void CustomSplitter::ToggleVisibility(QWidget *widget)
{
  qDebug() << "ToggleVisibility";
  SetVisible(!widget->isVisible(), widget);
}

void CustomSplitter::SetVisible(bool value, QWidget *widget)
{
  if (int index = indexOf(widget); index > 0)
  {
    if (value)
    {
      // widget should be visible
      m_hidden_widget_index.removeAll(index);
    }
    else
    {
      // mark widget as hidden
      if (!m_hidden_widget_index.contains(index))
      {
        m_hidden_widget_index.push_back(index);
      }
    }
    widget->setVisible(value);
  }
}

void CustomSplitter::WriteSettings()
{
  if (m_group_name.isEmpty())
  {
    return;
  }

  QSettings settings;
  settings.setValue(GetMainStateSettingName(m_group_name), saveState());
  if (!m_hidden_widget_index.empty())
  {
    settings.setValue(GetHiddenStateSettingName(m_group_name),
                      GetStringList(m_hidden_widget_index));
  }
}

}  // namespace sup::gui
