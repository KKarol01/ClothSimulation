#pragma once

#include <array>
#include <unordered_map>
#include <vector>
#include <functional>
#include <initializer_list>

namespace ph {
    enum class BUFFERSTORAGE : bool { IMMUTABLE, MUTABLE };

    struct BufferConfig {
        uint32_t idx{0};
    };
    struct BufferAttribConfig {
        uint32_t idx{0}, binding_idx{0}, size{0}, type{0}, offset{0};
        bool normalize{0};
    };
    struct BufferBindingConfig {
        uint32_t binding_idx{0}, buffer_idx{0}, offset{0}, stride{0}, divisor{0};
    };
    struct BufferStorageConfig {
        using enum BUFFERSTORAGE;
        uint32_t buffer_id{0}, size{0};
        void *data{nullptr};
        uint32_t flags{0};
        BUFFERSTORAGE storage{IMMUTABLE};
    };
    struct BufferMapRangeConfig {
        uint32_t buffer_id{0};
        size_t from{0}, size{0};
        uint32_t flags{0};
    };

    class BufferArray {
      protected:
        uint32_t vao;
        std::unordered_map<uint32_t, uint32_t> _buffer_handles;

      public:
        BufferArray();
        void configure_binding(BufferBindingConfig config);
        void configure_attrib(BufferAttribConfig config);
        void configure_storage(BufferStorageConfig config);
        void configure_element_buffer(BufferStorageConfig config);

        void *map_buffer_range(BufferMapRangeConfig config);

        uint32_t get_buffer_handle(size_t idx) const;

        void bind() const;

        virtual ~BufferArray();
    };
} // namespace ph