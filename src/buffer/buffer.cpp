#include <glad/glad.h>
#include "buffer.hpp"
#include <stdexcept>

ph::BufferArray::BufferArray() { glCreateVertexArrays(1, &vao); }

void ph::BufferArray::configure_binding(BufferBindingConfig config) {

    if (!_buffer_handles.contains(config.buffer_idx)) { glCreateBuffers(1, &_buffer_handles[config.buffer_idx]); }

    glVertexArrayVertexBuffer(
        vao, config.binding_idx, _buffer_handles.at(config.buffer_idx), config.offset, config.stride);

    if (config.divisor != 0) { glVertexArrayBindingDivisor(vao, config.binding_idx, config.divisor); }
}

void ph::BufferArray::configure_attrib(BufferAttribConfig config) {
    glEnableVertexArrayAttrib(vao, config.idx);
    glVertexArrayAttribBinding(vao, config.idx, config.binding_idx);
    glVertexArrayAttribFormat(vao, config.idx, config.size, config.type, config.normalize, config.offset);
}

void ph::BufferArray::configure_storage(BufferStorageConfig config) {
    switch (config.storage) {
    case BUFFERSTORAGE::IMMUTABLE:
        glNamedBufferStorage(_buffer_handles.at(config.buffer_id), config.size, config.data, config.flags);
        break;
    case BUFFERSTORAGE::MUTABLE:
        glNamedBufferData(_buffer_handles.at(config.buffer_id), config.size, config.data, config.flags);
        break;
    }
}

void ph::BufferArray::configure_element_buffer(BufferStorageConfig config) {
    if (_buffer_handles.contains(config.buffer_id))
        throw std::runtime_error{
            "[BufferArray::configure_element_buffer()]: Buffer under that id has been already reserved."};

    glCreateBuffers(1, &_buffer_handles[config.buffer_id]);
    configure_storage(config);
    glVertexArrayElementBuffer(vao, _buffer_handles.at(config.buffer_id));
}

void *ph::BufferArray::map_buffer_range(BufferMapRangeConfig config) {
    return glMapNamedBufferRange(_buffer_handles.at(config.buffer_id), config.from, config.size, config.flags);
}

uint32_t ph::BufferArray::get_buffer_handle(size_t idx) const
{
    return _buffer_handles.at(idx);
}

void ph::BufferArray::bind() const { glBindVertexArray(vao); }

ph::BufferArray::~BufferArray() {
    glDeleteVertexArrays(1, &vao);
    for (auto &[_, h] : _buffer_handles) { glDeleteBuffers(1, &h); }
}
