# Project       : Graphical User Interface for SUP and PSPS
#
# Description   : Common libraries and tools for Operation Application GUIs
#
# Author        : Gennady Pospelov (IO)
#
# Copyright (c) : 2010-2025 ITER Organization,
#                 CS 90 046
#                 13067 St. Paul-lez-Durance Cedex
#                 France
# SPDX-License-Identifier: MIT
#
# This file is part of ITER CODAC software.
# For the terms and conditions of redistribution or use of this software
# refer to the file LICENSE located in the top level directory
# of the distribution package.

cmake_minimum_required(VERSION 3.13...3.31)
project(sup-gui-core-project VERSION 1.8.0 LANGUAGES CXX)

option(COA_COVERAGE "Generate unit test coverage information" OFF)
option(COA_PARASOFT_INTEGRATION "Parasoft integration" OFF)
option(COA_EXPORT_BUILD_TREE "Export build tree in /home/user/.cmake registry" OFF)
option(COA_BUILD_TESTS "Build unit tests" ON)
option(COA_BUILD_DOCUMENTATION "Build documentation" OFF)
option(COA_NO_CODAC "Don't look for the presence of CODAC environment" OFF)
option(COA_SETUP_CLANGFORMAT "Setups target to beautify the code with 'make clangformat'" OFF)
option(COA_USE_QT6 "Compile with Qt6" ON)
option(COA_FETCH_DEPS "Fetch and build dependencies from github sources" OFF)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/modules)
set(CMAKE_CONFIG_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/configs)
include(configuration)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(doc)

include(installation)

# Defines 'clangformat' target ('make clangformat').
project_codetools_setup()
