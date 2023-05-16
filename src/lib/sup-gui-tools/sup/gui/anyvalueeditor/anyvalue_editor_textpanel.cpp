/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "anyvalue_editor_textpanel.h"

#include "highlighter/qsourcehighliter.h"

#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_utils.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/project/model_has_changed_controller.h>

#include <sup/dto/anyvalue.h>

#include <definition.h>
#include <syntaxhighlighter.h>
#include <theme.h>

#include <QDebug>
#include <QScrollBar>
#include <QTextEdit>
#include <QVBoxLayout>

namespace sup::gui
{

AnyValueEditorTextPanel::AnyValueEditorTextPanel(mvvm::ApplicationModel *model, QWidget *parent)
    : QWidget(parent), m_text_edit(new QTextEdit), m_model(model)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_text_edit);

  QFont textFont("Monospace");
  m_text_edit->setFont(textFont);
  m_text_edit->setLineWrapMode(QTextEdit::NoWrap);

  //  auto highlighter = new QSourceHighlite::QSourceHighliter(m_text_edit->document());
  //  highlighter->setCurrentLanguage(QSourceHighlite::QSourceHighliter::CodeJSON);

  auto highlighter = new KSyntaxHighlighting::SyntaxHighlighter(m_text_edit);

  highlighter->setTheme(
      (m_text_edit->palette().color(QPalette::Base).lightness() < 128)
          ? m_repository.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
          : m_repository.defaultTheme(KSyntaxHighlighting::Repository::LightTheme));

  const auto def = m_repository.definitionForFileName("aaa.json");
  highlighter->setDefinition(def);

  auto on_model_changed = [this]() { UpdateJson(); };
  m_model_changed_controller =
      std::make_unique<mvvm::ModelHasChangedController>(m_model, on_model_changed);
}

AnyValueEditorTextPanel::~AnyValueEditorTextPanel() = default;

void AnyValueEditorTextPanel::UpdateJson()
{
  if (auto item = mvvm::utils::GetTopItem<sup::gui::AnyValueItem>(m_model); item)
  {
    SaveScrollBarPosition();

    try
    {
      auto any_value = sup::gui::CreateAnyValue(*item);
      auto str = sup::gui::AnyValueToJSONString(any_value, true);
      m_text_edit->setText(QString::fromStdString(str));
      RestoreScrollBarPosition();
    }
    catch (const std::exception &ex)
    {
      // Current simplified approach calls the method `UpdateJson` on every
      // model change. If model is inconsistent, CreateAnyValue method will fail.
      m_text_edit->clear();
    }
  }
  else
  {
    m_text_edit->clear();
  }
}

void AnyValueEditorTextPanel::SaveScrollBarPosition()
{
  const int current_scrollbar_value = m_text_edit->verticalScrollBar()->value();
  if (current_scrollbar_value > 0)
  {
    // We save scroll bar position only if current position is not zero.
    // This is a simple way to ignore moments, when text editor was cleared because of
    // inconsistency in the model.
    m_cached_scrollbar_value = current_scrollbar_value;
  }
}

void AnyValueEditorTextPanel::RestoreScrollBarPosition()
{
  m_text_edit->verticalScrollBar()->setValue(m_cached_scrollbar_value);
}

}  // namespace sup::gui
