/*
GolfHook - Golf With Your Friends Hook
Copyright 2026
https://github.com/thecybermind/golfhook/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#pragma once

#ifndef GOLFHOOK_IL2CPP_HPP
#define GOLFHOOK_IL2CPP_HPP

#include <cstdint>
#include <cstring>
#include <vector>

 // Declare a function pointer named 'name' with signature 'ret_type(args)'
#define IL2CPP_API( name, ret_type, args )          ret_type (*name) args;
// Assign the function address of 'func_name' in module 'handle' to the function pointer named 'name'
#define IL2CPP_GET_ADDR( name, func_name, handle )  dll. name = (decltype(dll. name))Util::dll_symbol(handle, func_name)

#if defined(IL2CPP_INTERNAL)
 #define DLL_FUNCS (IL2CPP::dll)
#else
 #define DLL_FUNCS (*(g_pluginvars->IL2CPP_dll))
#endif

#define FIELD_ATTRIBUTE_FIELD_ACCESS_MASK   0x0007
#define FIELD_ATTRIBUTE_COMPILER_CONTROLLED 0x0000
#define FIELD_ATTRIBUTE_PRIVATE             0x0001
#define FIELD_ATTRIBUTE_FAM_AND_ASSEM       0x0002
#define FIELD_ATTRIBUTE_ASSEMBLY            0x0003
#define FIELD_ATTRIBUTE_FAMILY              0x0004
#define FIELD_ATTRIBUTE_FAM_OR_ASSEM        0x0005
#define FIELD_ATTRIBUTE_PUBLIC              0x0006

#define FIELD_ATTRIBUTE_STATIC              0x0010
#define FIELD_ATTRIBUTE_INIT_ONLY           0x0020
#define FIELD_ATTRIBUTE_LITERAL             0x0040
#define FIELD_ATTRIBUTE_NOT_SERIALIZED      0x0080
#define FIELD_ATTRIBUTE_SPECIAL_NAME        0x0200
#define FIELD_ATTRIBUTE_PINVOKE_IMPL        0x2000

// For runtime use only
#define FIELD_ATTRIBUTE_RESERVED_MASK       0x9500
#define FIELD_ATTRIBUTE_RT_SPECIAL_NAME     0x0400
#define FIELD_ATTRIBUTE_HAS_FIELD_MARSHAL   0x1000
#define FIELD_ATTRIBUTE_HAS_DEFAULT         0x8000
#define FIELD_ATTRIBUTE_HAS_FIELD_RVA       0x0100

// Method Attributes (22.1.9)
#define METHOD_IMPL_ATTRIBUTE_CODE_TYPE_MASK 0x0003
#define METHOD_IMPL_ATTRIBUTE_IL            0x0000
#define METHOD_IMPL_ATTRIBUTE_NATIVE        0x0001
#define METHOD_IMPL_ATTRIBUTE_OPTIL         0x0002
#define METHOD_IMPL_ATTRIBUTE_RUNTIME       0x0003

#define METHOD_IMPL_ATTRIBUTE_MANAGED_MASK  0x0004
#define METHOD_IMPL_ATTRIBUTE_UNMANAGED     0x0004
#define METHOD_IMPL_ATTRIBUTE_MANAGED       0x0000

#define METHOD_IMPL_ATTRIBUTE_FORWARD_REF   0x0010
#define METHOD_IMPL_ATTRIBUTE_PRESERVE_SIG  0x0080
#define METHOD_IMPL_ATTRIBUTE_INTERNAL_CALL 0x1000
#define METHOD_IMPL_ATTRIBUTE_SYNCHRONIZED  0x0020
#define METHOD_IMPL_ATTRIBUTE_NOINLINING    0x0008
#define METHOD_IMPL_ATTRIBUTE_MAX_METHOD_IMPL_VAL 0xffff

#define METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK 0x0007
#define METHOD_ATTRIBUTE_COMPILER_CONTROLLED 0x0000
#define METHOD_ATTRIBUTE_PRIVATE            0x0001
#define METHOD_ATTRIBUTE_FAM_AND_ASSEM      0x0002
#define METHOD_ATTRIBUTE_ASSEM              0x0003
#define METHOD_ATTRIBUTE_FAMILY             0x0004
#define METHOD_ATTRIBUTE_FAM_OR_ASSEM       0x0005
#define METHOD_ATTRIBUTE_PUBLIC             0x0006

#define METHOD_ATTRIBUTE_STATIC             0x0010
#define METHOD_ATTRIBUTE_FINAL              0x0020
#define METHOD_ATTRIBUTE_VIRTUAL            0x0040
#define METHOD_ATTRIBUTE_HIDE_BY_SIG        0x0080

#define METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK 0x0100
#define METHOD_ATTRIBUTE_REUSE_SLOT         0x0000
#define METHOD_ATTRIBUTE_NEW_SLOT           0x0100

#define METHOD_ATTRIBUTE_STRICT             0x0200
#define METHOD_ATTRIBUTE_ABSTRACT           0x0400
#define METHOD_ATTRIBUTE_SPECIAL_NAME       0x0800

#define METHOD_ATTRIBUTE_PINVOKE_IMPL       0x2000
#define METHOD_ATTRIBUTE_UNMANAGED_EXPORT   0x0008

// For runtime use only
#define METHOD_ATTRIBUTE_RESERVED_MASK      0xd000
#define METHOD_ATTRIBUTE_RT_SPECIAL_NAME    0x1000
#define METHOD_ATTRIBUTE_HAS_SECURITY       0x4000
#define METHOD_ATTRIBUTE_REQUIRE_SEC_OBJECT 0x8000

// Type Attributes (21.1.13).
#define TYPE_ATTRIBUTE_VISIBILITY_MASK      0x00000007
#define TYPE_ATTRIBUTE_NOT_PUBLIC           0x00000000
#define TYPE_ATTRIBUTE_PUBLIC               0x00000001
#define TYPE_ATTRIBUTE_NESTED_PUBLIC        0x00000002
#define TYPE_ATTRIBUTE_NESTED_PRIVATE       0x00000003
#define TYPE_ATTRIBUTE_NESTED_FAMILY        0x00000004
#define TYPE_ATTRIBUTE_NESTED_ASSEMBLY      0x00000005
#define TYPE_ATTRIBUTE_NESTED_FAM_AND_ASSEM 0x00000006
#define TYPE_ATTRIBUTE_NESTED_FAM_OR_ASSEM  0x00000007

#define TYPE_ATTRIBUTE_LAYOUT_MASK          0x00000018
#define TYPE_ATTRIBUTE_AUTO_LAYOUT          0x00000000
#define TYPE_ATTRIBUTE_SEQUENTIAL_LAYOUT    0x00000008
#define TYPE_ATTRIBUTE_EXPLICIT_LAYOUT      0x00000010

#define TYPE_ATTRIBUTE_CLASS_SEMANTIC_MASK  0x00000020
#define TYPE_ATTRIBUTE_CLASS                0x00000000
#define TYPE_ATTRIBUTE_INTERFACE            0x00000020

#define TYPE_ATTRIBUTE_ABSTRACT             0x00000080
#define TYPE_ATTRIBUTE_SEALED               0x00000100
#define TYPE_ATTRIBUTE_SPECIAL_NAME         0x00000400

#define TYPE_ATTRIBUTE_IMPORT               0x00001000
#define TYPE_ATTRIBUTE_SERIALIZABLE         0x00002000

#define TYPE_ATTRIBUTE_STRING_FORMAT_MASK   0x00030000
#define TYPE_ATTRIBUTE_ANSI_CLASS           0x00000000
#define TYPE_ATTRIBUTE_UNICODE_CLASS        0x00010000
#define TYPE_ATTRIBUTE_AUTO_CLASS           0x00020000

#define TYPE_ATTRIBUTE_BEFORE_FIELD_INIT    0x00100000
#define TYPE_ATTRIBUTE_FORWARDER            0x00200000

#define TYPE_ATTRIBUTE_RESERVED_MASK        0x00040800
#define TYPE_ATTRIBUTE_RT_SPECIAL_NAME      0x00000800
#define TYPE_ATTRIBUTE_HAS_SECURITY         0x00040000

// Flags for Params (22.1.12)
#define PARAM_ATTRIBUTE_IN                  0x0001
#define PARAM_ATTRIBUTE_OUT                 0x0002
#define PARAM_ATTRIBUTE_OPTIONAL            0x0010
#define PARAM_ATTRIBUTE_RESERVED_MASK       0xf000
#define PARAM_ATTRIBUTE_HAS_DEFAULT         0x1000
#define PARAM_ATTRIBUTE_HAS_FIELD_MARSHAL   0x2000
#define PARAM_ATTRIBUTE_UNUSED              0xcfe0

// Flags for Generic Parameters (II.23.1.7)
#define IL2CPP_GENERIC_PARAMETER_ATTRIBUTE_NON_VARIANT                        0x00
#define IL2CPP_GENERIC_PARAMETER_ATTRIBUTE_COVARIANT                          0x01
#define IL2CPP_GENERIC_PARAMETER_ATTRIBUTE_CONTRAVARIANT                      0x02
#define IL2CPP_GENERIC_PARAMETER_ATTRIBUTE_VARIANCE_MASK                      0x03
#define IL2CPP_GENERIC_PARAMETER_ATTRIBUTE_REFERENCE_TYPE_CONSTRAINT          0x04
#define IL2CPP_GENERIC_PARAMETER_ATTRIBUTE_NOT_NULLABLE_VALUE_TYPE_CONSTRAINT 0x08
#define IL2CPP_GENERIC_PARAMETER_ATTRIBUTE_DEFAULT_CONSTRUCTOR_CONSTRAINT     0x10
#define IL2CPP_GENERIC_PARAMETER_ATTRIBUTE_SPECIAL_CONSTRAINT_MASK            0x1C

// 21.5 AssemblyRefs
#define ASSEMBLYREF_FULL_PUBLIC_KEY_FLAG             0x00000001
#define ASSEMBLYREF_RETARGETABLE_FLAG                0x00000100
#define ASSEMBLYREF_ENABLEJITCOMPILE_TRACKING_FLAG   0x00008000
#define ASSEMBLYREF_DISABLEJITCOMPILE_OPTIMIZER_FLAG 0x00004000


namespace IL2CPP {
    struct Method;
    struct Field;
    struct Class;
    struct Type;
    struct Image;

    // Initialize IL2CPP pointers
    bool initialize();

    // Clean up internal IL2CPP resources
    void shutdown();

    struct DLLFunctions {
        // debugger
        IL2CPP_API(is_debugger_attached, bool, (void));

        // internal calls and runtime
        IL2CPP_API(add_internal_call, void, (const char* name, const Method* self_ptr));
        IL2CPP_API(resolve_icall, void, (const char* name));
        IL2CPP_API(runtime_class_init, void, (void* _class));
        IL2CPP_API(runtime_object_init, void, (void* obj));

        // thread, domain, and assembly
        IL2CPP_API(thread_attach, const void*, (const void* domain));
        IL2CPP_API(thread_detach, void, (const void* thread));
        IL2CPP_API(domain_get, const void*, (void));
        IL2CPP_API(domain_get_assemblies, const void**, (const void* domain, size_t* count));

        // images
        IL2CPP_API(assembly_get_image, const Image*, (const void* assembly));
        IL2CPP_API(image_get_name, const char*, (const Image* self));
        IL2CPP_API(image_get_class_count, size_t, (const Image* self));
        IL2CPP_API(image_get_class, const Class*, (const Image* self, size_t index));

        // objects
        IL2CPP_API(object_new, void*, (void* domain, const Class* _class));
        IL2CPP_API(object_unbox, void*, (void* obj));
        IL2CPP_API(object_box, void*, (const Class* _class, void* data));
        IL2CPP_API(object_get_size, uint32_t, (void* obj));
        IL2CPP_API(object_get_class, void*, (void* obj));
        IL2CPP_API(object_get_virtual_method, void*, (void* obj, void* method));

        // arrays
        IL2CPP_API(array_element_size, int, (const void* array_class));
        IL2CPP_API(array_class_get, void*, (const void* element_class, uint32_t rank));
        IL2CPP_API(get_array_length, uint32_t, (const void* array));
        IL2CPP_API(array_new, void*, (void* element_type_infof, size_t length));

        // class
        IL2CPP_API(class_from_name, const Class*, (const Image* image, const char* _namespace, const char* name));
        IL2CPP_API(class_get_name, const char*, (const Class* self));
        IL2CPP_API(class_get_namespace, const char*, (const Class* self));
        IL2CPP_API(class_enum_basetype, void*, (const Class* self));
        IL2CPP_API(class_is_generic, bool, (const Class* self));
        IL2CPP_API(class_is_inflated, bool, (const Class* self));
        IL2CPP_API(class_has_parent, bool, (const Class* self, const void* class_c));
        IL2CPP_API(class_get_parent, void*, (const Class* self));
        IL2CPP_API(class_is_subclass_of, bool, (const Class* self, const void* parent__class, bool check_interfaces));
        IL2CPP_API(class_from_il2cpp_type, void*, (const Type* self));
        IL2CPP_API(class_get_events, void*, (const Class* self, void* iter));
        IL2CPP_API(class_get_interfaces, void*, (const Class* self, void* iter));
        IL2CPP_API(class_get_properties, void*, (const Class* self, void* iter));
        IL2CPP_API(class_property_from_name, void*, (const Class* self, const char* name));

        // types
        IL2CPP_API(class_get_nested_types, const Class*, (const Class* self, void* iter));
        IL2CPP_API(type_get_attrs, uint32_t, (const Type* self));
        IL2CPP_API(type_get_class_or_element_class, const Class*, (const Type* self));
        IL2CPP_API(type_get_name, const char*, (const Type* self));
        IL2CPP_API(type_get_object, void*, (const Type* self));
        IL2CPP_API(type_get_type, int, (const Type* self));
        IL2CPP_API(type_equals, bool, (const Type* type_1, const Type* type_2));
        IL2CPP_API(type_is_byref, bool, (const Type* self));

        // methods
        IL2CPP_API(runtime_invoke, void*, (const Method* self, void* obj, void** params, void** excption));
        IL2CPP_API(class_get_method_from_name, const Method*, (const Class* self, const char* name, int params));
        IL2CPP_API(class_get_methods, void*, (const Class* self, void* iter));
        IL2CPP_API(method_get_name, const char*, (const Method* self));
        IL2CPP_API(method_is_generic, bool, (const Method* self));
        IL2CPP_API(method_is_inflated, bool, (const Method* self));
        IL2CPP_API(method_is_instance, bool, (const Method* self));
        IL2CPP_API(method_get_param_count, int, (const Method* self));
        IL2CPP_API(method_get_param_name, const char*, (const Method* self, int index));
        IL2CPP_API(method_get_param_type, const char*, (const Method* self, int index));
        IL2CPP_API(method_get_return_type, void*, (const Method* self));

        // fields
        IL2CPP_API(class_get_fields, const Field*, (const Class* self, void* iter));
        IL2CPP_API(class_get_field_from_name, const Field*, (const Class* self, const char* name));
        IL2CPP_API(field_get_offset, size_t, (const Field* field));
        IL2CPP_API(class_num_fields, size_t, (const Class* self));
        IL2CPP_API(field_get_name, const char*, (const Field* field));
        IL2CPP_API(field_get_value, void, (const void* obj, const Field* field, void* value));
        IL2CPP_API(field_set_value, void, (void* obj, const Field* field, const void* value));
        IL2CPP_API(field_get_value_object, void*, (const Field* field, const void* obj));
        IL2CPP_API(field_set_value_object, void, (const Field* field, void* obj, const void* value));
        IL2CPP_API(field_get_type, Type*, (const Field* field));
        IL2CPP_API(field_static_get_value, void*, (const Field* field, void* output));
        IL2CPP_API(field_static_set_value, void*, (const Field* field, void* value));
        IL2CPP_API(field_get_flags, int, (const Field* field));

        // strings
        IL2CPP_API(string_chars, wchar_t*, (void* string_obj));
        IL2CPP_API(string_new, const char*, (void* domain, const char* text));
    };

    // struct to hold all the IL2CPP function pointers (for accessibility from plugins)
    extern DLLFunctions dll;


    struct Method {
        void* invoke_static(void** params = nullptr, void** exception = nullptr) const {
            if (!DLL_FUNCS.runtime_invoke)
                return nullptr;

            void* exc = nullptr;

            const auto result = DLL_FUNCS.runtime_invoke(this, nullptr, params, &exc);

            if (exception)
                *exception = exc;

            return result;
        }

        const void* invoke(void* obj, void** params = nullptr, void** exception = nullptr) const {
            if (!DLL_FUNCS.runtime_invoke)
                return nullptr;

            void* exc = nullptr;

            const auto result = DLL_FUNCS.runtime_invoke(this, obj, params, &exc);

            if (exception)
                *exception = exc;

            return result;
        }

        uint32_t get_param_count() const {
            if (!DLL_FUNCS.method_get_param_count)
                return 0;

            return DLL_FUNCS.method_get_param_count(this);
        }

        const char* get_param_name(int index) const {
            if (!DLL_FUNCS.method_get_param_name)
                return nullptr;

            return DLL_FUNCS.method_get_param_name(this, index);
        }

        const char* get_param_type(int index) const {
            if (!DLL_FUNCS.method_get_param_type)
                return nullptr;

            return DLL_FUNCS.method_get_param_type(this, index);
        }

        bool is_generic() const {
            if (!DLL_FUNCS.method_is_generic)
                return false;

            return DLL_FUNCS.method_is_generic(this);
        }

        bool is_inflated() const {
            if (!DLL_FUNCS.method_is_inflated)
                return false;

            return DLL_FUNCS.method_is_inflated(this);
        }

        bool is_instance() const {
            if (!DLL_FUNCS.method_is_instance)
                return false;

            return DLL_FUNCS.method_is_instance(this);
        }

        const char* get_name() const {
            if (!DLL_FUNCS.method_get_name)
                return nullptr;

            return DLL_FUNCS.method_get_name(this);
        }
    };  // struct Method


    struct Field {
        void* get_static_value() const {
            if (!DLL_FUNCS.field_static_get_value)
                return nullptr;

            void* val = nullptr;

            DLL_FUNCS.field_static_get_value(this, &val);

            return val;
        }

        void* set_static_value(void* value) const {
            if (!DLL_FUNCS.field_static_set_value)
                return nullptr;

            return DLL_FUNCS.field_static_set_value(this, value);
        }

        size_t get_offset() const {
            if (!DLL_FUNCS.field_get_offset)
                return 0x0;

            return DLL_FUNCS.field_get_offset(this);
        }

        void* get_object(const void* self) const {
            if (!DLL_FUNCS.field_get_value_object)
                return nullptr;

            return DLL_FUNCS.field_get_value_object(this, self);
        }

        Type* get_type() const {
            if (!DLL_FUNCS.field_get_type)
                return nullptr;

            return DLL_FUNCS.field_get_type(this);
        }

        const char* get_name() const {
            if (!DLL_FUNCS.field_get_name)
                return nullptr;

            return DLL_FUNCS.field_get_name(this);
        }
    };  // struct Field


    struct Class {
        const Method* get_method(const char* name, int argc = 0) const {
            if (!DLL_FUNCS.class_get_method_from_name)
                return nullptr;

             return DLL_FUNCS.class_get_method_from_name(this, name, argc);
        }

        const Field* get_field(const char* name) const {
            if (!DLL_FUNCS.class_get_field_from_name)
                return nullptr;

            return DLL_FUNCS.class_get_field_from_name(this, name);
        }

        std::vector<const Field*> get_fields() const {
            std::vector<const Field*> fields = {};

            if (!DLL_FUNCS.class_num_fields || !DLL_FUNCS.class_get_fields)
                return fields;

            const size_t count = DLL_FUNCS.class_num_fields(this);
            if (count) {
                fields.reserve(count);

                void* iter = nullptr;
                const Field* field = nullptr;

                while ((field = DLL_FUNCS.class_get_fields(this, &iter))) {
                    fields.push_back(field);
                }
            }

            return fields;
        }

        size_t get_field_count() const {
            if (!DLL_FUNCS.class_num_fields)
                return 0;

            return DLL_FUNCS.class_num_fields(this);
        }

        const Class* get_nested_class(const char* name) const {
            if (!DLL_FUNCS.class_get_nested_types || !DLL_FUNCS.class_get_name)
                return nullptr;

            void* iter = nullptr;
            const Class* type = nullptr;

            while ((type = DLL_FUNCS.class_get_nested_types(this, &iter))) {
                const auto class_name = DLL_FUNCS.class_get_name(type);

                if (strcmp(class_name, name) == 0)
                    return type;
            }

            return nullptr;
        }

        const char* get_name() const {
            if (!DLL_FUNCS.class_get_name)
                return nullptr;

            return DLL_FUNCS.class_get_name(this);
        }
    };  // struct Class


    struct Type {
        const Class* get_class() const {
            if (!DLL_FUNCS.type_get_class_or_element_class)
                return nullptr;

            return DLL_FUNCS.type_get_class_or_element_class(this);
        }

        void* get_object() const {
            if (!DLL_FUNCS.type_get_object)
                return nullptr;

            return DLL_FUNCS.type_get_object(this);
        }

        int get_type() const {
            if (!DLL_FUNCS.type_get_type)
                return -1;

            return DLL_FUNCS.type_get_type(this);
        }

        uint32_t get_attributes() const {
            if (!DLL_FUNCS.type_get_attrs)
                return 0;

            return DLL_FUNCS.type_get_attrs(this);
        }

        bool compare_type(Type* other) const {
            if (!DLL_FUNCS.type_equals)
                return false;

            return DLL_FUNCS.type_equals(this, other);
        }

        bool is_by_ref() const {
            if (!DLL_FUNCS.type_is_byref)
                return false;

            return DLL_FUNCS.type_is_byref(this);
        }

        const char* get_name() const {
            if (!DLL_FUNCS.type_get_name)
                return nullptr;

            return DLL_FUNCS.type_get_name(this);
        }
    };  // struct Type


    struct Image {
    private:
        const char* m_name; // this exists in the IL2CPP object
    public:
        const Class* get_class(const char* name, const char* name_space = "") const {
            if (!DLL_FUNCS.class_from_name)
                return nullptr;

            return DLL_FUNCS.class_from_name(this, name_space, name);
        }

        size_t get_class_count() const {
            if (!DLL_FUNCS.image_get_class_count)
                return 0;

            return DLL_FUNCS.image_get_class_count(this);
        }

        std::vector<const Class*> get_classes() const {
            std::vector<const Class*> classes = {};

            if (!DLL_FUNCS.image_get_class_count || !DLL_FUNCS.image_get_class || !DLL_FUNCS.class_get_name)
                return classes;

            size_t count = DLL_FUNCS.image_get_class_count(this);
            if (count) {
                classes.reserve(count);

                for (size_t index = 0U; index < count; ++index) {
                    const Class* address = DLL_FUNCS.image_get_class(this, index);
                    if (!address)
                        continue;

                    const char* name = DLL_FUNCS.class_get_name(address);
                    if (!name || !strcmp(name, "<Module>"))
                        continue;

                    classes.push_back(address);
                }
            }

            return classes;
        }

        const char* get_name() const {
            if (m_name)
                return m_name;

            if (!DLL_FUNCS.image_get_name)
                return nullptr;


            return DLL_FUNCS.image_get_name(this);
        }
    };  // struct Image


    struct Manager {
        std::vector<const Image*> get_images() const {
            std::vector<const Image*> images = {};

            if (!DLL_FUNCS.domain_get || !DLL_FUNCS.domain_get_assemblies || !DLL_FUNCS.assembly_get_image)
                return images;

            size_t count = 0U;

            const auto assemblies = DLL_FUNCS.domain_get_assemblies(DLL_FUNCS.domain_get(), &count);

            if (count && assemblies && *assemblies) {
                images.reserve(count);

                for (size_t index = 0U; index < count; ++index) {
                    auto image = DLL_FUNCS.assembly_get_image(assemblies[index]);
                    if (image)
                        images.push_back(image);
                }
            }

            return images;
        }

        const Image* get_image(const char* name) const {
            std::vector<const Image*> images = get_images();

            for (const auto image : images) {
                if (!strcmp(name, image->get_name()))
                    return image;
            }

            return nullptr;
        }

        bool is_debugger_attached() const {
            return DLL_FUNCS.is_debugger_attached();
        }
    };  // struct Manager

} // namespace IL2CPP

#endif // GOLFHOOK_IL2CPP_HPP
