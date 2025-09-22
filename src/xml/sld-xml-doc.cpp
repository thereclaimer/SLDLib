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

        xml_doc_t* doc = xml_memory_get_ptr_doc(h_doc); 
        xml_memory_free_doc(doc);
    }

    SLD_API void
    xml_doc_reset(
        const xml_hnd_doc_t h_doc) {

        xml_doc_t* doc = xml_memory_get_ptr_doc(h_doc); 
        xml_memory_reset_doc(doc);
    }

    SLD_API bool
    xml_doc_buffer_read(
        const xml_hnd_doc_t h_doc,
        const buffer_t&     buffer) {

        const u64 buffer_length = xml_doc_buffer_length(h_doc);

        xml_doc_t* doc = xml_memory_get_ptr_doc(h_doc); 

        assert(doc);
        assert(buffer.data != NULL && buffer.size != 0);

        const pugi::xml_parse_result result = doc->pugi.load_buffer(
            (void*)buffer.data,
            buffer.size
        );

        const bool did_read = (result.status == pugi::xml_parse_status::status_ok);

        return(did_read);
    }

    SLD_API bool
    xml_doc_buffer_write(
        const xml_hnd_doc_t h_doc,
        buffer_t&           buffer) {

        xml_doc_t* doc = xml_memory_get_ptr_doc(h_doc); 

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
    xml_doc_buffer_length(
        const xml_hnd_doc_t h_doc) {

        xml_doc_t* doc = xml_memory_get_ptr_doc(h_doc); 
        assert(doc);

        xml_writer_t xml_writer;
        xml_writer.buffer.data = NULL;

        doc->pugi.save(xml_writer);

        const u64 length = xml_writer.buffer.length;
        return(length);
    }

    SLD_API const xml_hnd_node_t
    xml_doc_get_child_node(
        const xml_hnd_doc_t h_doc,
        const xml_utf8_t*   child_name) {

        xml_doc_t*  doc        = xml_memory_get_ptr_doc (h_doc); 
        xml_node_t* child_node = NULL;

        bool is_valid = true;
        is_valid &= (doc        != NULL);
        is_valid &= (child_name != NULL);
        assert(is_valid);

        pugi::xml_node pugi_node = doc->pugi.child(child_name); 
        if (pugi_node != NULL) {

            child_node = xml_memory_alloc_node();
            assert(child_node);

            child_node->pugi     = pugi_node;
            child_node->parent   = NULL;
            child_node->next     = doc->nodes;
            child_node->children = NULL;
            child_node->doc      = doc;

            doc->nodes = child_node;
        }

        const xml_hnd_node_t hnd_node = xml_memory_get_hnd_node(child_node);
        return(hnd_node);
    }

    SLD_API const xml_hnd_node_t
    xml_doc_get_or_add_child_node(
        const xml_hnd_doc_t h_doc,
        const xml_utf8_t*   child_name) {

        xml_doc_t*  doc        = xml_memory_get_ptr_doc (h_doc); 
        xml_node_t* child_node = NULL; 

        bool is_valid = true;
        is_valid &= (doc        != NULL);
        is_valid &= (child_name != NULL);
        assert(is_valid);

        pugi::xml_node pugi_node = doc->pugi.child(child_name); 
        if (pugi_node == NULL) {
            pugi_node = doc->pugi.append_child(child_name);            
            if (pugi_node != NULL) {

                child_node = xml_memory_alloc_node();
                assert(child_node);

                child_node->pugi     = pugi_node;
                child_node->parent   = NULL;
                child_node->next     = doc->nodes;
                child_node->children = NULL;
                child_node->doc      = doc;

                doc->nodes = child_node;
            }
        }

        const xml_hnd_node_t hnd_node = xml_memory_get_hnd_node(child_node);
        return(hnd_node);
    }

    SLD_API const xml_hnd_node_t
    xml_doc_add_child_node(
        const xml_hnd_doc_t h_doc,
        const c8*           child_name) {

        xml_doc_t*  doc        = xml_memory_get_ptr_doc (h_doc); 
        xml_node_t* child_node = NULL;

        bool is_valid = true;
        is_valid &= (doc        != NULL);
        is_valid &= (child_name != NULL);
        assert(is_valid);

        pugi::xml_node pugi_node = doc->pugi.append_child(child_name);
        if (pugi_node != NULL) {
            
            child_node = xml_memory_alloc_node();
            assert(child_node);

            child_node->pugi     = pugi_node;
            child_node->parent   = NULL;
            child_node->next     = doc->nodes;
            child_node->children = NULL;
            child_node->doc      = doc;

            doc->nodes = child_node;
        }

        const xml_hnd_node_t hnd_node = xml_memory_get_hnd_node(child_node);
        return(hnd_node);
    }

    SLD_API u32
    xml_doc_get_child_node_count(
        const xml_hnd_doc_t h_doc,
        const xml_utf8_t*   child_name) {

        xml_doc_t* doc = xml_memory_get_ptr_doc (h_doc); 

        assert(doc);
        assert(child_name);

        u32  count    = 0;
        auto children = doc->pugi.children(child_name); 
        for (
            auto iter = children.begin();
            iter != children.end();
            ++iter) {
            ++count;
        }
    
        return(count);
    }
};