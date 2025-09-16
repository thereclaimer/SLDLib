#pragma once

#include "sld-xml-internal.cpp"

namespace sld {

    SLD_API bool
    xml_doc_buffer_read(
        xml_doc_t* const doc,
        const buffer_t&  buffer) {
    
        return(NULL);
    }

    SLD_API bool
    xml_doc_buffer_write(
        xml_doc_t* const doc,
        buffer_t&        buffer) {

        xml_writer_t writer;

        writer.buffer = buffer;

        doc->pugi_doc.save(writer);

        buffer = writer.buffer;

        const bool did_write = (buffer.length != 0);
        return(did_write);
    }
    
    SLD_API u64
    xml_doc_buffer_size(
        xml_doc_t* const doc) {

        assert(doc);

        xml_writer_t xml_writer;
        xml_writer.buffer.data   = NULL;
        xml_writer.buffer.length = 0;
        xml_writer.buffer.size   = 0;

        doc->pugi_doc.save(xml_writer);

        const u64 size = xml_writer.buffer.size;
        return(size);
    }

    SLD_API bool
    xml_doc_get_next_child_node(
        xml_doc_t* const doc,
        const c8*        child_name,
        xml_node_t*      child_node) {

        return(false);
    }

    SLD_API bool
    xml_doc_add_child_node(
        xml_doc_t* const doc,
        const c8*        child_name,
        xml_node_t*      child_node) {

        assert(doc);
        assert(child_name);
        assert(child_node);

        child_node->pugi_node = doc->pugi_doc.append_child(child_name);
        
        const bool is_valid = (child_node->pugi_node != NULL); 
        return(is_valid);
    }
};