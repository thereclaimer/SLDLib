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

        xml_stack_t* stack = (xml_stack_t*)data_stack_init_from_memory(memory, size);    

        return(stack);
    }

    SLD_API bool
    xml_stack_validate(
        xml_stack_t* const stack) {

        bool is_valid = (stack != NULL);
        if (is_valid) {
            is_valid &= stack->is_valid(); 
            is_valid &= (stack->size >= XML_STACK_MIN_SIZE);
        }
        return(is_valid);
    }

    SLD_API void
    xml_stack_reset(
        xml_stack_t* const stack) {

        bool is_reset = true; 
        is_reset = xml_stack_validate (stack); 
        stack->reset();
        assert(is_reset);
    }

    SLD_API xml_doc_t*
    xml_stack_push_doc(
        xml_stack_t* const stack) {

        bool can_push = true;
        can_push &= xml_stack_validate(stack);
        assert(can_push);

        constexpr u32  doc_size  = sizeof(xml_doc_t);
        xml_doc_t*     doc_ptr   = (xml_doc_t*)stack->push(doc_size);
        assert(doc_ptr);
        return(doc_ptr);
    }

    SLD_API xml_node_t*
    xml_stack_push_node(
        xml_stack_t* const stack) {

        bool can_push = true;
        can_push &= xml_stack_validate(stack);
        assert(can_push);

        constexpr u32 node_size = sizeof(xml_node_t);
        xml_node_t*   node_ptr  = (xml_node_t*)stack->push(node_size);
        assert(node_ptr != 0);      

        return(node_ptr);
    }

    SLD_API xml_attrib_t*
    xml_stack_push_attrib(
        xml_stack_t* const stack) {

        bool can_push = true;
        can_push &= xml_stack_validate(stack);
        assert(can_push);

        constexpr u32 attrib_size  = sizeof(xml_attrib_t);
        xml_attrib_t* attrib_ptr   = (xml_attrib_t*)stack->push(attrib_size);
        assert(attrib_ptr);
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