#pragma once

#include "sld-xml-internal.cpp"

namespace sld {

    SLD_API const xml_hnd_doc_t
    xml_doc_create(
        void) {

        xml_doc_t* doc = xml_memory_alloc_doc();
        assert(doc);

        const alloc_hnd_t   hnd_mem = heap_alctr_get_hnd(_xml_mem.alctr,(const void*)doc);
        const xml_hnd_doc_t hnd_doc = {hnd_mem.val};
        assert(hnd_doc.val != 0);

        return(hnd_doc);
    }

    SLD_API void
    xml_doc_destroy(
        const xml_hnd_doc_t h_doc) {

        xml_doc_t* doc = (xml_doc_t*)xml_memory_get_ptr(h_doc); 
        xml_memory_free_doc(doc);
    }

    SLD_API bool
    xml_doc_buffer_read(
        const xml_hnd_doc_t h_doc,
        const buffer_t&  buffer) {
    
            xml_doc_t* doc = (xml_doc_t*)xml_memory_get_ptr(h_doc);

        return(NULL);
    }

    SLD_API bool
    xml_doc_buffer_write(
        const xml_hnd_doc_t h_doc,
        buffer_t&           buffer) {

        xml_doc_t* doc = (xml_doc_t*)xml_memory_get_ptr(h_doc);
        assert(doc);
        assert(buffer.data != NULL && buffer.size != 0);

        xml_writer_t writer;

        writer.buffer = buffer;

        doc->pugi.save(writer);

        buffer = writer.buffer;

        const bool did_write = (buffer.length != 0);
        return(did_write);
    }
    
    SLD_API u64
    xml_doc_buffer_size(
        const xml_hnd_doc_t h_doc) {

        xml_doc_t* doc = (xml_doc_t*)xml_memory_get_ptr(h_doc);
        assert(doc);

        xml_writer_t xml_writer;
        xml_writer.buffer.data   = NULL;
        xml_writer.buffer.length = 0;
        xml_writer.buffer.size   = 0;

        doc->pugi.save(xml_writer);

        const u64 size = xml_writer.buffer.size;
        return(size);
    }

    SLD_API const xml_hnd_node_t
    xml_doc_get_next_child_node(
        const xml_hnd_doc_t h_doc,
        const xml_utf8_t*   child_name) {
        
        xml_doc_t*  doc        = (xml_doc_t*)xml_memory_get_ptr(h_doc);
        xml_node_t* child_node = xml_memory_alloc_node(doc);

        bool is_valid = true;
        is_valid &= (doc        != NULL);
        is_valid &= (child_name != NULL);
        is_valid &= (child_node != NULL);
        assert(is_valid);

        child_node->pugi = doc->pugi.child(child_name); 


        const xml_hnd_t      hnd      = xml_memory_get_hnd((void*)child_node);        
        const xml_hnd_node_t hnd_node = {hnd.val};
        return(hnd_node);
    }

    SLD_API const xml_hnd_node_t
    xml_doc_add_child_node(
        const xml_hnd_doc_t h_doc,
        const c8*           child_name) {

        xml_doc_t*  doc = (xml_doc_t*)xml_memory_get_ptr(h_doc);
        xml_node_t* child_node = xml_memory_alloc_node(doc);

        assert(doc);
        assert(child_name);
        assert(child_node);

        child_node->pugi = doc->pugi.append_child(child_name);
        const xml_hnd_t      hnd      = xml_memory_get_hnd((void*)child_node);        
        const xml_hnd_node_t hnd_node = {hnd.val};
        return(hnd_node);
    }
};