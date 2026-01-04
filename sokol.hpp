/* sokol.hpp -- https://github.com/takeiteasy/sokol.hpp

 Copyright (C) 2025 George Watson

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#pragma once
#include <type_traits>

namespace gen {
#include "sokol.inl"
}

namespace sg {
template<typename T> struct sg_type_traits;
#define DEFINE_SG_TRAITS(desc_type, handle_type, prefix) \
template<> \
struct sg_type_traits<desc_type> { \
    using handle = handle_type; \
    static inline handle make(const desc_type* desc) { return sg_make_##prefix(desc); } \
    static inline void destroy(handle h) { sg_destroy_##prefix(h); } \
    static inline desc_type query_desc(handle h) { return sg_query_##prefix##_desc(h); } \
    static inline desc_type query_defaults(const desc_type* desc) { return sg_query_##prefix##_defaults(desc); } \
    static inline sg_resource_state query_state(handle h) { return sg_query_##prefix##_state(h); } \
    static inline void dealloc(handle h) { sg_dealloc_##prefix(h); } \
    static inline void init(handle h, const desc_type* desc) { sg_init_##prefix(h, desc); } \
    static inline void uninit(handle h) { sg_uninit_##prefix(h); } \
    static inline void fail(handle h) { sg_fail_##prefix(h); } \
};

DEFINE_SG_TRAITS(sg_buffer_desc, sg_buffer, buffer)
DEFINE_SG_TRAITS(sg_image_desc, sg_image, image)
DEFINE_SG_TRAITS(sg_view_desc, sg_view, view)
DEFINE_SG_TRAITS(sg_sampler_desc, sg_sampler, sampler)
DEFINE_SG_TRAITS(sg_shader_desc, sg_shader, shader)
DEFINE_SG_TRAITS(sg_pipeline_desc, sg_pipeline, pipeline)

using buffer = gen::sg::buffer;
using image = gen::sg::image;
using sampler = gen::sg::sampler;
using shader = gen::sg::shader;
using pipeline = gen::sg::pipeline;
using view = gen::sg::view;

class buffer_desc : public gen::sg::buffer_desc {
public:
    buffer_desc() = default;

    static buffer_desc make_vertex(size_t size, bool immutable = true) {
        buffer_desc desc;
        desc.size(size).usage_vertex_buffer(true).usage_immutable(immutable);
        return desc;
    }

    static buffer_desc make_index(size_t size, bool immutable = true) {
        buffer_desc desc;
        desc.size(size).usage_index_buffer(true).usage_immutable(immutable);
        return desc;
    }

    static buffer_desc make_vertex_with_data(const void* data, size_t size) {
        buffer_desc desc;
        desc.size(size)
            .usage_vertex_buffer(true)
            .usage_immutable(true)
            .data(sg_range{data, size});
        return desc;
    }

    static buffer_desc make_index_with_data(const void* data, size_t size) {
        buffer_desc desc;
        desc.size(size)
            .usage_index_buffer(true)
            .usage_immutable(true)
            .data(sg_range{data, size});
        return desc;
    }
};

class image_desc : public gen::sg::image_desc {
public:
    image_desc() = default;

    static image_desc make_texture_2d(int w, int h, sg_pixel_format fmt = SG_PIXELFORMAT_RGBA8) {
        image_desc desc;
        desc.type(SG_IMAGETYPE_2D)
            .width(w)
            .height(h)
            .pixel_format(fmt);
        return desc;
    }

    static image_desc make_render_target(int w, int h, sg_pixel_format fmt = SG_PIXELFORMAT_RGBA8, int samples = 1) {
        image_desc desc;
        desc.type(SG_IMAGETYPE_2D)
            .width(w)
            .height(h)
            .pixel_format(fmt)
            .sample_count(samples)
            .usage_render_target(true);
        return desc;
    }

    static image_desc make_depth_stencil(int w, int h, sg_pixel_format fmt = SG_PIXELFORMAT_DEPTH_STENCIL, int samples = 1) {
        image_desc desc;
        desc.type(SG_IMAGETYPE_2D)
            .width(w)
            .height(h)
            .pixel_format(fmt)
            .sample_count(samples)
            .usage_render_target(true);
        return desc;
    }
};

class sampler_desc : public gen::sg::sampler_desc {
public:
    sampler_desc() = default;

    static sampler_desc make_linear() {
        sampler_desc desc;
        desc.min_filter(SG_FILTER_LINEAR)
            .mag_filter(SG_FILTER_LINEAR);
        return desc;
    }

    static sampler_desc make_nearest() {
        sampler_desc desc;
        desc.min_filter(SG_FILTER_NEAREST)
            .mag_filter(SG_FILTER_NEAREST);
        return desc;
    }

    static sampler_desc make_linear_clamp() {
        sampler_desc desc;
        desc.min_filter(SG_FILTER_LINEAR)
            .mag_filter(SG_FILTER_LINEAR)
            .wrap_u(SG_WRAP_CLAMP_TO_EDGE)
            .wrap_v(SG_WRAP_CLAMP_TO_EDGE);
        return desc;
    }
};

using pipeline_desc = gen::sg::pipeline_desc;
using shader_desc = gen::sg::shader_desc;
using view_desc = gen::sg::view_desc;
using buffer_view_desc = gen::sg::buffer_view_desc;
using image_view_desc = gen::sg::image_view_desc;
using texture_view_desc = gen::sg::texture_view_desc;
} // namespace sg

namespace sapp {
    using desc = gen::sapp::desc;
    using win32_desc = gen::sapp::win32_desc;
    using vulkan_desc = gen::sapp::vulkan_desc;
    using metal_desc = gen::sapp::metal_desc;
    using html5_desc = gen::sapp::html5_desc;
    using ios_desc = gen::sapp::ios_desc;
    using gl_desc = gen::sapp::gl_desc;
    using icon_desc = gen::sapp::icon_desc;
    using d3d11_desc = gen::sapp::d3d11_desc;
    using wgpu_desc = gen::sapp::wgpu_desc;
    using n3ds_desc = gen::sapp::n3ds_desc;
}

namespace saudio {
    using desc = gen::saudio::desc;
}