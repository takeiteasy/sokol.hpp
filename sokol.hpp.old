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
#include <utility>
#include <memory>

#ifndef SOKOL_GFX_INCLUDED
#if __has_include("sokol_gfx.h")
#include "sokol_gfx.h"
#elif __has_include("sokol/sokol_gfx.h")
#include "sokol/sokol_gfx.h"
#else
#error Can't find sokol_gfx.h, please include sokol_gfx before including sokol.hpp
#endif
#endif

#ifndef SOKOL_APP_INCLUDED
#if __has_include("sokol_app.h")
#include "sokol_app.h"
#elif __has_include("sokol/sokol_app.h")
#include "sokol/sokol_app.h"
#else
#error Can't find sokol_app.h, please include sokol_app before including sokol.hpp
#endif
#endif

namespace sg {
namespace helper {
template <typename T>
struct deleter {
    void operator()(T* handle_ptr) const {
        if (handle_ptr && handle_ptr->id != 0) {
            if constexpr (std::is_same_v<T, sg_buffer>)
                sg_destroy_buffer(*handle_ptr);
            else if constexpr (std::is_same_v<T, sg_image>)
                sg_destroy_image(*handle_ptr);
            else if constexpr (std::is_same_v<T, sg_view>)
                sg_destroy_view(*handle_ptr);
            else if constexpr (std::is_same_v<T, sg_sampler>)
                sg_destroy_sampler(*handle_ptr);
            else if constexpr (std::is_same_v<T, sg_shader>)
                sg_destroy_shader(*handle_ptr);
            else if constexpr (std::is_same_v<T, sg_pipeline>)
                sg_destroy_pipeline(*handle_ptr);
        }
        delete handle_ptr;
    }
};

template <typename T>
class ptr {
    using ptr_t = std::unique_ptr<T, deleter<T>>;
    ptr_t handle_ptr;

public:
    ptr() : handle_ptr(nullptr) {}
    ptr(const ptr &other) = delete;
    ptr &operator=(const ptr &other) = delete;
    ptr(ptr &&other) noexcept = default;
    ptr &operator=(ptr &&other) noexcept = default;
    ptr(const T &h) : handle_ptr(h.id != 0 ? new T(h) : nullptr) {}

    ptr &operator=(const T &h) {
        if (h.id != 0)
            handle_ptr = ptr_t(new T(h));
        else
            handle_ptr.reset();
        return *this;
    }

    operator T() const {
        return handle_ptr ? *handle_ptr : T{0};
    }

    T get() const {
        return handle_ptr ? *handle_ptr : T{0};
    }

    uint32_t id() const {
        return handle_ptr ? handle_ptr->id : 0;
    }

    sg_resource_state state() const {
        if (!handle_ptr)
            return SG_RESOURCESTATE_INVALID;
        if constexpr (std::is_same_v<T, sg_buffer>)
            return sg_query_buffer_state(*handle_ptr);
        else if constexpr (std::is_same_v<T, sg_image>)
            return sg_query_image_state(*handle_ptr);
        else if constexpr (std::is_same_v<T, sg_view>)
            return sg_query_view_state(*handle_ptr);
        else if constexpr (std::is_same_v<T, sg_sampler>)
            return sg_query_sampler_state(*handle_ptr);
        else if constexpr (std::is_same_v<T, sg_shader>)
            return sg_query_shader_state(*handle_ptr);
        else if constexpr (std::is_same_v<T, sg_pipeline>)
            return sg_query_pipeline_state(*handle_ptr);
        return SG_RESOURCESTATE_INVALID;
    }

    bool is_valid() const {
        return handle_ptr && handle_ptr->id != 0 && state() == SG_RESOURCESTATE_VALID;
    }

    // Reset the pointer (release the resource)
    void reset() {
        handle_ptr.reset();
    }

    // Release ownership without destroying the resource
    T release() {
        if (handle_ptr) {
            T result = *handle_ptr;
            handle_ptr.release(); // Don't delete, just release ownership
            return result;
        }
        return T{0};
    }
};
}

using buffer = helper::ptr<sg_buffer>;
using image = helper::ptr<sg_image>;
using view = helper::ptr<sg_view>;
using sampler = helper::ptr<sg_sampler>;
using shader = helper::ptr<sg_shader>;
using pipeline = helper::ptr<sg_pipeline>;

template<typename T>
struct sg_type_traits;

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

// Define traits for all types
DEFINE_SG_TRAITS(sg_buffer_desc, sg_buffer, buffer)
DEFINE_SG_TRAITS(sg_image_desc, sg_image, image)
DEFINE_SG_TRAITS(sg_view_desc, sg_view, view)
DEFINE_SG_TRAITS(sg_sampler_desc, sg_sampler, sampler)
DEFINE_SG_TRAITS(sg_shader_desc, sg_shader, shader)
DEFINE_SG_TRAITS(sg_pipeline_desc, sg_pipeline, pipeline)

namespace helper {
template<typename T>
static inline const auto* get_ptr(T&& desc) {
    using DecayedT = std::decay_t<T>;
    using PointeeT = std::conditional_t<std::is_pointer_v<DecayedT>, 
                                        std::remove_pointer_t<DecayedT>, 
                                        DecayedT>;
    if constexpr (std::is_pointer_v<DecayedT>)
        return desc;
    else
        return &desc;
}
}

// Simplified functions
template<typename T>
static inline auto make(T&& desc) {
    using DecayedT = std::decay_t<T>;
    using PointeeT = std::conditional_t<std::is_pointer_v<DecayedT>, 
                                        std::remove_pointer_t<DecayedT>, 
                                        DecayedT>;
    return sg_type_traits<PointeeT>::make(helper::get_ptr(std::forward<T>(desc)));
}

template<typename T>
static inline void destroy(T&& desc) {
    using DecayedT = std::decay_t<T>;
    using PointeeT = std::conditional_t<std::is_pointer_v<DecayedT>, 
                                        std::remove_pointer_t<DecayedT>, 
                                        DecayedT>;
    sg_type_traits<PointeeT>::destroy(helper::get_ptr(std::forward<T>(desc)));
}

template<typename T>
static inline auto query_desc(T&& desc) {
    using DecayedT = std::decay_t<T>;
    using PointeeT = std::conditional_t<std::is_pointer_v<DecayedT>, 
                                        std::remove_pointer_t<DecayedT>, 
                                        DecayedT>;
    return sg_type_traits<PointeeT>::query_desc(helper::get_ptr(std::forward<T>(desc)));
}

template<typename T>
static inline auto query_defaults(T&& desc) {
    using DecayedT = std::decay_t<T>;
    using PointeeT = std::conditional_t<std::is_pointer_v<DecayedT>, 
                                        std::remove_pointer_t<DecayedT>, 
                                        DecayedT>;
    return sg_type_traits<PointeeT>::query_defaults(helper::get_ptr(std::forward<T>(desc)));
}

template<typename T>
static inline auto query_info(T&& desc) {
    using DecayedT = std::decay_t<T>;
    using PointeeT = std::conditional_t<std::is_pointer_v<DecayedT>, 
                                        std::remove_pointer_t<DecayedT>, 
                                        DecayedT>;
    return sg_type_traits<PointeeT>::query_info(helper::get_ptr(std::forward<T>(desc)));
}

// For handle-based functions (no pointer conversion needed)
template<typename T>
static inline auto query_state(T handle) {
    return sg_type_traits<std::decay_t<T>>::query_state(handle);
}

template<typename T>
static inline void dealloc(T handle) {
    sg_type_traits<std::decay_t<T>>::dealloc(handle);
}

template<typename T, typename DT>
static inline void init(T handle, DT&& desc) {
    using DecayedT = std::decay_t<DT>;
    using PointeeT = std::conditional_t<std::is_pointer_v<DecayedT>, 
                                        std::remove_pointer_t<DecayedT>, 
                                        DecayedT>;
    sg_type_traits<PointeeT>::init(handle, helper::get_ptr(std::forward<DT>(desc)));
}

template<typename T>
static inline void uninit(T handle) {
    sg_type_traits<std::decay_t<T>>::uninit(handle);
}

template<typename T>
static inline void fail(T handle) {
    sg_type_traits<std::decay_t<T>>::fail(handle);
}

namespace helper {
template<typename T>
class desc {
protected:
    T desc_ = {};
    
public:
    desc() = default;
    
    // Conversion operator - allows implicit conversion to the C struct
    operator const T&() const { return desc_; }
    operator T&() { return desc_; }
    
    // Explicit pointer access
    const T* operator&() const { return &desc_; }
    T* operator&() { return &desc_; }
    
    // Access to underlying descriptor
    const T& get() const { return desc_; }
    T& get() { return desc_; }
};
}

// Builder API for sg_buffer_desc
class buffer_desc : public helper::desc<sg_buffer_desc> {
public:
    buffer_desc() = default;

    buffer_desc& size(size_t s) { desc_.size = s; return *this; }
    buffer_desc& usage(const sg_buffer_usage& u) { desc_.usage = u; return *this; }
    buffer_desc& data(const sg_range& d) { desc_.data = d; return *this; }
    buffer_desc& label(const char* l) { desc_.label = l; return *this; }

    // Usage flag helpers
    buffer_desc& vertex_buffer(bool v = true) { desc_.usage.vertex_buffer = v; return *this; }
    buffer_desc& index_buffer(bool v = true) { desc_.usage.index_buffer = v; return *this; }
    buffer_desc& storage_buffer(bool v = true) { desc_.usage.storage_buffer = v; return *this; }
    buffer_desc& immutable(bool v = true) { desc_.usage.immutable = v; return *this; }
    buffer_desc& dynamic_update(bool v = true) { desc_.usage.dynamic_update = v; return *this; }
    buffer_desc& stream_update(bool v = true) { desc_.usage.stream_update = v; return *this; }

    // Convenience factory methods
    static buffer_desc make_vertex(size_t size, bool immutable = true) {
        return buffer_desc()
            .size(size)
            .vertex_buffer()
            .immutable(immutable);
    }

    static buffer_desc make_index(size_t size, bool immutable = true) {
        return buffer_desc()
            .size(size)
            .index_buffer()
            .immutable(immutable);
    }

    static buffer_desc make_vertex_with_data(const void* data, size_t size) {
        return buffer_desc()
            .size(size)
            .vertex_buffer()
            .immutable()
            .data(sg_range{data, size});
    }

    static buffer_desc make_index_with_data(const void* data, size_t size) {
        return buffer_desc()
            .size(size)
            .index_buffer()
            .immutable()
            .data(sg_range{data, size});
    }
};

// Builder API for sg_image_desc
class image_desc : public helper::desc<sg_image_desc> {
public:
    image_desc() = default;

    image_desc& type(sg_image_type t) { desc_.type = t; return *this; }
    image_desc& usage(const sg_image_usage& u) { desc_.usage = u; return *this; }
    image_desc& width(int w) { desc_.width = w; return *this; }
    image_desc& height(int h) { desc_.height = h; return *this; }
    image_desc& num_slices(int ns) { desc_.num_slices = ns; return *this; }
    image_desc& num_mipmaps(int nm) { desc_.num_mipmaps = nm; return *this; }
    image_desc& pixel_format(sg_pixel_format pf) { desc_.pixel_format = pf; return *this; }
    image_desc& sample_count(int sc) { desc_.sample_count = sc; return *this; }
    image_desc& data(const sg_image_data& d) { desc_.data = d; return *this; }
    image_desc& label(const char* l) { desc_.label = l; return *this; }

    // Usage flag helpers
    image_desc& storage_image(bool v = true) { desc_.usage.storage_image = v; return *this; }
    image_desc& color_attachment(bool v = true) { desc_.usage.color_attachment = v; return *this; }
    image_desc& resolve_attachment(bool v = true) { desc_.usage.resolve_attachment = v; return *this; }
    image_desc& depth_stencil_attachment(bool v = true) { desc_.usage.depth_stencil_attachment = v; return *this; }
    image_desc& immutable(bool v = true) { desc_.usage.immutable = v; return *this; }
    image_desc& dynamic_update(bool v = true) { desc_.usage.dynamic_update = v; return *this; }
    image_desc& stream_update(bool v = true) { desc_.usage.stream_update = v; return *this; }

    // Convenience factory methods
    static image_desc make_texture_2d(int w, int h, sg_pixel_format fmt = SG_PIXELFORMAT_RGBA8) {
        return image_desc()
            .type(SG_IMAGETYPE_2D)
            .width(w)
            .height(h)
            .pixel_format(fmt)
            .immutable();
    }

    static image_desc make_render_target(int w, int h, sg_pixel_format fmt = SG_PIXELFORMAT_RGBA8, int samples = 1) {
        return image_desc()
            .type(SG_IMAGETYPE_2D)
            .width(w)
            .height(h)
            .pixel_format(fmt)
            .sample_count(samples)
            .color_attachment();
    }

    static image_desc make_depth_stencil(int w, int h, sg_pixel_format fmt = SG_PIXELFORMAT_DEPTH_STENCIL, int samples = 1) {
        return image_desc()
            .type(SG_IMAGETYPE_2D)
            .width(w)
            .height(h)
            .pixel_format(fmt)
            .sample_count(samples)
            .depth_stencil_attachment();
    }
};

// Builder API for sg_shader_desc
class shader_desc : public helper::desc<sg_shader_desc> {
public:
    shader_desc() = default;

    shader_desc& vertex_source(const char* source) {
        desc_.vertex_func.source = source;
        return *this;
    }

    shader_desc& vertex_bytecode(const sg_range& bc) {
        desc_.vertex_func.bytecode = bc;
        return *this;
    }

    shader_desc& vertex_entry(const char* entry) {
        desc_.vertex_func.entry = entry;
        return *this;
    }

    shader_desc& fragment_source(const char* source) {
        desc_.fragment_func.source = source;
        return *this;
    }

    shader_desc& fragment_bytecode(const sg_range& bc) {
        desc_.fragment_func.bytecode = bc;
        return *this;
    }

    shader_desc& fragment_entry(const char* entry) {
        desc_.fragment_func.entry = entry;
        return *this;
    }

    shader_desc& compute_source(const char* source) {
        desc_.compute_func.source = source;
        return *this;
    }

    shader_desc& compute_bytecode(const sg_range& bc) {
        desc_.compute_func.bytecode = bc;
        return *this;
    }

    shader_desc& compute_entry(const char* entry) {
        desc_.compute_func.entry = entry;
        return *this;
    }

    shader_desc& label(const char* l) {
        desc_.label = l;
        return *this;
    }

    shader_desc& attr(int index, const char* glsl_name = nullptr, const char* hlsl_sem_name = nullptr, int hlsl_sem_index = 0) {
        if (glsl_name) desc_.attrs[index].glsl_name = glsl_name;
        if (hlsl_sem_name) desc_.attrs[index].hlsl_sem_name = hlsl_sem_name;
        desc_.attrs[index].hlsl_sem_index = hlsl_sem_index;
        return *this;
    }

    shader_desc& uniform_block(int index, size_t size) {
        desc_.uniform_blocks[index].size = size;
        return *this;
    }
};

// Builder API for sg_pipeline_desc
class pipeline_desc : public helper::desc<sg_pipeline_desc> {
public:
    pipeline_desc() = default;

    pipeline_desc& shader(sg_shader shd) { desc_.shader = shd; return *this; }
    pipeline_desc& primitive_type(sg_primitive_type pt) { desc_.primitive_type = pt; return *this; }
    pipeline_desc& index_type(sg_index_type it) { desc_.index_type = it; return *this; }
    pipeline_desc& cull_mode(sg_cull_mode cm) { desc_.cull_mode = cm; return *this; }
    pipeline_desc& face_winding(sg_face_winding fw) { desc_.face_winding = fw; return *this; }
    pipeline_desc& sample_count(int sc) { desc_.sample_count = sc; return *this; }
    pipeline_desc& blend_color(sg_color c) { desc_.blend_color = c; return *this; }
    pipeline_desc& alpha_to_coverage(bool v) { desc_.alpha_to_coverage_enabled = v; return *this; }
    pipeline_desc& label(const char* l) { desc_.label = l; return *this; }
    pipeline_desc& compute(bool v = true) { desc_.compute = v; return *this; }
    pipeline_desc& color_count(int count) { desc_.color_count = count; return *this; }

    // Depth state helpers
    pipeline_desc& depth_write_enabled(bool enabled) { desc_.depth.write_enabled = enabled; return *this; }
    pipeline_desc& depth_compare(sg_compare_func cf) { desc_.depth.compare = cf; return *this; }
    pipeline_desc& depth_bias(float bias) { desc_.depth.bias = bias; return *this; }
    pipeline_desc& depth_bias_slope_scale(float scale) { desc_.depth.bias_slope_scale = scale; return *this; }
    pipeline_desc& depth_bias_clamp(float clamp) { desc_.depth.bias_clamp = clamp; return *this; }

    // Stencil state helpers
    pipeline_desc& stencil_enabled(bool v) { desc_.stencil.enabled = v; return *this; }
    pipeline_desc& stencil_front_compare(sg_compare_func cf) { desc_.stencil.front.compare = cf; return *this; }
    pipeline_desc& stencil_front_fail_op(sg_stencil_op op) { desc_.stencil.front.fail_op = op; return *this; }
    pipeline_desc& stencil_front_depth_fail_op(sg_stencil_op op) { desc_.stencil.front.depth_fail_op = op; return *this; }
    pipeline_desc& stencil_front_pass_op(sg_stencil_op op) { desc_.stencil.front.pass_op = op; return *this; }
    pipeline_desc& stencil_back_compare(sg_compare_func cf) { desc_.stencil.back.compare = cf; return *this; }
    pipeline_desc& stencil_back_fail_op(sg_stencil_op op) { desc_.stencil.back.fail_op = op; return *this; }
    pipeline_desc& stencil_back_depth_fail_op(sg_stencil_op op) { desc_.stencil.back.depth_fail_op = op; return *this; }
    pipeline_desc& stencil_back_pass_op(sg_stencil_op op) { desc_.stencil.back.pass_op = op; return *this; }
    pipeline_desc& stencil_read_mask(uint8_t mask) { desc_.stencil.read_mask = mask; return *this; }
    pipeline_desc& stencil_write_mask(uint8_t mask) { desc_.stencil.write_mask = mask; return *this; }
    pipeline_desc& stencil_ref(uint8_t ref) { desc_.stencil.ref = ref; return *this; }

    // Vertex layout helpers
    pipeline_desc& layout_attr(int index, int buffer_index, int offset, sg_vertex_format format) {
        desc_.layout.attrs[index].buffer_index = buffer_index;
        desc_.layout.attrs[index].offset = offset;
        desc_.layout.attrs[index].format = format;
        return *this;
    }

    pipeline_desc& layout_buffer_stride(int index, int stride) {
        desc_.layout.buffers[index].stride = stride;
        return *this;
    }

    pipeline_desc& layout_buffer_step_func(int index, sg_vertex_step step) {
        desc_.layout.buffers[index].step_func = step;
        return *this;
    }

    pipeline_desc& layout_buffer_step_rate(int index, int rate) {
        desc_.layout.buffers[index].step_rate = rate;
        return *this;
    }

    // Color target helpers
    pipeline_desc& color_format(int index, sg_pixel_format fmt) {
        desc_.colors[index].pixel_format = fmt;
        return *this;
    }

    pipeline_desc& color_write_mask(int index, sg_color_mask mask) {
        desc_.colors[index].write_mask = mask;
        return *this;
    }

    pipeline_desc& color_blend_enabled(int index, bool v) {
        desc_.colors[index].blend.enabled = v;
        return *this;
    }

    pipeline_desc& color_blend_src_factor_rgb(int index, sg_blend_factor f) {
        desc_.colors[index].blend.src_factor_rgb = f;
        return *this;
    }

    pipeline_desc& color_blend_dst_factor_rgb(int index, sg_blend_factor f) {
        desc_.colors[index].blend.dst_factor_rgb = f;
        return *this;
    }

    pipeline_desc& color_blend_op_rgb(int index, sg_blend_op op) {
        desc_.colors[index].blend.op_rgb = op;
        return *this;
    }

    pipeline_desc& color_blend_src_factor_alpha(int index, sg_blend_factor f) {
        desc_.colors[index].blend.src_factor_alpha = f;
        return *this;
    }

    pipeline_desc& color_blend_dst_factor_alpha(int index, sg_blend_factor f) {
        desc_.colors[index].blend.dst_factor_alpha = f;
        return *this;
    }

    pipeline_desc& color_blend_op_alpha(int index, sg_blend_op op) {
        desc_.colors[index].blend.op_alpha = op;
        return *this;
    }
};

// Builder API for sg_sampler_desc
class sampler_desc : public helper::desc<sg_sampler_desc> {
public:
    sampler_desc() = default;

    sampler_desc& min_filter(sg_filter f) { desc_.min_filter = f; return *this; }
    sampler_desc& mag_filter(sg_filter f) { desc_.mag_filter = f; return *this; }
    sampler_desc& mipmap_filter(sg_filter f) { desc_.mipmap_filter = f; return *this; }
    sampler_desc& wrap_u(sg_wrap w) { desc_.wrap_u = w; return *this; }
    sampler_desc& wrap_v(sg_wrap w) { desc_.wrap_v = w; return *this; }
    sampler_desc& wrap_w(sg_wrap w) { desc_.wrap_w = w; return *this; }
    sampler_desc& min_lod(float lod) { desc_.min_lod = lod; return *this; }
    sampler_desc& max_lod(float lod) { desc_.max_lod = lod; return *this; }
    sampler_desc& border_color(sg_border_color bc) { desc_.border_color = bc; return *this; }
    sampler_desc& compare(sg_compare_func cf) { desc_.compare = cf; return *this; }
    sampler_desc& max_anisotropy(uint32_t ma) { desc_.max_anisotropy = ma; return *this; }
    sampler_desc& label(const char* l) { desc_.label = l; return *this; }

    // Convenience factory methods
    static sampler_desc make_linear() {
        return sampler_desc()
            .min_filter(SG_FILTER_LINEAR)
            .mag_filter(SG_FILTER_LINEAR);
    }

    static sampler_desc make_nearest() {
        return sampler_desc()
            .min_filter(SG_FILTER_NEAREST)
            .mag_filter(SG_FILTER_NEAREST);
    }

    static sampler_desc make_linear_clamp() {
        return sampler_desc()
            .min_filter(SG_FILTER_LINEAR)
            .mag_filter(SG_FILTER_LINEAR)
            .wrap_u(SG_WRAP_CLAMP_TO_EDGE)
            .wrap_v(SG_WRAP_CLAMP_TO_EDGE);
    }
};

// Builder API for sg_desc (context descriptor)
class context_desc : public helper::desc<sg_desc> {
public:
    context_desc() = default;

    context_desc& buffer_pool_size(int size) { desc_.buffer_pool_size = size; return *this; }
    context_desc& image_pool_size(int size) { desc_.image_pool_size = size; return *this; }
    context_desc& sampler_pool_size(int size) { desc_.sampler_pool_size = size; return *this; }
    context_desc& shader_pool_size(int size) { desc_.shader_pool_size = size; return *this; }
    context_desc& pipeline_pool_size(int size) { desc_.pipeline_pool_size = size; return *this; }
    context_desc& view_pool_size(int size) { desc_.view_pool_size = size; return *this; }
    context_desc& uniform_buffer_size(int size) { desc_.uniform_buffer_size = size; return *this; }
    context_desc& max_commit_listeners(int max) { desc_.max_commit_listeners = max; return *this; }
    context_desc& disable_validation(bool disable) { desc_.disable_validation = disable; return *this; }

    static context_desc make_defaults() {
        return context_desc()
            .buffer_pool_size(128)
            .image_pool_size(128)
            .sampler_pool_size(64)
            .shader_pool_size(32)
            .pipeline_pool_size(64)
            .view_pool_size(16);
    }
};
} // namespace sg


namespace sapp {
    // Specific wrapper for sapp_desc with builder-style API
class desc : public sg::helper::desc<sapp_desc> {
public:
    desc() = default;
    
    desc& width(int w) { desc_.width = w; return *this; }
    desc& height(int h) { desc_.height = h; return *this; }
    desc& sample_count(int sc) { desc_.sample_count = sc; return *this; }
    desc& swap_interval(int si) { desc_.swap_interval = si; return *this; }
    desc& high_dpi(bool hd) { desc_.high_dpi = hd; return *this; }
    desc& fullscreen(bool fs) { desc_.fullscreen = fs; return *this; }
    desc& alpha(bool a) { desc_.alpha = a; return *this; }
    desc& window_title(const char* title) { desc_.window_title = title; return *this; }
    desc& enable_clipboard(bool ec) { desc_.enable_clipboard = ec; return *this; }
    desc& clipboard_size(int size) { desc_.clipboard_size = size; return *this; }
    desc& enable_dragndrop(bool ed) { desc_.enable_dragndrop = ed; return *this; }
    desc& max_dropped_files(int max) { desc_.max_dropped_files = max; return *this; }
    desc& max_dropped_file_path_length(int max) { desc_.max_dropped_file_path_length = max; return *this; }
    
    // Callback setters
    desc& init_cb(void (*cb)()) { desc_.init_cb = cb; return *this; }
    desc& frame_cb(void (*cb)()) { desc_.frame_cb = cb; return *this; }
    desc& cleanup_cb(void (*cb)()) { desc_.cleanup_cb = cb; return *this; }
    desc& event_cb(void (*cb)(const sapp_event*)) { desc_.event_cb = cb; return *this; }
    
    // Constructor with common defaults
    static desc with_defaults(int w = 800, int h = 600, const char* title = "Sokol App") {
        return desc()
            .width(w)
            .height(h)
            .window_title(title)
            .sample_count(4);
    }
};

// Wrapper for sapp_image_desc (from sokol_app.h)
class image_desc : public sg::helper::desc<::sapp_image_desc> {
public:
    image_desc() = default;

    image_desc& width(int w) { desc_.width = w; return *this; }
    image_desc& height(int h) { desc_.height = h; return *this; }
    image_desc& cursor_hotspot_x(int x) { desc_.cursor_hotspot_x = x; return *this; }
    image_desc& cursor_hotspot_y(int y) { desc_.cursor_hotspot_y = y; return *this; }
    image_desc& pixels(const sapp_range& p) { desc_.pixels = p; return *this; }
};

// Wrapper for sapp_icon_desc
class icon_desc : public sg::helper::desc<::sapp_icon_desc> {
public:
    icon_desc() = default;

    icon_desc& sokol_default(bool use_default) { desc_.sokol_default = use_default; return *this; }

    // Helper to set an image at a specific index
    icon_desc& set_image(int index, const sapp_image_desc& img) {
        if (index >= 0 && index < SAPP_MAX_ICONIMAGES) {
            desc_.images[index] = img;
        }
        return *this;
    }
};
}