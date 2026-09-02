/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Assertions.h>
#include <AK/NumericLimits.h>
#include <LibWayland/Shm.h>
#include <LibWayland/ShmPool.h>

namespace Wayland {

static StringView shm_format_to_string(u32 format)
{
    switch (format) {
    case WL_SHM_FORMAT_ARGB8888:
        return "WL_SHM_FORMAT_ARGB8888"sv;
    case WL_SHM_FORMAT_XRGB8888:
        return "WL_SHM_FORMAT_XRGB8888"sv;
    case WL_SHM_FORMAT_C8:
        return "WL_SHM_FORMAT_C8"sv;
    case WL_SHM_FORMAT_RGB332:
        return "WL_SHM_FORMAT_RGB332"sv;
    case WL_SHM_FORMAT_BGR233:
        return "WL_SHM_FORMAT_BGR233"sv;
    case WL_SHM_FORMAT_XRGB4444:
        return "WL_SHM_FORMAT_XRGB4444"sv;
    case WL_SHM_FORMAT_XBGR4444:
        return "WL_SHM_FORMAT_XBGR4444"sv;
    case WL_SHM_FORMAT_RGBX4444:
        return "WL_SHM_FORMAT_RGBX4444"sv;
    case WL_SHM_FORMAT_BGRX4444:
        return "WL_SHM_FORMAT_BGRX4444"sv;
    case WL_SHM_FORMAT_ARGB4444:
        return "WL_SHM_FORMAT_ARGB4444"sv;
    case WL_SHM_FORMAT_ABGR4444:
        return "WL_SHM_FORMAT_ABGR4444"sv;
    case WL_SHM_FORMAT_RGBA4444:
        return "WL_SHM_FORMAT_RGBA4444"sv;
    case WL_SHM_FORMAT_BGRA4444:
        return "WL_SHM_FORMAT_BGRA4444"sv;
    case WL_SHM_FORMAT_XRGB1555:
        return "WL_SHM_FORMAT_XRGB1555"sv;
    case WL_SHM_FORMAT_XBGR1555:
        return "WL_SHM_FORMAT_XBGR1555"sv;
    case WL_SHM_FORMAT_RGBX5551:
        return "WL_SHM_FORMAT_RGBX5551"sv;
    case WL_SHM_FORMAT_BGRX5551:
        return "WL_SHM_FORMAT_BGRX5551"sv;
    case WL_SHM_FORMAT_ARGB1555:
        return "WL_SHM_FORMAT_ARGB1555"sv;
    case WL_SHM_FORMAT_ABGR1555:
        return "WL_SHM_FORMAT_ABGR1555"sv;
    case WL_SHM_FORMAT_RGBA5551:
        return "WL_SHM_FORMAT_RGBA5551"sv;
    case WL_SHM_FORMAT_BGRA5551:
        return "WL_SHM_FORMAT_BGRA5551"sv;
    case WL_SHM_FORMAT_RGB565:
        return "WL_SHM_FORMAT_RGB565"sv;
    case WL_SHM_FORMAT_BGR565:
        return "WL_SHM_FORMAT_BGR565"sv;
    case WL_SHM_FORMAT_RGB888:
        return "WL_SHM_FORMAT_RGB888"sv;
    case WL_SHM_FORMAT_BGR888:
        return "WL_SHM_FORMAT_BGR888"sv;
    case WL_SHM_FORMAT_XBGR8888:
        return "WL_SHM_FORMAT_XBGR8888"sv;
    case WL_SHM_FORMAT_RGBX8888:
        return "WL_SHM_FORMAT_RGBX8888"sv;
    case WL_SHM_FORMAT_BGRX8888:
        return "WL_SHM_FORMAT_BGRX8888"sv;
    case WL_SHM_FORMAT_ABGR8888:
        return "WL_SHM_FORMAT_ABGR8888"sv;
    case WL_SHM_FORMAT_RGBA8888:
        return "WL_SHM_FORMAT_RGBA8888"sv;
    case WL_SHM_FORMAT_BGRA8888:
        return "WL_SHM_FORMAT_BGRA8888"sv;
    case WL_SHM_FORMAT_XRGB2101010:
        return "WL_SHM_FORMAT_XRGB2101010"sv;
    case WL_SHM_FORMAT_XBGR2101010:
        return "WL_SHM_FORMAT_XBGR2101010"sv;
    case WL_SHM_FORMAT_RGBX1010102:
        return "WL_SHM_FORMAT_RGBX1010102"sv;
    case WL_SHM_FORMAT_BGRX1010102:
        return "WL_SHM_FORMAT_BGRX1010102"sv;
    case WL_SHM_FORMAT_ARGB2101010:
        return "WL_SHM_FORMAT_ARGB2101010"sv;
    case WL_SHM_FORMAT_ABGR2101010:
        return "WL_SHM_FORMAT_ABGR2101010"sv;
    case WL_SHM_FORMAT_RGBA1010102:
        return "WL_SHM_FORMAT_RGBA1010102"sv;
    case WL_SHM_FORMAT_BGRA1010102:
        return "WL_SHM_FORMAT_BGRA1010102"sv;
    case WL_SHM_FORMAT_YUYV:
        return "WL_SHM_FORMAT_YUYV"sv;
    case WL_SHM_FORMAT_YVYU:
        return "WL_SHM_FORMAT_YVYU"sv;
    case WL_SHM_FORMAT_UYVY:
        return "WL_SHM_FORMAT_UYVY"sv;
    case WL_SHM_FORMAT_VYUY:
        return "WL_SHM_FORMAT_VYUY"sv;
    case WL_SHM_FORMAT_AYUV:
        return "WL_SHM_FORMAT_AYUV"sv;
    case WL_SHM_FORMAT_NV12:
        return "WL_SHM_FORMAT_NV12"sv;
    case WL_SHM_FORMAT_NV21:
        return "WL_SHM_FORMAT_NV21"sv;
    case WL_SHM_FORMAT_NV16:
        return "WL_SHM_FORMAT_NV16"sv;
    case WL_SHM_FORMAT_NV61:
        return "WL_SHM_FORMAT_NV61"sv;
    case WL_SHM_FORMAT_YUV410:
        return "WL_SHM_FORMAT_YUV410"sv;
    case WL_SHM_FORMAT_YVU410:
        return "WL_SHM_FORMAT_YVU410"sv;
    case WL_SHM_FORMAT_YUV411:
        return "WL_SHM_FORMAT_YUV411"sv;
    case WL_SHM_FORMAT_YVU411:
        return "WL_SHM_FORMAT_YVU411"sv;
    case WL_SHM_FORMAT_YUV420:
        return "WL_SHM_FORMAT_YUV420"sv;
    case WL_SHM_FORMAT_YVU420:
        return "WL_SHM_FORMAT_YVU420"sv;
    case WL_SHM_FORMAT_YUV422:
        return "WL_SHM_FORMAT_YUV422"sv;
    case WL_SHM_FORMAT_YVU422:
        return "WL_SHM_FORMAT_YVU422"sv;
    case WL_SHM_FORMAT_YUV444:
        return "WL_SHM_FORMAT_YUV444"sv;
    case WL_SHM_FORMAT_YVU444:
        return "WL_SHM_FORMAT_YVU444"sv;
    case WL_SHM_FORMAT_R8:
        return "WL_SHM_FORMAT_R8"sv;
    case WL_SHM_FORMAT_R16:
        return "WL_SHM_FORMAT_R16"sv;
    case WL_SHM_FORMAT_RG88:
        return "WL_SHM_FORMAT_RG88"sv;
    case WL_SHM_FORMAT_GR88:
        return "WL_SHM_FORMAT_GR88"sv;
    case WL_SHM_FORMAT_RG1616:
        return "WL_SHM_FORMAT_RG1616"sv;
    case WL_SHM_FORMAT_GR1616:
        return "WL_SHM_FORMAT_GR1616"sv;
    case WL_SHM_FORMAT_XRGB16161616F:
        return "WL_SHM_FORMAT_XRGB16161616F"sv;
    case WL_SHM_FORMAT_XBGR16161616F:
        return "WL_SHM_FORMAT_XBGR16161616F"sv;
    case WL_SHM_FORMAT_ARGB16161616F:
        return "WL_SHM_FORMAT_ARGB16161616F"sv;
    case WL_SHM_FORMAT_ABGR16161616F:
        return "WL_SHM_FORMAT_ABGR16161616F"sv;
    case WL_SHM_FORMAT_XYUV8888:
        return "WL_SHM_FORMAT_XYUV8888"sv;
    case WL_SHM_FORMAT_VUY888:
        return "WL_SHM_FORMAT_VUY888"sv;
    case WL_SHM_FORMAT_VUY101010:
        return "WL_SHM_FORMAT_VUY101010"sv;
    case WL_SHM_FORMAT_Y210:
        return "WL_SHM_FORMAT_Y210"sv;
    case WL_SHM_FORMAT_Y212:
        return "WL_SHM_FORMAT_Y212"sv;
    case WL_SHM_FORMAT_Y216:
        return "WL_SHM_FORMAT_Y216"sv;
    case WL_SHM_FORMAT_Y410:
        return "WL_SHM_FORMAT_Y410"sv;
    case WL_SHM_FORMAT_Y412:
        return "WL_SHM_FORMAT_Y412"sv;
    case WL_SHM_FORMAT_Y416:
        return "WL_SHM_FORMAT_Y416"sv;
    case WL_SHM_FORMAT_XVYU2101010:
        return "WL_SHM_FORMAT_XVYU2101010"sv;
    case WL_SHM_FORMAT_XVYU12_16161616:
        return "WL_SHM_FORMAT_XVYU12_16161616"sv;
    case WL_SHM_FORMAT_XVYU16161616:
        return "WL_SHM_FORMAT_XVYU16161616"sv;
    case WL_SHM_FORMAT_Y0L0:
        return "WL_SHM_FORMAT_Y0L0"sv;
    case WL_SHM_FORMAT_X0L0:
        return "WL_SHM_FORMAT_X0L0"sv;
    case WL_SHM_FORMAT_Y0L2:
        return "WL_SHM_FORMAT_Y0L2"sv;
    case WL_SHM_FORMAT_X0L2:
        return "WL_SHM_FORMAT_X0L2"sv;
    case WL_SHM_FORMAT_YUV420_8BIT:
        return "WL_SHM_FORMAT_YUV420_8BIT"sv;
    case WL_SHM_FORMAT_YUV420_10BIT:
        return "WL_SHM_FORMAT_YUV420_10BIT"sv;
    case WL_SHM_FORMAT_XRGB8888_A8:
        return "WL_SHM_FORMAT_XRGB8888_A8"sv;
    case WL_SHM_FORMAT_XBGR8888_A8:
        return "WL_SHM_FORMAT_XBGR8888_A8"sv;
    case WL_SHM_FORMAT_RGBX8888_A8:
        return "WL_SHM_FORMAT_RGBX8888_A8"sv;
    case WL_SHM_FORMAT_BGRX8888_A8:
        return "WL_SHM_FORMAT_BGRX8888_A8"sv;
    case WL_SHM_FORMAT_RGB888_A8:
        return "WL_SHM_FORMAT_RGB888_A8"sv;
    case WL_SHM_FORMAT_BGR888_A8:
        return "WL_SHM_FORMAT_BGR888_A8"sv;
    case WL_SHM_FORMAT_RGB565_A8:
        return "WL_SHM_FORMAT_RGB565_A8"sv;
    case WL_SHM_FORMAT_BGR565_A8:
        return "WL_SHM_FORMAT_BGR565_A8"sv;
    case WL_SHM_FORMAT_NV24:
        return "WL_SHM_FORMAT_NV24"sv;
    case WL_SHM_FORMAT_NV42:
        return "WL_SHM_FORMAT_NV42"sv;
    case WL_SHM_FORMAT_P210:
        return "WL_SHM_FORMAT_P210"sv;
    case WL_SHM_FORMAT_P010:
        return "WL_SHM_FORMAT_P010"sv;
    case WL_SHM_FORMAT_P012:
        return "WL_SHM_FORMAT_P012"sv;
    case WL_SHM_FORMAT_P016:
        return "WL_SHM_FORMAT_P016"sv;
    case WL_SHM_FORMAT_AXBXGXRX106106106106:
        return "WL_SHM_FORMAT_AXBXGXRX106106106106"sv;
    case WL_SHM_FORMAT_NV15:
        return "WL_SHM_FORMAT_NV15"sv;
    case WL_SHM_FORMAT_Q410:
        return "WL_SHM_FORMAT_Q410"sv;
    case WL_SHM_FORMAT_Q401:
        return "WL_SHM_FORMAT_Q401"sv;
    case WL_SHM_FORMAT_XRGB16161616:
        return "WL_SHM_FORMAT_XRGB16161616"sv;
    case WL_SHM_FORMAT_XBGR16161616:
        return "WL_SHM_FORMAT_XBGR16161616"sv;
    case WL_SHM_FORMAT_ARGB16161616:
        return "WL_SHM_FORMAT_ARGB16161616"sv;
    case WL_SHM_FORMAT_ABGR16161616:
        return "WL_SHM_FORMAT_ABGR16161616"sv;
    case WL_SHM_FORMAT_C1:
        return "WL_SHM_FORMAT_C1"sv;
    case WL_SHM_FORMAT_C2:
        return "WL_SHM_FORMAT_C2"sv;
    case WL_SHM_FORMAT_C4:
        return "WL_SHM_FORMAT_C4"sv;
    case WL_SHM_FORMAT_D1:
        return "WL_SHM_FORMAT_D1"sv;
    case WL_SHM_FORMAT_D2:
        return "WL_SHM_FORMAT_D2"sv;
    case WL_SHM_FORMAT_D4:
        return "WL_SHM_FORMAT_D4"sv;
    case WL_SHM_FORMAT_D8:
        return "WL_SHM_FORMAT_D8"sv;
    case WL_SHM_FORMAT_R1:
        return "WL_SHM_FORMAT_R1"sv;
    case WL_SHM_FORMAT_R2:
        return "WL_SHM_FORMAT_R2"sv;
    case WL_SHM_FORMAT_R4:
        return "WL_SHM_FORMAT_R4"sv;
    case WL_SHM_FORMAT_R10:
        return "WL_SHM_FORMAT_R10"sv;
    case WL_SHM_FORMAT_R12:
        return "WL_SHM_FORMAT_R12"sv;
    case WL_SHM_FORMAT_AVUY8888:
        return "WL_SHM_FORMAT_AVUY8888"sv;
    case WL_SHM_FORMAT_XVUY8888:
        return "WL_SHM_FORMAT_XVUY8888"sv;
    case WL_SHM_FORMAT_P030:
        return "WL_SHM_FORMAT_P030"sv;
    case WL_SHM_FORMAT_RGB161616:
        return "WL_SHM_FORMAT_RGB161616"sv;
    case WL_SHM_FORMAT_BGR161616:
        return "WL_SHM_FORMAT_BGR161616"sv;
    case WL_SHM_FORMAT_R16F:
        return "WL_SHM_FORMAT_R16F"sv;
    case WL_SHM_FORMAT_GR1616F:
        return "WL_SHM_FORMAT_GR1616F"sv;
    case WL_SHM_FORMAT_BGR161616F:
        return "WL_SHM_FORMAT_BGR161616F"sv;
    case WL_SHM_FORMAT_R32F:
        return "WL_SHM_FORMAT_R32F"sv;
    case WL_SHM_FORMAT_GR3232F:
        return "WL_SHM_FORMAT_GR3232F"sv;
    case WL_SHM_FORMAT_BGR323232F:
        return "WL_SHM_FORMAT_BGR323232F"sv;
    case WL_SHM_FORMAT_ABGR32323232F:
        return "WL_SHM_FORMAT_ABGR32323232F"sv;
    case WL_SHM_FORMAT_NV20:
        return "WL_SHM_FORMAT_NV20"sv;
    case WL_SHM_FORMAT_NV30:
        return "WL_SHM_FORMAT_NV30"sv;
    case WL_SHM_FORMAT_S010:
        return "WL_SHM_FORMAT_S010"sv;
    case WL_SHM_FORMAT_S210:
        return "WL_SHM_FORMAT_S210"sv;
    case WL_SHM_FORMAT_S410:
        return "WL_SHM_FORMAT_S410"sv;
    case WL_SHM_FORMAT_S012:
        return "WL_SHM_FORMAT_S012"sv;
    case WL_SHM_FORMAT_S212:
        return "WL_SHM_FORMAT_S212"sv;
    case WL_SHM_FORMAT_S412:
        return "WL_SHM_FORMAT_S412"sv;
    case WL_SHM_FORMAT_S016:
        return "WL_SHM_FORMAT_S016"sv;
    case WL_SHM_FORMAT_S216:
        return "WL_SHM_FORMAT_S216"sv;
    case WL_SHM_FORMAT_S416:
        return "WL_SHM_FORMAT_S416"sv;
    case WL_SHM_FORMAT_XVUY2101010:
        return "WL_SHM_FORMAT_XVUY2101010"sv;
    case WL_SHM_FORMAT_P230:
        return "WL_SHM_FORMAT_P230"sv;
    case WL_SHM_FORMAT_T430:
        return "WL_SHM_FORMAT_T430"sv;
    case WL_SHM_FORMAT_Y8:
        return "WL_SHM_FORMAT_Y8"sv;
    case WL_SHM_FORMAT_XYYY2101010:
        return "WL_SHM_FORMAT_XYYY2101010"sv;
    default:
        return "(unknown)"sv;
    }
}

static const wl_shm_listener s_shm_listener = {
    .format = [](void*, wl_shm*, u32 format) {
        dbgln("Shm::format: allowed format={:x} ({})", format, shm_format_to_string(format));
    },
};

Shm::Shm(wl_shm* shm)
    : m_shm(shm)
{
    VERIFY(m_shm != nullptr);
    wl_shm_add_listener(m_shm, &s_shm_listener, this);
}

Shm::~Shm()
{
    wl_shm_destroy(m_shm);
}

NonnullOwnPtr<ShmPool> Shm::create_pool(Core::AnonymousBuffer buffer)
{
    auto fd = buffer.fd();
    auto const size = buffer.size();
    VERIFY(size < NumericLimits<i32>::max());
    return make<ShmPool>(wl_shm_create_pool(m_shm, fd, static_cast<i32>(size)), move(buffer));
}

}
