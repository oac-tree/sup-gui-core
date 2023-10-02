/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Rafael Siva (EXT)
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

#include "sup/gui/model/anyvalue_clipboard_utils.h"

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>

#include <QApplication>

using namespace sup::gui;

static const QClipboard::Mode kClipboardMode{QClipboard::Mode::Clipboard};

static const sup::dto::AnyValue kAnyValueEmpty{};
static const sup::dto::AnyValue kAnyValueScalar{sup::dto::SignedInteger32Type, 42};
static const std::vector<sup::dto::AnyValue> kAnyValueScalarList{kAnyValueScalar, kAnyValueScalar};
static const std::vector<sup::dto::AnyValue> kAnyValueScalarListSingle{kAnyValueScalar};

static const QString kInvalidAnyValueJSON{
    R"RAW([{"encoding":"sup-dto/v1.0/JSON"},{"datatype":{"type":"int32"}},{"instance":"abc"}])RAW"};
static const QString kAnyValueScalarJSON{
    R"RAW([{"encoding":"sup-dto/v1.0/JSON"},{"datatype":{"type":"int32"}},{"instance":42}])RAW"};
static const QString kAnyValueScalarListJSON{
    R"RAW([[{"encoding":"sup-dto/v1.0/JSON"},{"datatype":{"type":"int32"}},{"instance":42}],[{"encoding":"sup-dto/v1.0/JSON"},{"datatype":{"type":"int32"}},{"instance":42}]])RAW"};

class AnyValueClipboardUtilsTest : public ::testing::Test
{
public:
  void SetUp() override { m_clipboard = QGuiApplication::clipboard(); };
  static void SetUpTestSuite()
  {
    int argc{};
    char* argv[]{};
    m_application = std::make_unique<QApplication>(argc, argv);
  };
  static void TearDownTestSuite() { m_application.reset(); };

protected:
  static std::unique_ptr<QApplication> m_application;
  QClipboard* m_clipboard{nullptr};
};
std::unique_ptr<QApplication> AnyValueClipboardUtilsTest::m_application{nullptr};

TEST_F(AnyValueClipboardUtilsTest, ClipboardGetAnyvalue)
{
  // No clipboard
  EXPECT_NO_THROW(ClipboardGetAnyvalue(kClipboardMode, nullptr));

  // Valid scalar
  m_clipboard->setText(kAnyValueScalarJSON);
  EXPECT_EQ(ClipboardGetAnyvalue(kClipboardMode, m_clipboard), kAnyValueScalar);

  // Valid list
  m_clipboard->setText(kAnyValueScalarListJSON);
  EXPECT_EQ(ClipboardGetAnyvalue(kClipboardMode, m_clipboard), kAnyValueScalar);

  // Invalid scalar
  m_clipboard->setText(kInvalidAnyValueJSON);
  EXPECT_EQ(ClipboardGetAnyvalue(kClipboardMode, m_clipboard), kAnyValueEmpty);

  // Empty clipboard
  m_clipboard->clear();
  EXPECT_EQ(ClipboardGetAnyvalue(kClipboardMode, m_clipboard), kAnyValueEmpty);

  // Not an anyvalue clipboard
  m_clipboard->setText("abc");
  EXPECT_EQ(ClipboardGetAnyvalue(kClipboardMode, m_clipboard), kAnyValueEmpty);
}

TEST_F(AnyValueClipboardUtilsTest, ClipboardGetAnyvalueList)
{
  // No clipboard
  EXPECT_NO_THROW(ClipboardGetAnyvalueList(kClipboardMode, nullptr));

  // Valid scalar
  m_clipboard->setText(kAnyValueScalarJSON);
  EXPECT_EQ(ClipboardGetAnyvalueList(kClipboardMode, m_clipboard), kAnyValueScalarListSingle);

  // Valid list
  m_clipboard->setText(kAnyValueScalarListJSON);
  EXPECT_EQ(ClipboardGetAnyvalueList(kClipboardMode, m_clipboard), kAnyValueScalarList);

  // Invalid scalar
  m_clipboard->setText(kInvalidAnyValueJSON);
  EXPECT_TRUE(ClipboardGetAnyvalueList(kClipboardMode, m_clipboard).empty());

  // Empty clipboard
  m_clipboard->clear();
  EXPECT_TRUE(ClipboardGetAnyvalueList(kClipboardMode, m_clipboard).empty());

  // Not an anyvalue clipboard
  m_clipboard->setText("abc");
  EXPECT_TRUE(ClipboardGetAnyvalueList(kClipboardMode, m_clipboard).empty());
}

TEST_F(AnyValueClipboardUtilsTest, ClipboardSetAnyvalue)
{
  // No clipboard
  EXPECT_NO_THROW(ClipboardSetAnyvalue(kAnyValueScalar, kClipboardMode, nullptr));

  // Scalar
  EXPECT_NO_THROW(ClipboardSetAnyvalue(kAnyValueScalar, kClipboardMode, m_clipboard));
  EXPECT_EQ(m_clipboard->text(), kAnyValueScalarJSON);
}

TEST_F(AnyValueClipboardUtilsTest, ClipboardSetAnyvalueList)
{
  // No clipboard
  EXPECT_NO_THROW(ClipboardSetAnyvalueList(kAnyValueScalarList, kClipboardMode, nullptr));

  // Single item list
  EXPECT_NO_THROW(ClipboardSetAnyvalueList(kAnyValueScalarListSingle, kClipboardMode, m_clipboard));
  EXPECT_EQ(m_clipboard->text(), kAnyValueScalarJSON);

  // List
  EXPECT_NO_THROW(ClipboardSetAnyvalueList(kAnyValueScalarList, kClipboardMode, m_clipboard));
  EXPECT_EQ(m_clipboard->text(), kAnyValueScalarListJSON);
}
