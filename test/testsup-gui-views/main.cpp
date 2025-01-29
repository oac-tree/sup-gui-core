/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include <sup/gui/components/custom_metatypes.h>
#include <sup/gui/model/register_items.h>

#include <gtest/gtest.h>

#include <QApplication>

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  sup::gui::RegisterCustomMetaTypes();
  sup::gui::RegisterSessionItems();

  QApplication app(argc, argv);
  Q_UNUSED(app)

  // run all google tests
  return RUN_ALL_TESTS();
}
