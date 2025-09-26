#pragma once

#include "sld-xml-internal.cpp"

namespace sld {

    SLD_API u32
    xml_stack_minimum_size(
        void) {

        return(XML_STACK_MIN_SIZE);
    }

    SLD_API xml_stack_t*
    xml_stack_init(
        const void* memory,
        const u32   size) {

        assert(memory != NULL && size >= XML_STACK_MIN_SIZE);

        constexpr u32 struct_size = sizeof(xml_stack_t);

        xml_stack_t* stack = (xml_stack_t*)memory;
        stack->start       = (addr)memory + struct_size;
        stack->size        = (size - struct_size);
        stack->position    = 0;
        stack->save        = 0;

        return(stack);
    }

    SLD_API bool
    xml_stack_validate(
        xml_stack_t* const stack) {

        bool is_valid = (stack != NULL);
        if (is_valid) {
            is_valid &= stack_validate(stack);
            is_valid &= (stack->size >= XML_STACK_MIN_SIZE);
        }
        return(is_valid);
    }

    SLD_API void
    xml_stack_reset(
        xml_stack_t* const stack) {

        bool is_reset = true; 
        is_reset = xml_stack_validate (stack); 
        is_reset = stack_reset        (stack);
        assert(is_reset);
    }

    SLD_API xml_doc_t*
    xml_stack_push_doc(
        xml_stack_t* const stack,
        const u32          count) {

        bool can_push = true;
        can_push &= xml_stack_validate(stack);
        can_push &= (count != 0);
        assert(can_push);

        const u32  doc_size  = count * sizeof(xml_doc_t);
        xml_doc_t* doc_ptr   = (xml_doc_t*)stack_push(stack, doc_size);
        const addr doc_start = (addr)doc_ptr;
        assert(doc_start != 0);      

        for (
            u32 doc_index = 0;
            doc_index < count;
            ++doc_index) {

            const u32  doc_offset = doc_index * sizeof(xml_doc_t);
            void*      doc_mem    = (void*)((addr)doc_start + doc_offset);
            xml_doc_t* doc        = new (doc_mem) xml_doc_t();
            assert(doc);
        }

        return(doc_ptr);
    }

    SLD_API xml_node_t*
    xml_stack_push_node(
        xml_stack_t* const stack,
        const u32          count) {

        bool can_push = true;
        can_push &= xml_stack_validate(stack);
        can_push &= (count != 0);
        assert(can_push);

        const u32   node_size  = count * sizeof(xml_node_t);
        xml_node_t* node_ptr   = (xml_node_t*)stack_push(stack, node_size);
        const addr  node_start = (addr)node_ptr;
        assert(node_start != 0);      

        for (
            u32 node_index = 0;
            node_index < count;
            ++node_index) {

            const u32   node_offset = node_index * sizeof(xml_node_t);
            void*       node_mem    = (void*)((addr)node_start + node_offset);
            xml_node_t* node        = new (node_mem) xml_node_t();
            assert(node);
        }

        return(node_ptr);
    }

    SLD_API xml_attrib_t*
    xml_stack_push_attrib(
        xml_stack_t* const stack,
        const u32          count) {

        bool can_push = true;
        can_push &= xml_stack_validate(stack);
        can_push &= (count != 0);
        assert(can_push);

        const u32     attrib_size  = count * sizeof(xml_attrib_t);
        xml_attrib_t* attrib_ptr   = (xml_attrib_t*)stack_push(stack, attrib_size);
        const addr    attrib_start = (addr)attrib_ptr;
        assert(attrib_start != 0);      

        for (
            u32 attrib_index = 0;
            attrib_index < count;
            ++attrib_index) {

            const u32     attrib_offset = attrib_index * sizeof(xml_attrib_t);
            void*         attrib_mem    = (void*)((addr)attrib_start + attrib_offset);
            xml_attrib_t* attrib        = new (attrib_mem) xml_attrib_t();
            assert(attrib);
        }

        return(attrib_ptr);
    }

    SLD_API u32
    xml_stack_size_total(
        xml_stack_t* const stack) {

        assert(xml_stack_validate(stack));

        const u32 size_total = stack->size;
        return(size_total);
    }

    SLD_API u32
    xml_stack_size_used(
        xml_stack_t* const stack) {

        assert(xml_stack_validate(stack));

        const u32 size_used = stack->position;
        return(size_used);
    }
};