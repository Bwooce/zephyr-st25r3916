# Copyright (c) 2026 Bruce Fitzsimons
# SPDX-License-Identifier: Apache-2.0
cmake_minimum_required(VERSION 3.20.0)

# Make the enclosing module available without requiring a west-manifest
# entry: building this sample standalone wires the module in itself.
list(APPEND ZEPHYR_EXTRA_MODULES ${CMAKE_CURRENT_SOURCE_DIR}/../..)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(st25r3916_identity LANGUAGES C)

target_sources(app PRIVATE src/main.c)
