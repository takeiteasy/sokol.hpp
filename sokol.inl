// Machine generated C++ wrapper for Sokol library.
// https://github.com/takeiteasy/sokol.hpp
//
// Do not edit manually; regenerate using gen_cpp.py.

#pragma once
#include <utility>
#include <memory>

#ifndef SOKOL_LOG_INCLUDED
#if __has_include("sokol_log.h")
#include "sokol_log.h"
#elif __has_include("sokol/sokol_log.h")
#include "sokol/sokol_log.h"
#else
#warning "Cannot find sokol_log.h, disabling!"
#define SOKOL_NO_SLOG
#endif
#endif

#ifndef SOKOL_NO_SLOG
namespace slog {
} // namespace slog
#endif // SOKOL_NO_SLOG

#ifndef SOKOL_GFX_INCLUDED
#if __has_include("sokol_gfx.h")
#include "sokol_gfx.h"
#elif __has_include("sokol/sokol_gfx.h")
#include "sokol/sokol_gfx.h"
#else
#warning "Cannot find sokol_gfx.h, disabling!"
#define SOKOL_NO_SG
#endif
#endif

#ifndef SOKOL_NO_SG
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
            else if constexpr (std::is_same_v<T, sg_sampler>)
                sg_destroy_sampler(*handle_ptr);
            else if constexpr (std::is_same_v<T, sg_shader>)
                sg_destroy_shader(*handle_ptr);
            else if constexpr (std::is_same_v<T, sg_pipeline>)
                sg_destroy_pipeline(*handle_ptr);
            else if constexpr (std::is_same_v<T, sg_view>)
                sg_destroy_view(*handle_ptr);
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
        else if constexpr (std::is_same_v<T, sg_sampler>)
            return sg_query_sampler_state(*handle_ptr);
        else if constexpr (std::is_same_v<T, sg_shader>)
            return sg_query_shader_state(*handle_ptr);
        else if constexpr (std::is_same_v<T, sg_pipeline>)
            return sg_query_pipeline_state(*handle_ptr);
        else if constexpr (std::is_same_v<T, sg_view>)
            return sg_query_view_state(*handle_ptr);
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
} // namespace helper

// RAII wrapper type aliases
using buffer = helper::ptr<sg_buffer>;
using image = helper::ptr<sg_image>;
using sampler = helper::ptr<sg_sampler>;
using shader = helper::ptr<sg_shader>;
using pipeline = helper::ptr<sg_pipeline>;
using view = helper::ptr<sg_view>;

// Builder API for sg_buffer_desc
class buffer_desc : public helper::desc<sg_buffer_desc> {
public:
    buffer_desc() = default;

    buffer_desc& size(size_t value) {
        desc_.size = value;
        return *this;
    }

    buffer_desc& usage_vertex_buffer(bool value) {
        desc_.usage.vertex_buffer = value;
        return *this;
    }

    buffer_desc& usage_index_buffer(bool value) {
        desc_.usage.index_buffer = value;
        return *this;
    }

    buffer_desc& usage_storage_buffer(bool value) {
        desc_.usage.storage_buffer = value;
        return *this;
    }

    buffer_desc& usage_immutable(bool value) {
        desc_.usage.immutable = value;
        return *this;
    }

    buffer_desc& usage_dynamic_update(bool value) {
        desc_.usage.dynamic_update = value;
        return *this;
    }

    buffer_desc& usage_stream_update(bool value) {
        desc_.usage.stream_update = value;
        return *this;
    }

    buffer_desc& data_ptr(const void* value) {
        desc_.data.ptr = value;
        return *this;
    }

    buffer_desc& data_size(size_t value) {
        desc_.data.size = value;
        return *this;
    }

    buffer_desc& label(const char* value) {
        desc_.label = value;
        return *this;
    }

};

// Builder API for sg_image_desc
class image_desc : public helper::desc<sg_image_desc> {
public:
    image_desc() = default;

    image_desc& type(sg_image_type value) {
        desc_.type = value;
        return *this;
    }

    image_desc& usage_storage_image(bool value) {
        desc_.usage.storage_image = value;
        return *this;
    }

    image_desc& usage_color_attachment(bool value) {
        desc_.usage.color_attachment = value;
        return *this;
    }

    image_desc& usage_resolve_attachment(bool value) {
        desc_.usage.resolve_attachment = value;
        return *this;
    }

    image_desc& usage_depth_stencil_attachment(bool value) {
        desc_.usage.depth_stencil_attachment = value;
        return *this;
    }

    image_desc& usage_immutable(bool value) {
        desc_.usage.immutable = value;
        return *this;
    }

    image_desc& usage_dynamic_update(bool value) {
        desc_.usage.dynamic_update = value;
        return *this;
    }

    image_desc& usage_stream_update(bool value) {
        desc_.usage.stream_update = value;
        return *this;
    }

    image_desc& width(int value) {
        desc_.width = value;
        return *this;
    }

    image_desc& height(int value) {
        desc_.height = value;
        return *this;
    }

    image_desc& num_slices(int value) {
        desc_.num_slices = value;
        return *this;
    }

    image_desc& num_mipmaps(int value) {
        desc_.num_mipmaps = value;
        return *this;
    }

    image_desc& pixel_format(sg_pixel_format value) {
        desc_.pixel_format = value;
        return *this;
    }

    image_desc& sample_count(int value) {
        desc_.sample_count = value;
        return *this;
    }

    image_desc& data_mip_level_ptr(int index, const void* value) {
        desc_.data.mip_levels[index].ptr = value;
        return *this;
    }

    image_desc& data_mip_level_size(int index, size_t value) {
        desc_.data.mip_levels[index].size = value;
        return *this;
    }

    image_desc& label(const char* value) {
        desc_.label = value;
        return *this;
    }

};

// Builder API for sg_sampler_desc
class sampler_desc : public helper::desc<sg_sampler_desc> {
public:
    sampler_desc() = default;

    sampler_desc& min_filter(sg_filter value) {
        desc_.min_filter = value;
        return *this;
    }

    sampler_desc& mag_filter(sg_filter value) {
        desc_.mag_filter = value;
        return *this;
    }

    sampler_desc& mipmap_filter(sg_filter value) {
        desc_.mipmap_filter = value;
        return *this;
    }

    sampler_desc& wrap_u(sg_wrap value) {
        desc_.wrap_u = value;
        return *this;
    }

    sampler_desc& wrap_v(sg_wrap value) {
        desc_.wrap_v = value;
        return *this;
    }

    sampler_desc& wrap_w(sg_wrap value) {
        desc_.wrap_w = value;
        return *this;
    }

    sampler_desc& min_lod(float value) {
        desc_.min_lod = value;
        return *this;
    }

    sampler_desc& max_lod(float value) {
        desc_.max_lod = value;
        return *this;
    }

    sampler_desc& border_color(sg_border_color value) {
        desc_.border_color = value;
        return *this;
    }

    sampler_desc& compare(sg_compare_func value) {
        desc_.compare = value;
        return *this;
    }

    sampler_desc& max_anisotropy(uint32_t value) {
        desc_.max_anisotropy = value;
        return *this;
    }

    sampler_desc& label(const char* value) {
        desc_.label = value;
        return *this;
    }

};

// Builder API for sg_shader_desc
class shader_desc : public helper::desc<sg_shader_desc> {
public:
    shader_desc() = default;

    shader_desc& vertex_func_source(const char* value) {
        desc_.vertex_func.source = value;
        return *this;
    }

    shader_desc& vertex_func_bytecode_ptr(const void* value) {
        desc_.vertex_func.bytecode.ptr = value;
        return *this;
    }

    shader_desc& vertex_func_bytecode_size(size_t value) {
        desc_.vertex_func.bytecode.size = value;
        return *this;
    }

    shader_desc& vertex_func_entry(const char* value) {
        desc_.vertex_func.entry = value;
        return *this;
    }

    shader_desc& fragment_func_source(const char* value) {
        desc_.fragment_func.source = value;
        return *this;
    }

    shader_desc& fragment_func_bytecode_ptr(const void* value) {
        desc_.fragment_func.bytecode.ptr = value;
        return *this;
    }

    shader_desc& fragment_func_bytecode_size(size_t value) {
        desc_.fragment_func.bytecode.size = value;
        return *this;
    }

    shader_desc& fragment_func_entry(const char* value) {
        desc_.fragment_func.entry = value;
        return *this;
    }

    shader_desc& compute_func_source(const char* value) {
        desc_.compute_func.source = value;
        return *this;
    }

    shader_desc& compute_func_bytecode_ptr(const void* value) {
        desc_.compute_func.bytecode.ptr = value;
        return *this;
    }

    shader_desc& compute_func_bytecode_size(size_t value) {
        desc_.compute_func.bytecode.size = value;
        return *this;
    }

    shader_desc& compute_func_entry(const char* value) {
        desc_.compute_func.entry = value;
        return *this;
    }

    shader_desc& attr_base_type(int index, sg_shader_attr_base_type value) {
        desc_.attrs[index].base_type = value;
        return *this;
    }

    shader_desc& attr_glsl_name(int index, const char* value) {
        desc_.attrs[index].glsl_name = value;
        return *this;
    }

    shader_desc& attr_hlsl_sem_name(int index, const char* value) {
        desc_.attrs[index].hlsl_sem_name = value;
        return *this;
    }

    shader_desc& attr_hlsl_sem_index(int index, uint8_t value) {
        desc_.attrs[index].hlsl_sem_index = value;
        return *this;
    }

    shader_desc& uniform_block_stage(int index, sg_shader_stage value) {
        desc_.uniform_blocks[index].stage = value;
        return *this;
    }

    shader_desc& uniform_block_size(int index, uint32_t value) {
        desc_.uniform_blocks[index].size = value;
        return *this;
    }

    shader_desc& uniform_block_hlsl_register_b_n(int index, uint8_t value) {
        desc_.uniform_blocks[index].hlsl_register_b_n = value;
        return *this;
    }

    shader_desc& uniform_block_msl_buffer_n(int index, uint8_t value) {
        desc_.uniform_blocks[index].msl_buffer_n = value;
        return *this;
    }

    shader_desc& uniform_block_wgsl_group0_binding_n(int index, uint8_t value) {
        desc_.uniform_blocks[index].wgsl_group0_binding_n = value;
        return *this;
    }

    shader_desc& uniform_block_spirv_set0_binding_n(int index, uint8_t value) {
        desc_.uniform_blocks[index].spirv_set0_binding_n = value;
        return *this;
    }

    shader_desc& uniform_block_layout(int index, sg_uniform_layout value) {
        desc_.uniform_blocks[index].layout = value;
        return *this;
    }

    shader_desc& uniform_block_glsl_uniform_type(int index, sg_uniform_type value) {
        desc_.uniform_blocks[index].glsl_uniforms[index].type = value;
        return *this;
    }

    shader_desc& uniform_block_glsl_uniform_array_count(int index, uint16_t value) {
        desc_.uniform_blocks[index].glsl_uniforms[index].array_count = value;
        return *this;
    }

    shader_desc& uniform_block_glsl_uniform_glsl_name(int index, const char* value) {
        desc_.uniform_blocks[index].glsl_uniforms[index].glsl_name = value;
        return *this;
    }

    shader_desc& view_texture_stage(int index, sg_shader_stage value) {
        desc_.views[index].texture.stage = value;
        return *this;
    }

    shader_desc& view_texture_image_type(int index, sg_image_type value) {
        desc_.views[index].texture.image_type = value;
        return *this;
    }

    shader_desc& view_texture_sample_type(int index, sg_image_sample_type value) {
        desc_.views[index].texture.sample_type = value;
        return *this;
    }

    shader_desc& view_texture_multisampled(int index, bool value) {
        desc_.views[index].texture.multisampled = value;
        return *this;
    }

    shader_desc& view_texture_hlsl_register_t_n(int index, uint8_t value) {
        desc_.views[index].texture.hlsl_register_t_n = value;
        return *this;
    }

    shader_desc& view_texture_msl_texture_n(int index, uint8_t value) {
        desc_.views[index].texture.msl_texture_n = value;
        return *this;
    }

    shader_desc& view_texture_wgsl_group1_binding_n(int index, uint8_t value) {
        desc_.views[index].texture.wgsl_group1_binding_n = value;
        return *this;
    }

    shader_desc& view_texture_spirv_set1_binding_n(int index, uint8_t value) {
        desc_.views[index].texture.spirv_set1_binding_n = value;
        return *this;
    }

    shader_desc& view_storage_buffer_stage(int index, sg_shader_stage value) {
        desc_.views[index].storage_buffer.stage = value;
        return *this;
    }

    shader_desc& view_storage_buffer_readonly(int index, bool value) {
        desc_.views[index].storage_buffer.readonly = value;
        return *this;
    }

    shader_desc& view_storage_buffer_hlsl_register_t_n(int index, uint8_t value) {
        desc_.views[index].storage_buffer.hlsl_register_t_n = value;
        return *this;
    }

    shader_desc& view_storage_buffer_hlsl_register_u_n(int index, uint8_t value) {
        desc_.views[index].storage_buffer.hlsl_register_u_n = value;
        return *this;
    }

    shader_desc& view_storage_buffer_msl_buffer_n(int index, uint8_t value) {
        desc_.views[index].storage_buffer.msl_buffer_n = value;
        return *this;
    }

    shader_desc& view_storage_buffer_wgsl_group1_binding_n(int index, uint8_t value) {
        desc_.views[index].storage_buffer.wgsl_group1_binding_n = value;
        return *this;
    }

    shader_desc& view_storage_buffer_spirv_set1_binding_n(int index, uint8_t value) {
        desc_.views[index].storage_buffer.spirv_set1_binding_n = value;
        return *this;
    }

    shader_desc& view_storage_buffer_glsl_binding_n(int index, uint8_t value) {
        desc_.views[index].storage_buffer.glsl_binding_n = value;
        return *this;
    }

    shader_desc& view_storage_image_stage(int index, sg_shader_stage value) {
        desc_.views[index].storage_image.stage = value;
        return *this;
    }

    shader_desc& view_storage_image_image_type(int index, sg_image_type value) {
        desc_.views[index].storage_image.image_type = value;
        return *this;
    }

    shader_desc& view_storage_image_access_format(int index, sg_pixel_format value) {
        desc_.views[index].storage_image.access_format = value;
        return *this;
    }

    shader_desc& view_storage_image_writeonly(int index, bool value) {
        desc_.views[index].storage_image.writeonly = value;
        return *this;
    }

    shader_desc& view_storage_image_hlsl_register_u_n(int index, uint8_t value) {
        desc_.views[index].storage_image.hlsl_register_u_n = value;
        return *this;
    }

    shader_desc& view_storage_image_msl_texture_n(int index, uint8_t value) {
        desc_.views[index].storage_image.msl_texture_n = value;
        return *this;
    }

    shader_desc& view_storage_image_wgsl_group1_binding_n(int index, uint8_t value) {
        desc_.views[index].storage_image.wgsl_group1_binding_n = value;
        return *this;
    }

    shader_desc& view_storage_image_spirv_set1_binding_n(int index, uint8_t value) {
        desc_.views[index].storage_image.spirv_set1_binding_n = value;
        return *this;
    }

    shader_desc& view_storage_image_glsl_binding_n(int index, uint8_t value) {
        desc_.views[index].storage_image.glsl_binding_n = value;
        return *this;
    }

    shader_desc& sampler_stage(int index, sg_shader_stage value) {
        desc_.samplers[index].stage = value;
        return *this;
    }

    shader_desc& sampler_sampler_type(int index, sg_sampler_type value) {
        desc_.samplers[index].sampler_type = value;
        return *this;
    }

    shader_desc& sampler_hlsl_register_s_n(int index, uint8_t value) {
        desc_.samplers[index].hlsl_register_s_n = value;
        return *this;
    }

    shader_desc& sampler_msl_sampler_n(int index, uint8_t value) {
        desc_.samplers[index].msl_sampler_n = value;
        return *this;
    }

    shader_desc& sampler_wgsl_group1_binding_n(int index, uint8_t value) {
        desc_.samplers[index].wgsl_group1_binding_n = value;
        return *this;
    }

    shader_desc& sampler_spirv_set1_binding_n(int index, uint8_t value) {
        desc_.samplers[index].spirv_set1_binding_n = value;
        return *this;
    }

    shader_desc& texture_sampler_pair_stage(int index, sg_shader_stage value) {
        desc_.texture_sampler_pairs[index].stage = value;
        return *this;
    }

    shader_desc& texture_sampler_pair_view_slot(int index, uint8_t value) {
        desc_.texture_sampler_pairs[index].view_slot = value;
        return *this;
    }

    shader_desc& texture_sampler_pair_sampler_slot(int index, uint8_t value) {
        desc_.texture_sampler_pairs[index].sampler_slot = value;
        return *this;
    }

    shader_desc& texture_sampler_pair_glsl_name(int index, const char* value) {
        desc_.texture_sampler_pairs[index].glsl_name = value;
        return *this;
    }

    shader_desc& label(const char* value) {
        desc_.label = value;
        return *this;
    }

};

// Builder API for sg_pipeline_desc
class pipeline_desc : public helper::desc<sg_pipeline_desc> {
public:
    pipeline_desc() = default;

    pipeline_desc& compute(bool value) {
        desc_.compute = value;
        return *this;
    }

    pipeline_desc& shader_id(uint32_t value) {
        desc_.shader.id = value;
        return *this;
    }

    pipeline_desc& layout_buffer_stride(int index, int value) {
        desc_.layout.buffers[index].stride = value;
        return *this;
    }

    pipeline_desc& layout_buffer_step_func(int index, sg_vertex_step value) {
        desc_.layout.buffers[index].step_func = value;
        return *this;
    }

    pipeline_desc& layout_buffer_step_rate(int index, int value) {
        desc_.layout.buffers[index].step_rate = value;
        return *this;
    }

    pipeline_desc& layout_attr_buffer_index(int index, int value) {
        desc_.layout.attrs[index].buffer_index = value;
        return *this;
    }

    pipeline_desc& layout_attr_offset(int index, int value) {
        desc_.layout.attrs[index].offset = value;
        return *this;
    }

    pipeline_desc& layout_attr_format(int index, sg_vertex_format value) {
        desc_.layout.attrs[index].format = value;
        return *this;
    }

    pipeline_desc& depth_pixel_format(sg_pixel_format value) {
        desc_.depth.pixel_format = value;
        return *this;
    }

    pipeline_desc& depth_compare(sg_compare_func value) {
        desc_.depth.compare = value;
        return *this;
    }

    pipeline_desc& depth_write_enabled(bool value) {
        desc_.depth.write_enabled = value;
        return *this;
    }

    pipeline_desc& depth_bias(float value) {
        desc_.depth.bias = value;
        return *this;
    }

    pipeline_desc& depth_bias_slope_scale(float value) {
        desc_.depth.bias_slope_scale = value;
        return *this;
    }

    pipeline_desc& depth_bias_clamp(float value) {
        desc_.depth.bias_clamp = value;
        return *this;
    }

    pipeline_desc& stencil_enabled(bool value) {
        desc_.stencil.enabled = value;
        return *this;
    }

    pipeline_desc& stencil_front_compare(sg_compare_func value) {
        desc_.stencil.front.compare = value;
        return *this;
    }

    pipeline_desc& stencil_front_fail_op(sg_stencil_op value) {
        desc_.stencil.front.fail_op = value;
        return *this;
    }

    pipeline_desc& stencil_front_depth_fail_op(sg_stencil_op value) {
        desc_.stencil.front.depth_fail_op = value;
        return *this;
    }

    pipeline_desc& stencil_front_pass_op(sg_stencil_op value) {
        desc_.stencil.front.pass_op = value;
        return *this;
    }

    pipeline_desc& stencil_back_compare(sg_compare_func value) {
        desc_.stencil.back.compare = value;
        return *this;
    }

    pipeline_desc& stencil_back_fail_op(sg_stencil_op value) {
        desc_.stencil.back.fail_op = value;
        return *this;
    }

    pipeline_desc& stencil_back_depth_fail_op(sg_stencil_op value) {
        desc_.stencil.back.depth_fail_op = value;
        return *this;
    }

    pipeline_desc& stencil_back_pass_op(sg_stencil_op value) {
        desc_.stencil.back.pass_op = value;
        return *this;
    }

    pipeline_desc& stencil_read_mask(uint8_t value) {
        desc_.stencil.read_mask = value;
        return *this;
    }

    pipeline_desc& stencil_write_mask(uint8_t value) {
        desc_.stencil.write_mask = value;
        return *this;
    }

    pipeline_desc& stencil_ref(uint8_t value) {
        desc_.stencil.ref = value;
        return *this;
    }

    pipeline_desc& color_count(int value) {
        desc_.color_count = value;
        return *this;
    }

    pipeline_desc& color_pixel_format(int index, sg_pixel_format value) {
        desc_.colors[index].pixel_format = value;
        return *this;
    }

    pipeline_desc& color_write_mask(int index, sg_color_mask value) {
        desc_.colors[index].write_mask = value;
        return *this;
    }

    pipeline_desc& color_blend_enabled(int index, bool value) {
        desc_.colors[index].blend.enabled = value;
        return *this;
    }

    pipeline_desc& color_blend_src_factor_rgb(int index, sg_blend_factor value) {
        desc_.colors[index].blend.src_factor_rgb = value;
        return *this;
    }

    pipeline_desc& color_blend_dst_factor_rgb(int index, sg_blend_factor value) {
        desc_.colors[index].blend.dst_factor_rgb = value;
        return *this;
    }

    pipeline_desc& color_blend_op_rgb(int index, sg_blend_op value) {
        desc_.colors[index].blend.op_rgb = value;
        return *this;
    }

    pipeline_desc& color_blend_src_factor_alpha(int index, sg_blend_factor value) {
        desc_.colors[index].blend.src_factor_alpha = value;
        return *this;
    }

    pipeline_desc& color_blend_dst_factor_alpha(int index, sg_blend_factor value) {
        desc_.colors[index].blend.dst_factor_alpha = value;
        return *this;
    }

    pipeline_desc& color_blend_op_alpha(int index, sg_blend_op value) {
        desc_.colors[index].blend.op_alpha = value;
        return *this;
    }

    pipeline_desc& primitive_type(sg_primitive_type value) {
        desc_.primitive_type = value;
        return *this;
    }

    pipeline_desc& index_type(sg_index_type value) {
        desc_.index_type = value;
        return *this;
    }

    pipeline_desc& cull_mode(sg_cull_mode value) {
        desc_.cull_mode = value;
        return *this;
    }

    pipeline_desc& face_winding(sg_face_winding value) {
        desc_.face_winding = value;
        return *this;
    }

    pipeline_desc& sample_count(int value) {
        desc_.sample_count = value;
        return *this;
    }

    pipeline_desc& blend_color_r(float value) {
        desc_.blend_color.r = value;
        return *this;
    }

    pipeline_desc& blend_color_g(float value) {
        desc_.blend_color.g = value;
        return *this;
    }

    pipeline_desc& blend_color_b(float value) {
        desc_.blend_color.b = value;
        return *this;
    }

    pipeline_desc& blend_color_a(float value) {
        desc_.blend_color.a = value;
        return *this;
    }

    pipeline_desc& alpha_to_coverage_enabled(bool value) {
        desc_.alpha_to_coverage_enabled = value;
        return *this;
    }

    pipeline_desc& label(const char* value) {
        desc_.label = value;
        return *this;
    }

};

// Builder API for sg_buffer_view_desc
class buffer_view_desc : public helper::desc<sg_buffer_view_desc> {
public:
    buffer_view_desc() = default;

    buffer_view_desc& buffer_id(uint32_t value) {
        desc_.buffer.id = value;
        return *this;
    }

    buffer_view_desc& offset(int value) {
        desc_.offset = value;
        return *this;
    }

};

// Builder API for sg_image_view_desc
class image_view_desc : public helper::desc<sg_image_view_desc> {
public:
    image_view_desc() = default;

    image_view_desc& image_id(uint32_t value) {
        desc_.image.id = value;
        return *this;
    }

    image_view_desc& mip_level(int value) {
        desc_.mip_level = value;
        return *this;
    }

    image_view_desc& slice(int value) {
        desc_.slice = value;
        return *this;
    }

};

// Builder API for sg_texture_view_desc
class texture_view_desc : public helper::desc<sg_texture_view_desc> {
public:
    texture_view_desc() = default;

    texture_view_desc& image_id(uint32_t value) {
        desc_.image.id = value;
        return *this;
    }

    texture_view_desc& mip_levels_base(int value) {
        desc_.mip_levels.base = value;
        return *this;
    }

    texture_view_desc& mip_levels_count(int value) {
        desc_.mip_levels.count = value;
        return *this;
    }

    texture_view_desc& slices_base(int value) {
        desc_.slices.base = value;
        return *this;
    }

    texture_view_desc& slices_count(int value) {
        desc_.slices.count = value;
        return *this;
    }

};

// Builder API for sg_view_desc
class view_desc : public helper::desc<sg_view_desc> {
public:
    view_desc() = default;

    view_desc& texture_image_id(uint32_t value) {
        desc_.texture.image.id = value;
        return *this;
    }

    view_desc& texture_mip_levels_base(int value) {
        desc_.texture.mip_levels.base = value;
        return *this;
    }

    view_desc& texture_mip_levels_count(int value) {
        desc_.texture.mip_levels.count = value;
        return *this;
    }

    view_desc& texture_slices_base(int value) {
        desc_.texture.slices.base = value;
        return *this;
    }

    view_desc& texture_slices_count(int value) {
        desc_.texture.slices.count = value;
        return *this;
    }

    view_desc& storage_buffer_buffer_id(uint32_t value) {
        desc_.storage_buffer.buffer.id = value;
        return *this;
    }

    view_desc& storage_buffer_offset(int value) {
        desc_.storage_buffer.offset = value;
        return *this;
    }

    view_desc& storage_image_image_id(uint32_t value) {
        desc_.storage_image.image.id = value;
        return *this;
    }

    view_desc& storage_image_mip_level(int value) {
        desc_.storage_image.mip_level = value;
        return *this;
    }

    view_desc& storage_image_slice(int value) {
        desc_.storage_image.slice = value;
        return *this;
    }

    view_desc& color_attachment_image_id(uint32_t value) {
        desc_.color_attachment.image.id = value;
        return *this;
    }

    view_desc& color_attachment_mip_level(int value) {
        desc_.color_attachment.mip_level = value;
        return *this;
    }

    view_desc& color_attachment_slice(int value) {
        desc_.color_attachment.slice = value;
        return *this;
    }

    view_desc& resolve_attachment_image_id(uint32_t value) {
        desc_.resolve_attachment.image.id = value;
        return *this;
    }

    view_desc& resolve_attachment_mip_level(int value) {
        desc_.resolve_attachment.mip_level = value;
        return *this;
    }

    view_desc& resolve_attachment_slice(int value) {
        desc_.resolve_attachment.slice = value;
        return *this;
    }

    view_desc& depth_stencil_attachment_image_id(uint32_t value) {
        desc_.depth_stencil_attachment.image.id = value;
        return *this;
    }

    view_desc& depth_stencil_attachment_mip_level(int value) {
        desc_.depth_stencil_attachment.mip_level = value;
        return *this;
    }

    view_desc& depth_stencil_attachment_slice(int value) {
        desc_.depth_stencil_attachment.slice = value;
        return *this;
    }

    view_desc& label(const char* value) {
        desc_.label = value;
        return *this;
    }

};

// Builder API for sg_d3d11_desc
class d3d11_desc : public helper::desc<sg_d3d11_desc> {
public:
    d3d11_desc() = default;

    d3d11_desc& shader_debugging(bool value) {
        desc_.shader_debugging = value;
        return *this;
    }

};

// Builder API for sg_metal_desc
class metal_desc : public helper::desc<sg_metal_desc> {
public:
    metal_desc() = default;

    metal_desc& force_managed_storage_mode(bool value) {
        desc_.force_managed_storage_mode = value;
        return *this;
    }

    metal_desc& use_command_buffer_with_retained_references(bool value) {
        desc_.use_command_buffer_with_retained_references = value;
        return *this;
    }

};

// Builder API for sg_wgpu_desc
class wgpu_desc : public helper::desc<sg_wgpu_desc> {
public:
    wgpu_desc() = default;

    wgpu_desc& disable_bindgroups_cache(bool value) {
        desc_.disable_bindgroups_cache = value;
        return *this;
    }

    wgpu_desc& bindgroups_cache_size(int value) {
        desc_.bindgroups_cache_size = value;
        return *this;
    }

};

// Builder API for sg_vulkan_desc
class vulkan_desc : public helper::desc<sg_vulkan_desc> {
public:
    vulkan_desc() = default;

    vulkan_desc& copy_staging_buffer_size(int value) {
        desc_.copy_staging_buffer_size = value;
        return *this;
    }

    vulkan_desc& stream_staging_buffer_size(int value) {
        desc_.stream_staging_buffer_size = value;
        return *this;
    }

    vulkan_desc& descriptor_buffer_size(int value) {
        desc_.descriptor_buffer_size = value;
        return *this;
    }

};

// Builder API for sg_desc
class desc : public helper::desc<sg_desc> {
public:
    desc() = default;

    desc& buffer_pool_size(int value) {
        desc_.buffer_pool_size = value;
        return *this;
    }

    desc& image_pool_size(int value) {
        desc_.image_pool_size = value;
        return *this;
    }

    desc& sampler_pool_size(int value) {
        desc_.sampler_pool_size = value;
        return *this;
    }

    desc& shader_pool_size(int value) {
        desc_.shader_pool_size = value;
        return *this;
    }

    desc& pipeline_pool_size(int value) {
        desc_.pipeline_pool_size = value;
        return *this;
    }

    desc& view_pool_size(int value) {
        desc_.view_pool_size = value;
        return *this;
    }

    desc& uniform_buffer_size(int value) {
        desc_.uniform_buffer_size = value;
        return *this;
    }

    desc& max_commit_listeners(int value) {
        desc_.max_commit_listeners = value;
        return *this;
    }

    desc& disable_validation(bool value) {
        desc_.disable_validation = value;
        return *this;
    }

    desc& enforce_portable_limits(bool value) {
        desc_.enforce_portable_limits = value;
        return *this;
    }

    desc& d3d11_shader_debugging(bool value) {
        desc_.d3d11.shader_debugging = value;
        return *this;
    }

    desc& metal_force_managed_storage_mode(bool value) {
        desc_.metal.force_managed_storage_mode = value;
        return *this;
    }

    desc& metal_use_command_buffer_with_retained_references(bool value) {
        desc_.metal.use_command_buffer_with_retained_references = value;
        return *this;
    }

    desc& wgpu_disable_bindgroups_cache(bool value) {
        desc_.wgpu.disable_bindgroups_cache = value;
        return *this;
    }

    desc& wgpu_bindgroups_cache_size(int value) {
        desc_.wgpu.bindgroups_cache_size = value;
        return *this;
    }

    desc& vulkan_copy_staging_buffer_size(int value) {
        desc_.vulkan.copy_staging_buffer_size = value;
        return *this;
    }

    desc& vulkan_stream_staging_buffer_size(int value) {
        desc_.vulkan.stream_staging_buffer_size = value;
        return *this;
    }

    desc& vulkan_descriptor_buffer_size(int value) {
        desc_.vulkan.descriptor_buffer_size = value;
        return *this;
    }

    desc& allocator_alloc_fn(void *(*)(size_t, void *) value) {
        desc_.allocator.alloc_fn = value;
        return *this;
    }

    desc& allocator_free_fn(void (*)(void *, void *) value) {
        desc_.allocator.free_fn = value;
        return *this;
    }

    desc& allocator_user_data(void* value) {
        desc_.allocator.user_data = value;
        return *this;
    }

    desc& logger_func(void (*)(const char *, uint32_t, uint32_t, const char *, uint32_t, const char *, void *) value) {
        desc_.logger.func = value;
        return *this;
    }

    desc& logger_user_data(void* value) {
        desc_.logger.user_data = value;
        return *this;
    }

    desc& environment_defaults_color_format(sg_pixel_format value) {
        desc_.environment.defaults.color_format = value;
        return *this;
    }

    desc& environment_defaults_depth_format(sg_pixel_format value) {
        desc_.environment.defaults.depth_format = value;
        return *this;
    }

    desc& environment_defaults_sample_count(int value) {
        desc_.environment.defaults.sample_count = value;
        return *this;
    }

    desc& environment_metal_device(const void* value) {
        desc_.environment.metal.device = value;
        return *this;
    }

    desc& environment_d3d11_device(const void* value) {
        desc_.environment.d3d11.device = value;
        return *this;
    }

    desc& environment_d3d11_device_context(const void* value) {
        desc_.environment.d3d11.device_context = value;
        return *this;
    }

    desc& environment_wgpu_device(const void* value) {
        desc_.environment.wgpu.device = value;
        return *this;
    }

    desc& environment_vulkan_physical_device(const void* value) {
        desc_.environment.vulkan.physical_device = value;
        return *this;
    }

    desc& environment_vulkan_device(const void* value) {
        desc_.environment.vulkan.device = value;
        return *this;
    }

    desc& environment_vulkan_queue(const void* value) {
        desc_.environment.vulkan.queue = value;
        return *this;
    }

    desc& environment_vulkan_queue_family_index(uint32_t value) {
        desc_.environment.vulkan.queue_family_index = value;
        return *this;
    }

};

} // namespace sg
#endif // SOKOL_NO_SG

#ifndef SOKOL_APP_INCLUDED
#if __has_include("sokol_app.h")
#include "sokol_app.h"
#elif __has_include("sokol/sokol_app.h")
#include "sokol/sokol_app.h"
#else
#warning "Cannot find sokol_app.h, disabling!"
#define SOKOL_NO_SAPP
#endif
#endif

#ifndef SOKOL_NO_SAPP
namespace sapp {
// Builder API for sapp_image_desc
class image_desc : public helper::desc<sapp_image_desc> {
public:
    image_desc() = default;

    image_desc& width(int value) {
        desc_.width = value;
        return *this;
    }

    image_desc& height(int value) {
        desc_.height = value;
        return *this;
    }

    image_desc& cursor_hotspot_x(int value) {
        desc_.cursor_hotspot_x = value;
        return *this;
    }

    image_desc& cursor_hotspot_y(int value) {
        desc_.cursor_hotspot_y = value;
        return *this;
    }

    image_desc& pixels_ptr(const void* value) {
        desc_.pixels.ptr = value;
        return *this;
    }

    image_desc& pixels_size(size_t value) {
        desc_.pixels.size = value;
        return *this;
    }

};

// Builder API for sapp_icon_desc
class icon_desc : public helper::desc<sapp_icon_desc> {
public:
    icon_desc() = default;

    icon_desc& sokol_default(bool value) {
        desc_.sokol_default = value;
        return *this;
    }

    icon_desc& image_width(int index, int value) {
        desc_.images[index].width = value;
        return *this;
    }

    icon_desc& image_height(int index, int value) {
        desc_.images[index].height = value;
        return *this;
    }

    icon_desc& image_cursor_hotspot_x(int index, int value) {
        desc_.images[index].cursor_hotspot_x = value;
        return *this;
    }

    icon_desc& image_cursor_hotspot_y(int index, int value) {
        desc_.images[index].cursor_hotspot_y = value;
        return *this;
    }

    icon_desc& image_pixels_ptr(int index, const void* value) {
        desc_.images[index].pixels.ptr = value;
        return *this;
    }

    icon_desc& image_pixels_size(int index, size_t value) {
        desc_.images[index].pixels.size = value;
        return *this;
    }

};

// Builder API for sapp_gl_desc
class gl_desc : public helper::desc<sapp_gl_desc> {
public:
    gl_desc() = default;

    gl_desc& major_version(int value) {
        desc_.major_version = value;
        return *this;
    }

    gl_desc& minor_version(int value) {
        desc_.minor_version = value;
        return *this;
    }

};

// Builder API for sapp_win32_desc
class win32_desc : public helper::desc<sapp_win32_desc> {
public:
    win32_desc() = default;

    win32_desc& console_utf8(bool value) {
        desc_.console_utf8 = value;
        return *this;
    }

    win32_desc& console_create(bool value) {
        desc_.console_create = value;
        return *this;
    }

    win32_desc& console_attach(bool value) {
        desc_.console_attach = value;
        return *this;
    }

};

// Builder API for sapp_html5_desc
class html5_desc : public helper::desc<sapp_html5_desc> {
public:
    html5_desc() = default;

    html5_desc& canvas_selector(const char* value) {
        desc_.canvas_selector = value;
        return *this;
    }

    html5_desc& canvas_resize(bool value) {
        desc_.canvas_resize = value;
        return *this;
    }

    html5_desc& preserve_drawing_buffer(bool value) {
        desc_.preserve_drawing_buffer = value;
        return *this;
    }

    html5_desc& premultiplied_alpha(bool value) {
        desc_.premultiplied_alpha = value;
        return *this;
    }

    html5_desc& ask_leave_site(bool value) {
        desc_.ask_leave_site = value;
        return *this;
    }

    html5_desc& update_document_title(bool value) {
        desc_.update_document_title = value;
        return *this;
    }

    html5_desc& bubble_mouse_events(bool value) {
        desc_.bubble_mouse_events = value;
        return *this;
    }

    html5_desc& bubble_touch_events(bool value) {
        desc_.bubble_touch_events = value;
        return *this;
    }

    html5_desc& bubble_wheel_events(bool value) {
        desc_.bubble_wheel_events = value;
        return *this;
    }

    html5_desc& bubble_key_events(bool value) {
        desc_.bubble_key_events = value;
        return *this;
    }

    html5_desc& bubble_char_events(bool value) {
        desc_.bubble_char_events = value;
        return *this;
    }

    html5_desc& use_emsc_set_main_loop(bool value) {
        desc_.use_emsc_set_main_loop = value;
        return *this;
    }

    html5_desc& emsc_set_main_loop_simulate_infinite_loop(bool value) {
        desc_.emsc_set_main_loop_simulate_infinite_loop = value;
        return *this;
    }

};

// Builder API for sapp_ios_desc
class ios_desc : public helper::desc<sapp_ios_desc> {
public:
    ios_desc() = default;

    ios_desc& keyboard_resizes_canvas(bool value) {
        desc_.keyboard_resizes_canvas = value;
        return *this;
    }

};

// Builder API for sapp_desc
class desc : public helper::desc<sapp_desc> {
public:
    desc() = default;

    desc& init_cb(void (*)(void) value) {
        desc_.init_cb = value;
        return *this;
    }

    desc& frame_cb(void (*)(void) value) {
        desc_.frame_cb = value;
        return *this;
    }

    desc& cleanup_cb(void (*)(void) value) {
        desc_.cleanup_cb = value;
        return *this;
    }

    desc& event_cb(void (*)(const sapp_event *) value) {
        desc_.event_cb = value;
        return *this;
    }

    desc& user_data(void* value) {
        desc_.user_data = value;
        return *this;
    }

    desc& init_userdata_cb(void (*)(void *) value) {
        desc_.init_userdata_cb = value;
        return *this;
    }

    desc& frame_userdata_cb(void (*)(void *) value) {
        desc_.frame_userdata_cb = value;
        return *this;
    }

    desc& cleanup_userdata_cb(void (*)(void *) value) {
        desc_.cleanup_userdata_cb = value;
        return *this;
    }

    desc& event_userdata_cb(void (*)(const sapp_event *, void *) value) {
        desc_.event_userdata_cb = value;
        return *this;
    }

    desc& width(int value) {
        desc_.width = value;
        return *this;
    }

    desc& height(int value) {
        desc_.height = value;
        return *this;
    }

    desc& sample_count(int value) {
        desc_.sample_count = value;
        return *this;
    }

    desc& swap_interval(int value) {
        desc_.swap_interval = value;
        return *this;
    }

    desc& high_dpi(bool value) {
        desc_.high_dpi = value;
        return *this;
    }

    desc& fullscreen(bool value) {
        desc_.fullscreen = value;
        return *this;
    }

    desc& alpha(bool value) {
        desc_.alpha = value;
        return *this;
    }

    desc& window_title(const char* value) {
        desc_.window_title = value;
        return *this;
    }

    desc& enable_clipboard(bool value) {
        desc_.enable_clipboard = value;
        return *this;
    }

    desc& clipboard_size(int value) {
        desc_.clipboard_size = value;
        return *this;
    }

    desc& enable_dragndrop(bool value) {
        desc_.enable_dragndrop = value;
        return *this;
    }

    desc& max_dropped_files(int value) {
        desc_.max_dropped_files = value;
        return *this;
    }

    desc& max_dropped_file_path_length(int value) {
        desc_.max_dropped_file_path_length = value;
        return *this;
    }

    desc& icon_sokol_default(bool value) {
        desc_.icon.sokol_default = value;
        return *this;
    }

    desc& icon_image_width(int index, int value) {
        desc_.icon.images[index].width = value;
        return *this;
    }

    desc& icon_image_height(int index, int value) {
        desc_.icon.images[index].height = value;
        return *this;
    }

    desc& icon_image_cursor_hotspot_x(int index, int value) {
        desc_.icon.images[index].cursor_hotspot_x = value;
        return *this;
    }

    desc& icon_image_cursor_hotspot_y(int index, int value) {
        desc_.icon.images[index].cursor_hotspot_y = value;
        return *this;
    }

    desc& icon_image_pixels_ptr(int index, const void* value) {
        desc_.icon.images[index].pixels.ptr = value;
        return *this;
    }

    desc& icon_image_pixels_size(int index, size_t value) {
        desc_.icon.images[index].pixels.size = value;
        return *this;
    }

    desc& allocator_alloc_fn(void *(*)(size_t, void *) value) {
        desc_.allocator.alloc_fn = value;
        return *this;
    }

    desc& allocator_free_fn(void (*)(void *, void *) value) {
        desc_.allocator.free_fn = value;
        return *this;
    }

    desc& allocator_user_data(void* value) {
        desc_.allocator.user_data = value;
        return *this;
    }

    desc& logger_func(void (*)(const char *, uint32_t, uint32_t, const char *, uint32_t, const char *, void *) value) {
        desc_.logger.func = value;
        return *this;
    }

    desc& logger_user_data(void* value) {
        desc_.logger.user_data = value;
        return *this;
    }

    desc& gl_major_version(int value) {
        desc_.gl.major_version = value;
        return *this;
    }

    desc& gl_minor_version(int value) {
        desc_.gl.minor_version = value;
        return *this;
    }

    desc& win32_console_utf8(bool value) {
        desc_.win32.console_utf8 = value;
        return *this;
    }

    desc& win32_console_create(bool value) {
        desc_.win32.console_create = value;
        return *this;
    }

    desc& win32_console_attach(bool value) {
        desc_.win32.console_attach = value;
        return *this;
    }

    desc& html5_canvas_selector(const char* value) {
        desc_.html5.canvas_selector = value;
        return *this;
    }

    desc& html5_canvas_resize(bool value) {
        desc_.html5.canvas_resize = value;
        return *this;
    }

    desc& html5_preserve_drawing_buffer(bool value) {
        desc_.html5.preserve_drawing_buffer = value;
        return *this;
    }

    desc& html5_premultiplied_alpha(bool value) {
        desc_.html5.premultiplied_alpha = value;
        return *this;
    }

    desc& html5_ask_leave_site(bool value) {
        desc_.html5.ask_leave_site = value;
        return *this;
    }

    desc& html5_update_document_title(bool value) {
        desc_.html5.update_document_title = value;
        return *this;
    }

    desc& html5_bubble_mouse_events(bool value) {
        desc_.html5.bubble_mouse_events = value;
        return *this;
    }

    desc& html5_bubble_touch_events(bool value) {
        desc_.html5.bubble_touch_events = value;
        return *this;
    }

    desc& html5_bubble_wheel_events(bool value) {
        desc_.html5.bubble_wheel_events = value;
        return *this;
    }

    desc& html5_bubble_key_events(bool value) {
        desc_.html5.bubble_key_events = value;
        return *this;
    }

    desc& html5_bubble_char_events(bool value) {
        desc_.html5.bubble_char_events = value;
        return *this;
    }

    desc& html5_use_emsc_set_main_loop(bool value) {
        desc_.html5.use_emsc_set_main_loop = value;
        return *this;
    }

    desc& html5_emsc_set_main_loop_simulate_infinite_loop(bool value) {
        desc_.html5.emsc_set_main_loop_simulate_infinite_loop = value;
        return *this;
    }

    desc& ios_keyboard_resizes_canvas(bool value) {
        desc_.ios.keyboard_resizes_canvas = value;
        return *this;
    }

};

} // namespace sapp
#endif // SOKOL_NO_SAPP

#ifndef SOKOL_TIME_INCLUDED
#if __has_include("sokol_time.h")
#include "sokol_time.h"
#elif __has_include("sokol/sokol_time.h")
#include "sokol/sokol_time.h"
#else
#warning "Cannot find sokol_time.h, disabling!"
#define SOKOL_NO_STM
#endif
#endif

#ifndef SOKOL_NO_STM
namespace stm {
} // namespace stm
#endif // SOKOL_NO_STM

#ifndef SOKOL_AUDIO_INCLUDED
#if __has_include("sokol_audio.h")
#include "sokol_audio.h"
#elif __has_include("sokol/sokol_audio.h")
#include "sokol/sokol_audio.h"
#else
#warning "Cannot find sokol_audio.h, disabling!"
#define SOKOL_NO_SAUDIO
#endif
#endif

#ifndef SOKOL_NO_SAUDIO
namespace saudio {
// Builder API for saudio_n3ds_desc
class n3ds_desc : public helper::desc<saudio_n3ds_desc> {
public:
    n3ds_desc() = default;

    n3ds_desc& queue_count(int value) {
        desc_.queue_count = value;
        return *this;
    }

    n3ds_desc& interpolation_type(saudio_n3ds_ndspinterptype value) {
        desc_.interpolation_type = value;
        return *this;
    }

    n3ds_desc& channel_id(int value) {
        desc_.channel_id = value;
        return *this;
    }

};

// Builder API for saudio_desc
class desc : public helper::desc<saudio_desc> {
public:
    desc() = default;

    desc& sample_rate(int value) {
        desc_.sample_rate = value;
        return *this;
    }

    desc& num_channels(int value) {
        desc_.num_channels = value;
        return *this;
    }

    desc& buffer_frames(int value) {
        desc_.buffer_frames = value;
        return *this;
    }

    desc& packet_frames(int value) {
        desc_.packet_frames = value;
        return *this;
    }

    desc& num_packets(int value) {
        desc_.num_packets = value;
        return *this;
    }

    desc& stream_cb(void (*)(float *, int, int) value) {
        desc_.stream_cb = value;
        return *this;
    }

    desc& stream_userdata_cb(void (*)(float *, int, int, void *) value) {
        desc_.stream_userdata_cb = value;
        return *this;
    }

    desc& user_data(void* value) {
        desc_.user_data = value;
        return *this;
    }

    desc& n3ds_queue_count(int value) {
        desc_.n3ds.queue_count = value;
        return *this;
    }

    desc& n3ds_interpolation_type(saudio_n3ds_ndspinterptype value) {
        desc_.n3ds.interpolation_type = value;
        return *this;
    }

    desc& n3ds_channel_id(int value) {
        desc_.n3ds.channel_id = value;
        return *this;
    }

    desc& allocator_alloc_fn(void *(*)(size_t, void *) value) {
        desc_.allocator.alloc_fn = value;
        return *this;
    }

    desc& allocator_free_fn(void (*)(void *, void *) value) {
        desc_.allocator.free_fn = value;
        return *this;
    }

    desc& allocator_user_data(void* value) {
        desc_.allocator.user_data = value;
        return *this;
    }

    desc& logger_func(void (*)(const char *, uint32_t, uint32_t, const char *, uint32_t, const char *, void *) value) {
        desc_.logger.func = value;
        return *this;
    }

    desc& logger_user_data(void* value) {
        desc_.logger.user_data = value;
        return *this;
    }

};

} // namespace saudio
#endif // SOKOL_NO_SAUDIO

