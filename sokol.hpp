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
#include "sokol/sokol_gfx.h"
#include "sokol/sokol_app.h"

namespace sg {
namespace helper {
template <typename T>
struct deleter {
    void operator()(T* handle_ptr) const {
        if (handle_ptr && handle_ptr->id != 0)
            sg::destroy(*handle_ptr);
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
        return handle_ptr ? sg::query_state(*handle_ptr) : SG_RESOURCESTATE_INVALID;
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
    static constexpr auto make = sg_make_##prefix; \
    static constexpr auto destroy = sg_destroy_##prefix; \
    static constexpr auto query_desc = sg_query_##prefix##_desc; \
    static constexpr auto query_defaults = sg_query_##prefix##_defaults; \
    static constexpr auto query_info = sg_query_##prefix##_info; \
    static constexpr auto query_state = sg_query_##prefix##_state; \
    static constexpr auto dealloc = sg_dealloc_##prefix; \
    static constexpr auto init = sg_init_##prefix; \
    static constexpr auto uninit = sg_uninit_##prefix; \
    static constexpr auto fail = sg_fail_##prefix; \
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

// Wrapper for sg_buffer_desc
class buffer_desc : public helper::desc<sg_buffer_desc> {
public:
    buffer_desc() = default;
    
    buffer_desc& size(size_t s) { desc_.size = s; return *this; }
    buffer_desc& type(sg_buffer_type t) { desc_.type = t; return *this; }
    buffer_desc& usage(sg_usage u) { desc_.usage = u; return *this; }
    buffer_desc& data(const sg_range& d) { desc_.data = d; return *this; }
    buffer_desc& label(const char* l) { desc_.label = l; return *this; }
    
    // Convenience constructors
    static buffer_desc vertex(size_t size, sg_usage usage = SG_USAGE_IMMUTABLE) {
        return buffer_desc()
            .type(SG_BUFFERTYPE_VERTEXBUFFER)
            .size(size)
            .usage(usage);
    }
    
    static buffer_desc index(size_t size, sg_usage usage = SG_USAGE_IMMUTABLE) {
        return buffer_desc()
            .type(SG_BUFFERTYPE_INDEXBUFFER)
            .size(size)
            .usage(usage);
    }
    
    static buffer_desc vertex_with_data(const void* data, size_t size) {
        return buffer_desc()
            .type(SG_BUFFERTYPE_VERTEXBUFFER)
            .size(size)
            .usage(SG_USAGE_IMMUTABLE)
            .data(sg_range{data, size});
    }
};

// Wrapper for sg_image_desc
class image_desc : public helper::desc<sg_image_desc> {
public:
    image_desc() = default;
    
    image_desc& type(sg_image_type t) { desc_.type = t; return *this; }
    image_desc& render_target(bool rt) { desc_.render_target = rt; return *this; }
    image_desc& width(int w) { desc_.width = w; return *this; }
    image_desc& height(int h) { desc_.height = h; return *this; }
    image_desc& num_slices(int ns) { desc_.num_slices = ns; return *this; }
    image_desc& num_mipmaps(int nm) { desc_.num_mipmaps = nm; return *this; }
    image_desc& usage(sg_usage u) { desc_.usage = u; return *this; }
    image_desc& pixel_format(sg_pixel_format pf) { desc_.pixel_format = pf; return *this; }
    image_desc& sample_count(int sc) { desc_.sample_count = sc; return *this; }
    image_desc& data(const sg_image_data& d) { desc_.data = d; return *this; }
    image_desc& label(const char* l) { desc_.label = l; return *this; }
    
    static image_desc texture_2d(int w, int h, sg_pixel_format fmt = SG_PIXELFORMAT_RGBA8) {
        return image_desc()
            .type(SG_IMAGETYPE_2D)
            .width(w)
            .height(h)
            .pixel_format(fmt)
            .usage(SG_USAGE_IMMUTABLE);
    }
    
    static image_desc render_target(int w, int h, sg_pixel_format fmt = SG_PIXELFORMAT_RGBA8) {
        return image_desc()
            .type(SG_IMAGETYPE_2D)
            .render_target(true)
            .width(w)
            .height(h)
            .pixel_format(fmt)
            .sample_count(1);
    }
};

// Wrapper for sg_shader_desc
class shader_desc : public helper::desc<sg_shader_desc> {
public:
    shader_desc() = default;
    
    shader_desc& vertex_shader(const char* source) {
        desc_.vs.source = source;
        return *this;
    }
    
    shader_desc& fragment_shader(const char* source) {
        desc_.fs.source = source;
        return *this;
    }
    
    shader_desc& label(const char* l) {
        desc_.label = l;
        return *this;
    }
    
    // More methods for uniforms, attributes, etc.
    shader_desc& vs_uniform_block(int index, size_t size) {
        desc_.vs.uniform_blocks[index].size = size;
        return *this;
    }
    
    shader_desc& fs_uniform_block(int index, size_t size) {
        desc_.fs.uniform_blocks[index].size = size;
        return *this;
    }
};

// Wrapper for sg_pipeline_desc
class pipeline_desc : public helper::desc<sg_pipeline_desc> {
public:
    pipeline_desc() = default;
    
    pipeline_desc& shader(sg_shader shd) { desc_.shader = shd; return *this; }
    pipeline_desc& primitive_type(sg_primitive_type pt) { desc_.primitive_type = pt; return *this; }
    pipeline_desc& index_type(sg_index_type it) { desc_.index_type = it; return *this; }
    pipeline_desc& cull_mode(sg_cull_mode cm) { desc_.cull_mode = cm; return *this; }
    pipeline_desc& depth_write_enabled(bool enabled) { desc_.depth.write_enabled = enabled; return *this; }
    pipeline_desc& depth_compare(sg_compare_func cf) { desc_.depth.compare = cf; return *this; }
    pipeline_desc& label(const char* l) { desc_.label = l; return *this; }
    
    // Vertex layout helpers
    pipeline_desc& vertex_attr(int index, int buffer_index, int offset, sg_vertex_format format) {
        desc_.layout.attrs[index].buffer_index = buffer_index;
        desc_.layout.attrs[index].offset = offset;
        desc_.layout.attrs[index].format = format;
        return *this;
    }
    
    pipeline_desc& vertex_buffer_stride(int index, int stride) {
        desc_.layout.buffers[index].stride = stride;
        return *this;
    }
};

// Wrapper for sg_sampler_desc
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

    // Convenience constructors
    static sampler_desc linear() {
        return sampler_desc()
            .min_filter(SG_FILTER_LINEAR)
            .mag_filter(SG_FILTER_LINEAR)
            .mipmap_filter(SG_FILTER_LINEAR)
            .wrap_u(SG_WRAP_CLAMP_TO_EDGE)
            .wrap_v(SG_WRAP_CLAMP_TO_EDGE)
            .wrap_w(SG_WRAP_CLAMP_TO_EDGE);
    }

    static sampler_desc nearest() {
        return sampler_desc()
            .min_filter(SG_FILTER_NEAREST)
            .mag_filter(SG_FILTER_NEAREST)
            .mipmap_filter(SG_FILTER_NEAREST)
            .wrap_u(SG_WRAP_CLAMP_TO_EDGE)
            .wrap_v(SG_WRAP_CLAMP_TO_EDGE)
            .wrap_w(SG_WRAP_CLAMP_TO_EDGE);
    }

    static sampler_desc repeating() {
        return sampler_desc()
            .min_filter(SG_FILTER_LINEAR)
            .mag_filter(SG_FILTER_LINEAR)
            .mipmap_filter(SG_FILTER_LINEAR)
            .wrap_u(SG_WRAP_REPEAT)
            .wrap_v(SG_WRAP_REPEAT)
            .wrap_w(SG_WRAP_REPEAT);
    }
};

// Wrapper for sg_texture_view_range
class texture_view_range : public helper::desc<sg_texture_view_range> {
public:
    texture_view_range() = default;

    texture_view_range& base(int b) { desc_.base = b; return *this; }
    texture_view_range& count(int c) { desc_.count = c; return *this; }
};

// Wrapper for sg_buffer_view_desc
class buffer_view_desc : public helper::desc<sg_buffer_view_desc> {
public:
    buffer_view_desc() = default;

    buffer_view_desc& buffer(sg_buffer buf) { desc_.buffer = buf; return *this; }
    buffer_view_desc& offset(int off) { desc_.offset = off; return *this; }
};

// Wrapper for sg_image_view_desc
class image_view_desc : public helper::desc<sg_image_view_desc> {
public:
    image_view_desc() = default;

    image_view_desc& image(sg_image img) { desc_.image = img; return *this; }
    image_view_desc& mip_level(int level) { desc_.mip_level = level; return *this; }
    image_view_desc& slice(int s) { desc_.slice = s; return *this; }
};

// Wrapper for sg_texture_view_desc
class texture_view_desc : public helper::desc<sg_texture_view_desc> {
public:
    texture_view_desc() = default;

    texture_view_desc& image(sg_image img) { desc_.image = img; return *this; }
    texture_view_desc& mip_levels(const sg_texture_view_range& range) { desc_.mip_levels = range; return *this; }
    texture_view_desc& slices(const sg_texture_view_range& range) { desc_.slices = range; return *this; }
};

// Wrapper for sg_view_desc
class view_desc : public helper::desc<sg_view_desc> {
public:
    view_desc() = default;

    view_desc& texture(const sg_texture_view_desc& t) { desc_.texture = t; return *this; }
    view_desc& storage_buffer(const sg_buffer_view_desc& sb) { desc_.storage_buffer = sb; return *this; }
    view_desc& storage_image(const sg_image_view_desc& si) { desc_.storage_image = si; return *this; }
    view_desc& color_attachment(const sg_image_view_desc& ca) { desc_.color_attachment = ca; return *this; }
    view_desc& resolve_attachment(const sg_image_view_desc& ra) { desc_.resolve_attachment = ra; return *this; }
    view_desc& depth_stencil_attachment(const sg_image_view_desc& dsa) { desc_.depth_stencil_attachment = dsa; return *this; }
    view_desc& label(const char* l) { desc_.label = l; return *this; }
};

// Wrapper for sg_desc (main sokol_gfx setup descriptor)
class desc : public helper::desc<sg_desc> {
public:
    desc() = default;

    desc& buffer_pool_size(int size) { desc_.buffer_pool_size = size; return *this; }
    desc& image_pool_size(int size) { desc_.image_pool_size = size; return *this; }
    desc& sampler_pool_size(int size) { desc_.sampler_pool_size = size; return *this; }
    desc& shader_pool_size(int size) { desc_.shader_pool_size = size; return *this; }
    desc& pipeline_pool_size(int size) { desc_.pipeline_pool_size = size; return *this; }
    desc& view_pool_size(int size) { desc_.view_pool_size = size; return *this; }
    desc& uniform_buffer_size(int size) { desc_.uniform_buffer_size = size; return *this; }
    desc& max_commit_listeners(int max) { desc_.max_commit_listeners = max; return *this; }
    desc& disable_validation(bool disable) { desc_.disable_validation = disable; return *this; }
    desc& enforce_portable_limits(bool enforce) { desc_.enforce_portable_limits = enforce; return *this; }
    desc& environment(const sg_environment& env) { desc_.environment = env; return *this; }

    // Convenience constructor with reasonable defaults
    static desc with_defaults() {
        return desc()
            .buffer_pool_size(128)
            .image_pool_size(128)
            .sampler_pool_size(64)
            .shader_pool_size(32)
            .pipeline_pool_size(64)
            .view_pool_size(16);
    }
};
}

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