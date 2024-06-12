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

#include "sup/gui/components/mime_conversion_helper.h"

#include <mvvm/model/property_item.h>

#include <gtest/gtest.h>

#include <QMimeData>

using namespace sup::gui;

class MimeConverionHelperTests : public ::testing::Test
{
};

TEST_F(MimeConverionHelperTests, CreatePropertyFromMime)
{
  const QString mime_type = "application.coa.tests";
  {
    // wrong mime type
    const QMimeData data;
    auto item = CreateSessionItem(&data, mime_type);
    EXPECT_EQ(item.get(), nullptr);
  }

  {
    const std::string expected_name("abc");
    mvvm::PropertyItem item;
    item.SetDisplayName(expected_name);
    item.SetData(42);

    auto data = CreateCopyMimeData(item, mime_type);
    EXPECT_TRUE(data->hasFormat(mime_type));

    auto reconstructed_item = CreateSessionItem(data.get(), mime_type);
    EXPECT_EQ(reconstructed_item->GetDisplayName(), expected_name);

    auto reconstructed_property = dynamic_cast<mvvm::PropertyItem*>(reconstructed_item.get());
    ASSERT_NE(reconstructed_property, nullptr);
    EXPECT_EQ(reconstructed_property->Data<int>(), 42);
  }
}
