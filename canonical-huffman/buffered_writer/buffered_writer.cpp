#include "buffered_writer.h"

buffered_writer::buffered_writer(std::ostream &stream) : writer(stream), end_index(0) {}

buffered_writer::~buffered_writer() { write_chunk(); }

void buffered_writer::write_chunk() {
        writer.write(buffer, static_cast<std::streamsize>(end_index));
        end_index = 0;
}
