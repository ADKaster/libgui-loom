/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// FIXME: Workaround for https://openradar.appspot.com/21478051, remove once that's fixed.
#ifndef __COLORSYNCDEPRECATED__
#    define __COLORSYNCDEPRECATED__
#endif

#include <stdint.h>

typedef struct OpaqueCMProfileRef* CMProfileRef;
typedef uint32_t CMDisplayIDType;
typedef struct CMProfileLocation {
    void* data[4];
} CMProfileLocation;
typedef union CMColor {
    struct {
        uint16_t red;
        uint16_t green;
        uint16_t blue;
        uint16_t alpha;
    } rgb;
    uint8_t raw[8];
} CMColor;

// Several AK types conflict with MacOS types.
#define Duration DurationMacOS
#define FixedPoint FixedPointMacOS
#import <Carbon/Carbon.h>
#undef Duration
#undef FixedPoint
