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

#ifndef SUP_GUI_ANYVALUEEDITOR_HIGHLIGHTER_LANGUAGEDATA_H_
#define SUP_GUI_ANYVALUEEDITOR_HIGHLIGHTER_LANGUAGEDATA_H_

template <typename key, typename val>
class QMultiHash;

class QLatin1String;

namespace QSourceHighlite
{

using LanguageData = QMultiHash<char, QLatin1String>;

/**********************************************************/
/* LuaData ************************************************/
/**********************************************************/
void loadLuaData(LanguageData &typess, LanguageData &keywordss, LanguageData &builtins,
                 LanguageData &literalss, LanguageData &others);

/**********************************************************/
/* C/C++ Data *********************************************/
/**********************************************************/
void loadCppData(LanguageData &typess, LanguageData &keywordss, LanguageData &builtins,
                 LanguageData &literalss, LanguageData &others);

/**********************************************************/
/* Shell Data *********************************************/
/**********************************************************/
void loadShellData(LanguageData &types, LanguageData &keywords, LanguageData &builtin,
                   LanguageData &literals, LanguageData &other);

/**********************************************************/
/* JS Data *********************************************/
/**********************************************************/
void loadJSData(LanguageData &types, LanguageData &keywords, LanguageData &builtin,
                LanguageData &literals, LanguageData &other);

/**********************************************************/
/* PHP Data *********************************************/
/**********************************************************/
void loadPHPData(LanguageData &types, LanguageData &keywords, LanguageData &builtin,
                 LanguageData &literals, LanguageData &other);

/**********************************************************/
/* QML Data *********************************************/
/**********************************************************/
void loadQMLData(LanguageData &types, LanguageData &keywords, LanguageData &builtin,
                 LanguageData &literals, LanguageData &other);

/**********************************************************/
/* Python Data *********************************************/
/**********************************************************/
void loadPythonData(LanguageData &types, LanguageData &keywords, LanguageData &builtin,
                    LanguageData &literals, LanguageData &other);

/********************************************************/
/***   Rust DATA      ***********************************/
/********************************************************/
void loadRustData(LanguageData &types, LanguageData &keywords, LanguageData &builtin,
                  LanguageData &literals, LanguageData &other);

/********************************************************/
/***   Java DATA      ***********************************/
/********************************************************/
void loadJavaData(LanguageData &types, LanguageData &keywords, LanguageData &builtin,
                  LanguageData &literals, LanguageData &other);

/********************************************************/
/***   C# DATA      *************************************/
/********************************************************/
void loadCSharpData(LanguageData &types, LanguageData &keywords, LanguageData &builtin,
                    LanguageData &literals, LanguageData &other);

/********************************************************/
/***   Go DATA      *************************************/
/********************************************************/
void loadGoData(LanguageData &types, LanguageData &keywords, LanguageData &builtin,
                LanguageData &literals, LanguageData &other);

/********************************************************/
/***   V DATA      **************************************/
/********************************************************/
void loadVData(LanguageData &types, LanguageData &keywords, LanguageData &builtin,
               LanguageData &literals, LanguageData &other);

/********************************************************/
/***   SQL DATA      ************************************/
/********************************************************/
void loadSQLData(LanguageData &types, LanguageData &keywords, LanguageData &builtin,
                 LanguageData &literals, LanguageData &other);

/********************************************************/
/***   JSON DATA      ***********************************/
/********************************************************/
void loadJSONData(LanguageData &types, LanguageData &keywords, LanguageData &builtin,
                  LanguageData &literals, LanguageData &other);

/********************************************************/
/***   CSS DATA      ***********************************/
/********************************************************/
void loadCSSData(LanguageData &types, LanguageData &keywords, LanguageData &builtin,
                 LanguageData &literals, LanguageData &other);

/********************************************************/
/***   Typescript DATA  *********************************/
/********************************************************/
void loadTypescriptData(LanguageData &types, LanguageData &keywords, LanguageData &builtin,
                        LanguageData &literals, LanguageData &other);

/********************************************************/
/***   YAML DATA  ***************************************/
/********************************************************/
void loadYAMLData(LanguageData &types, LanguageData &keywords, LanguageData &builtin,
                  LanguageData &literals, LanguageData &other);

/********************************************************/
/***   VEX DATA   ***************************************/
/********************************************************/
void loadVEXData(LanguageData &types, LanguageData &keywords, LanguageData &builtin,
                 LanguageData &literals, LanguageData &other);

/********************************************************/
/***   CMake DATA  **************************************/
/********************************************************/
void loadCMakeData(QMultiHash<char, QLatin1String> &types,
                   QMultiHash<char, QLatin1String> &keywords,
                   QMultiHash<char, QLatin1String> &builtin,
                   QMultiHash<char, QLatin1String> &literals,
                   QMultiHash<char, QLatin1String> &other);

/********************************************************/
/***   Make DATA  ***************************************/
/********************************************************/
void loadMakeData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                  QMultiHash<char, QLatin1String> &builtin,
                  QMultiHash<char, QLatin1String> &literals,
                  QMultiHash<char, QLatin1String> &other);

void loadAsmData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                 QMultiHash<char, QLatin1String> &builtin,
                 QMultiHash<char, QLatin1String> &literals, QMultiHash<char, QLatin1String> &other);
}  // namespace QSourceHighlite
#endif  // SUP_GUI_ANYVALUEEDITOR_HIGHLIGHTER_LANGUAGEDATA_H_
