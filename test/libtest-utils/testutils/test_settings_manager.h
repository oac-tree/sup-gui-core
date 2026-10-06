/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef TESTUTILS_TEST_SETTINGS_MANAGER_H_
#define TESTUTILS_TEST_SETTINGS_MANAGER_H_

#include <sup/gui/settings/in_memory_settings_storage.h>
#include <sup/gui/settings/settings_manager.h>

namespace sup::gui::test
{

/**
 * @brief The TestSettingsManager class is a settings manager with in-memory application storage.
 */
class TestSettingsManager : public SettingsManager
{
public:
  TestSettingsManager();

private:
  InMemorySettingsStorage m_storage;
};

}  // namespace sup::gui::test

#endif  // TESTUTILS_TEST_SETTINGS_MANAGER_H_
