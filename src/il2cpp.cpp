/*
GolfHook - Golf With Your Friends Hook
Copyright 2026
https://github.com/thecybermind/golfhook/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#define _CRT_SECURE_NO_WARNINGS 1
#include <cstring>

#define IL2CPP_INTERNAL     // use internal location of DLL pointer struct
#include "il2cpp.hpp"
#undef IL2CPP_INTERNAL
#include "util.hpp"


namespace IL2CPP {
    DLLFunctions dll = {};

    bool initialize() {
        static bool initialized = false;
        if (initialized)
            return true;

        void* hGameAssembly = Util::dll_handle("GameAssembly.dll");
        if (!hGameAssembly)
            return false;

        // debugger
        IL2CPP_GET_ADDR(is_debugger_attached, "il2cpp_is_debugger_attached", hGameAssembly);

        // internal calls and runtime
        IL2CPP_GET_ADDR(add_internal_call, "il2cpp_add_internal_call", hGameAssembly);
        IL2CPP_GET_ADDR(resolve_icall, "il2cpp_resolve_icall", hGameAssembly);
        IL2CPP_GET_ADDR(runtime_class_init, "il2cpp_runtime_class_init", hGameAssembly);
        IL2CPP_GET_ADDR(runtime_object_init, "il2cpp_runtime_object_init", hGameAssembly);

        // thread, domains, and assemblies
        IL2CPP_GET_ADDR(thread_attach, "il2cpp_thread_attach", hGameAssembly);
        IL2CPP_GET_ADDR(thread_detach, "il2cpp_thread_detach", hGameAssembly);
        IL2CPP_GET_ADDR(domain_get, "il2cpp_domain_get", hGameAssembly);
        IL2CPP_GET_ADDR(domain_get_assemblies, "il2cpp_domain_get_assemblies", hGameAssembly);

        // images
        IL2CPP_GET_ADDR(assembly_get_image, "il2cpp_assembly_get_image", hGameAssembly);
        IL2CPP_GET_ADDR(image_get_name, "il2cpp_image_get_name", hGameAssembly);
        IL2CPP_GET_ADDR(image_get_class, "il2cpp_image_get_class", hGameAssembly);
        IL2CPP_GET_ADDR(image_get_class_count, "il2cpp_image_get_class_count", hGameAssembly);

        // objects
        IL2CPP_GET_ADDR(object_new, "il2cpp_object_new", hGameAssembly);
        IL2CPP_GET_ADDR(object_box, "il2cpp_object_box", hGameAssembly);
        IL2CPP_GET_ADDR(object_unbox, "il2cpp_object_unbox", hGameAssembly);
        IL2CPP_GET_ADDR(object_get_size, "il2cpp_object_get_size", hGameAssembly);
        IL2CPP_GET_ADDR(object_get_class, "il2cpp_object_get_class", hGameAssembly);
        IL2CPP_GET_ADDR(object_get_virtual_method, "il2cpp_object_get_virtual_method", hGameAssembly);

        // arrays
        IL2CPP_GET_ADDR(array_element_size, "il2cpp_array_element_size", hGameAssembly);
        IL2CPP_GET_ADDR(array_class_get, "il2cpp_array_class_get", hGameAssembly);
        IL2CPP_GET_ADDR(get_array_length, "il2cpp_array_length", hGameAssembly);
        IL2CPP_GET_ADDR(array_new, "il2cpp_array_new", hGameAssembly);

        // classes
        IL2CPP_GET_ADDR(class_from_name, "il2cpp_class_from_name", hGameAssembly);
        IL2CPP_GET_ADDR(class_get_name, "il2cpp_class_get_name", hGameAssembly);
        IL2CPP_GET_ADDR(class_get_namespace, "il2cpp_class_get_namespace", hGameAssembly);
        IL2CPP_GET_ADDR(class_enum_basetype, "il2cpp_class_enum_basetype", hGameAssembly);
        IL2CPP_GET_ADDR(class_is_generic, "il2cpp_class_is_generic", hGameAssembly);
        IL2CPP_GET_ADDR(class_is_inflated, "il2cpp_class_is_inflated", hGameAssembly);
        IL2CPP_GET_ADDR(class_has_parent, "il2cpp_has_parent", hGameAssembly);
        IL2CPP_GET_ADDR(class_get_parent, "il2cpp_class_get_parent", hGameAssembly);
        IL2CPP_GET_ADDR(class_is_subclass_of, "il2cpp_class_is_subclass_of", hGameAssembly);
        IL2CPP_GET_ADDR(class_from_il2cpp_type, "il2cpp_class_from_il2cpp_type", hGameAssembly);
        IL2CPP_GET_ADDR(class_get_events, "il2cpp_class_get_events", hGameAssembly);
        IL2CPP_GET_ADDR(class_get_interfaces, "il2cpp_class_get_interfaces", hGameAssembly);
        IL2CPP_GET_ADDR(class_get_properties, "il2cpp_class_get_properties", hGameAssembly);
        IL2CPP_GET_ADDR(class_property_from_name, "il2cpp_class_property_from_name", hGameAssembly);

        // types
        IL2CPP_GET_ADDR(class_get_nested_types, "il2cpp_class_get_nested_types", hGameAssembly);
        IL2CPP_GET_ADDR(type_get_attrs, "il2cpp_type_get_attrs", hGameAssembly);
        IL2CPP_GET_ADDR(type_get_class_or_element_class, "il2cpp_type_get_class_or_element_class", hGameAssembly);
        IL2CPP_GET_ADDR(type_get_name, "il2cpp_type_get_name", hGameAssembly);
        IL2CPP_GET_ADDR(type_get_object, "il2cpp_type_get_object", hGameAssembly);
        IL2CPP_GET_ADDR(type_get_type, "il2cpp_type_get_type", hGameAssembly);
        IL2CPP_GET_ADDR(type_equals, "il2cpp_type_equals", hGameAssembly);
        IL2CPP_GET_ADDR(type_is_byref, "il2cpp_type_is_byref", hGameAssembly);

        // methods
        IL2CPP_GET_ADDR(runtime_invoke, "il2cpp_runtime_invoke", hGameAssembly);
        IL2CPP_GET_ADDR(class_get_method_from_name, "il2cpp_class_get_method_from_name", hGameAssembly);
        IL2CPP_GET_ADDR(class_get_methods, "il2cpp_class_get_methods", hGameAssembly);
        IL2CPP_GET_ADDR(method_get_name, "il2cpp_method_get_name", hGameAssembly);
        IL2CPP_GET_ADDR(method_is_generic, "il2cpp_method_is_generic", hGameAssembly);
        IL2CPP_GET_ADDR(method_is_inflated, "il2cpp_method_is_inflated", hGameAssembly);
        IL2CPP_GET_ADDR(method_is_instance, "il2cpp_method_is_instance", hGameAssembly);
        IL2CPP_GET_ADDR(method_get_param_count, "il2cpp_method_get_param_count", hGameAssembly);
        IL2CPP_GET_ADDR(method_get_param_name, "il2cpp_method_get_param_name", hGameAssembly);
        IL2CPP_GET_ADDR(method_get_param_type, "il2cpp_get_param", hGameAssembly);
        IL2CPP_GET_ADDR(method_get_return_type, "il2cpp_method_get_return_type", hGameAssembly);

        // fields
        IL2CPP_GET_ADDR(class_get_fields, "il2cpp_class_get_fields", hGameAssembly);
        IL2CPP_GET_ADDR(class_get_field_from_name, "il2cpp_class_get_field_from_name", hGameAssembly);
        IL2CPP_GET_ADDR(field_get_offset, "il2cpp_field_get_offset", hGameAssembly);
        IL2CPP_GET_ADDR(class_num_fields, "il2cpp_class_num_fields", hGameAssembly);
        IL2CPP_GET_ADDR(field_get_name, "il2cpp_field_get_name", hGameAssembly);
        IL2CPP_GET_ADDR(field_get_value_object, "il2cpp_field_get_value_object", hGameAssembly);
        IL2CPP_GET_ADDR(field_set_value_object, "il2cpp_field_set_value_object", hGameAssembly);
        IL2CPP_GET_ADDR(field_get_value, "il2cpp_field_get_value", hGameAssembly);
        IL2CPP_GET_ADDR(field_set_value, "il2cpp_field_set_value", hGameAssembly);
        IL2CPP_GET_ADDR(field_get_type, "il2cpp_field_get_type", hGameAssembly);
        IL2CPP_GET_ADDR(field_static_get_value, "il2cpp_field_static_get_value", hGameAssembly);
        IL2CPP_GET_ADDR(field_static_set_value, "il2cpp_field_static_set_value", hGameAssembly);
        IL2CPP_GET_ADDR(field_get_flags, "il2cpp_field_get_flags", hGameAssembly);

        // strings
        IL2CPP_GET_ADDR(string_chars, "il2cpp_string_chars", hGameAssembly);
        IL2CPP_GET_ADDR(string_new, "il2cpp_string_new", hGameAssembly);

        initialized = true;
        return true;
    }   // initialize()


    void shutdown() {
    }
}   // namespace IL2CPP
