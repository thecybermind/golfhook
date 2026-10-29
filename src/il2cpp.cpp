/*
GolfHook - Golf With Your Friends Hook
Copyright 2026
https://github.com/thecybermind/golfhook/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

/* The idea for this came from Aetherim and the IL2CPP Reverse Engineering Guide:
 * - https://github.com/Toxocious/Aetherim/
 * - https://github.com/jadis0x/il2cpp-reverse-engineering-guide
 *
 * I made this a virtual interface scheme so that the plugins don't need [separate] implementations of the IL2CPP stuff.
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdint>
#include <vector>

#include "il2cpp.hpp"
#include "log.hpp"


#define IL2CPP_API( name, ret_type, args )         \
  ret_type (* name) args = nullptr;

#define IL2CPP_GET_ADDR( name, IL2CPP_export, handle ) \
  name = (decltype(name))(GetProcAddress( handle, IL2CPP_export ) );


namespace IL2CPP {
    // debugger
    IL2CPP_API(is_debugger_attached, bool, (void));

    // internal calls and runtime
    IL2CPP_API(add_icall, void, (const char* name, const IMethod* method_ptr));
    IL2CPP_API(resolve_icall, void, (const char* name));
    IL2CPP_API(init_class, void, (void* _class));
    IL2CPP_API(init_object, void, (void* obj));

    // thread, domain, and assembly
    IL2CPP_API(thread_attach, void*, (void* domain));
    IL2CPP_API(get_domain, void*, (void));
    IL2CPP_API(get_assemblies, void**, (const void* domain, size_t* count));

    // images
    IL2CPP_API(get_image, IImage*, (const void* assembly));
    IL2CPP_API(get_image_name, const char*, (const void* image));
    IL2CPP_API(get_class_count, size_t, (const void* image));
    IL2CPP_API(get_class_from_image, IClass*, (const void* image, size_t index));

    // objects
    IL2CPP_API(object_create, void*, (void* domain, void* _class));
    IL2CPP_API(object_unbox, void*, (void* obj));
    IL2CPP_API(object_box, void*, (IClass* _class, void* data));
    IL2CPP_API(object_get_size, uint32_t, (void* obj));
    IL2CPP_API(object_get_class, IClass*, (void* obj));
    IL2CPP_API(object_get_virtual_method, IMethod*, (void* obj, IMethod* method));

    // arrays
    IL2CPP_API(get_array_element_size, int, (const void* array_class));
    IL2CPP_API(get_array_class, void*, (const void* element_class, uint32_t rank));
    IL2CPP_API(get_array_length, uint32_t, (const void* array));
    IL2CPP_API(array_new, void*, (void* element_type_infof, size_t length));

    // class
    IL2CPP_API(get_class, IClass*, (const void* image, const char* _namespace, const char* name));
    IL2CPP_API(get_class_name, const char*, (const void* _class));
    IL2CPP_API(get_class_namespace, const char*, (const void* _class));
    IL2CPP_API(get_class_type, void*, (const void* _class));
    IL2CPP_API(get_class_generic, bool, (const void* _class));
    IL2CPP_API(get_class_inflated, bool, (const void* _class));
    IL2CPP_API(get_class_has_parent, bool, (const void* _class, const void* class_c));
    IL2CPP_API(get_class_get_parent, void*, (const void* _class));
    IL2CPP_API(get_class_is_subclass_of, bool, (const void* _class, const void* parent__class, bool check_interfaces));
    IL2CPP_API(get_class_from_type, void*, (const void* type));
    IL2CPP_API(get_class_event_info, void*, (const void* _class, void* iter));
    IL2CPP_API(get_class_interfaces, void*, (const void* _class, void* iter));
    IL2CPP_API(get_class_properties, void*, (const void* _class, void* iter));
    IL2CPP_API(get_class_property_from_name, void*, (const void* _class, const char* name));

    // types
    IL2CPP_API(get_nested_types, IClass*, (const void* _class, void*));
    IL2CPP_API(get_type_attributes, uint32_t, (const void* type));
    IL2CPP_API(get_type_class, IClass*, (const void* type));
    IL2CPP_API(get_type_name, const char*, (const void* type));
    IL2CPP_API(get_type_object, void*, (const void* type));
    IL2CPP_API(get_type_type, int, (const void* type));
    IL2CPP_API(get_type_equals, bool, (IType* type_1, IType* type_2));
    IL2CPP_API(get_type_is_byref, bool, (IType* type));

    // methods
    IL2CPP_API(method_call, void*, (const IMethod* method, void* obj, void** params, void** excption));
    IL2CPP_API(get_method, void*, (const void* _class, const char* name, int params));
    IL2CPP_API(get_methods, void*, (const void* _class, void* iter));
    IL2CPP_API(get_method_name, const char*, (const IMethod* method));
    IL2CPP_API(get_method_is_generic, bool, (const IMethod* method));
    IL2CPP_API(get_method_is_inflated, bool, (const IMethod* method));
    IL2CPP_API(get_method_is_instance, bool, (const IMethod* method));
    IL2CPP_API(get_method_param_count, int, (const IMethod* method));
    IL2CPP_API(get_method_param_name, const char*, (const IMethod* method, int index));
    IL2CPP_API(get_method_param_type, const char*, (const IMethod* method, int index));
    IL2CPP_API(get_method_return_type, void*, (const IMethod* method));

    // fields
    IL2CPP_API(get_fields, void*, (const void* _class, void* iter));
    IL2CPP_API(get_field, void*, (const void* _class, const char* name));
    IL2CPP_API(get_field_offset, size_t, (const void* field));
    IL2CPP_API(get_field_count, size_t, (const void* _class));
    IL2CPP_API(get_field_name, const char*, (const void* field));
    IL2CPP_API(get_field_object, void*, (const void* field, const void* obj));
    IL2CPP_API(get_field_type, void*, (const void* field));
    IL2CPP_API(get_static_field, void*, (const void* field, void* output));
    IL2CPP_API(set_static_field, void*, (const void* field, void* value));
    IL2CPP_API(get_field_flags, int, (const void* field));

    // strings
    IL2CPP_API(get_string_chars, wchar_t*, (void* string_obj));
    IL2CPP_API(create_string, const char*, (void* domain, const char* text));


    bool initialize() {
        static bool initialized = false;
        if (initialized)
            return true;

        const HMODULE hGameAssembly = GetModuleHandleA("GameAssembly.dll");
        if (!hGameAssembly)
            return false;

        // debugger
        IL2CPP_GET_ADDR(is_debugger_attached, "IL2CPP_is_debugger_attached", hGameAssembly);

        // internal calls and runtime
        IL2CPP_GET_ADDR(add_icall, "IL2CPP_add_internal_call", hGameAssembly);
        IL2CPP_GET_ADDR(resolve_icall, "IL2CPP_resolve_icall", hGameAssembly);
        IL2CPP_GET_ADDR(init_class, "IL2CPP_runtime_class_init", hGameAssembly);
        IL2CPP_GET_ADDR(init_object, "IL2CPP_runtime_object_init", hGameAssembly);

        // thread, domains, and assemblies
        IL2CPP_GET_ADDR(thread_attach, "IL2CPP_thread_attach", hGameAssembly);
        IL2CPP_GET_ADDR(get_domain, "IL2CPP_domain_get", hGameAssembly);
        IL2CPP_GET_ADDR(get_assemblies, "IL2CPP_domain_get_assemblies", hGameAssembly);

        // images
        IL2CPP_GET_ADDR(get_image, "IL2CPP_assembly_get_image", hGameAssembly);
        IL2CPP_GET_ADDR(get_image_name, "IL2CPP_image_get_name", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_from_image, "IL2CPP_image_get_class", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_count, "IL2CPP_image_get_class_count", hGameAssembly);

        // objects
        IL2CPP_GET_ADDR(object_create, "IL2CPP_object_new", hGameAssembly);
        IL2CPP_GET_ADDR(object_box, "IL2CPP_object_box", hGameAssembly);
        IL2CPP_GET_ADDR(object_unbox, "IL2CPP_object_unbox", hGameAssembly);
        IL2CPP_GET_ADDR(object_get_size, "IL2CPP_object_get_size", hGameAssembly);
        IL2CPP_GET_ADDR(object_get_class, "IL2CPP_object_get_class", hGameAssembly);
        IL2CPP_GET_ADDR(object_get_virtual_method, "IL2CPP_object_get_virtual_method", hGameAssembly);

        // arrays
        IL2CPP_GET_ADDR(get_array_element_size, "IL2CPP_array_element_size", hGameAssembly);
        IL2CPP_GET_ADDR(get_array_class, "IL2CPP_array_class_get", hGameAssembly);
        IL2CPP_GET_ADDR(get_array_length, "IL2CPP_array_length", hGameAssembly);
        IL2CPP_GET_ADDR(array_new, "IL2CPP_array_new", hGameAssembly);

        // classes
        IL2CPP_GET_ADDR(get_class, "IL2CPP_class_from_name", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_name, "IL2CPP_class_get_name", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_namespace, "IL2CPP_class_get_namespace", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_type, "IL2CPP_class_enum_basetype", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_generic, "IL2CPP_class_is_generic", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_inflated, "IL2CPP_class_is_inflated", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_has_parent, "IL2CPP_has_parent", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_get_parent, "IL2CPP_class_get_parent", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_is_subclass_of, "IL2CPP_class_is_subclass_of", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_from_type, "IL2CPP_class_from_IL2CPP_type", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_event_info, "IL2CPP_class_get_events", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_interfaces, "IL2CPP_class_get_interfaces", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_properties, "IL2CPP_class_get_properties", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_property_from_name, "IL2CPP_class_property_from_name", hGameAssembly);

        // types
        IL2CPP_GET_ADDR(get_nested_types, "IL2CPP_class_get_nested_types", hGameAssembly);
        IL2CPP_GET_ADDR(get_type_attributes, "IL2CPP_type_get_attrs", hGameAssembly);
        IL2CPP_GET_ADDR(get_type_class, "IL2CPP_type_get_class_or_element_class", hGameAssembly);
        IL2CPP_GET_ADDR(get_type_name, "IL2CPP_type_get_name", hGameAssembly);
        IL2CPP_GET_ADDR(get_type_object, "IL2CPP_type_get_object", hGameAssembly);
        IL2CPP_GET_ADDR(get_type_type, "IL2CPP_type_get_type", hGameAssembly);
        IL2CPP_GET_ADDR(get_type_equals, "IL2CPP_type_equals", hGameAssembly);
        IL2CPP_GET_ADDR(get_type_is_byref, "IL2CPP_type_is_byref", hGameAssembly);

        // methods
        IL2CPP_GET_ADDR(method_call, "IL2CPP_runtime_invoke", hGameAssembly);
        IL2CPP_GET_ADDR(get_method, "IL2CPP_class_get_method_from_name", hGameAssembly);
        IL2CPP_GET_ADDR(get_methods, "IL2CPP_class_get_methods", hGameAssembly);
        IL2CPP_GET_ADDR(get_method_name, "IL2CPP_method_get_name", hGameAssembly);
        IL2CPP_GET_ADDR(get_method_is_generic, "IL2CPP_method_is_generic", hGameAssembly);
        IL2CPP_GET_ADDR(get_method_is_inflated, "IL2CPP_method_is_inflated", hGameAssembly);
        IL2CPP_GET_ADDR(get_method_is_instance, "IL2CPP_method_is_instance", hGameAssembly);
        IL2CPP_GET_ADDR(get_method_param_count, "IL2CPP_method_get_param_count", hGameAssembly);
        IL2CPP_GET_ADDR(get_method_param_name, "IL2CPP_method_get_param_name", hGameAssembly);
        IL2CPP_GET_ADDR(get_method_param_type, "IL2CPP_get_param", hGameAssembly);
        IL2CPP_GET_ADDR(get_method_return_type, "IL2CPP_method_get_return_type", hGameAssembly);

        // fields
        IL2CPP_GET_ADDR(get_fields, "IL2CPP_class_get_fields", hGameAssembly);
        IL2CPP_GET_ADDR(get_field, "IL2CPP_class_get_field_from_name", hGameAssembly);
        IL2CPP_GET_ADDR(get_field_offset, "IL2CPP_field_get_offset", hGameAssembly);
        IL2CPP_GET_ADDR(get_field_count, "IL2CPP_class_num_fields", hGameAssembly);
        IL2CPP_GET_ADDR(get_field_name, "IL2CPP_field_get_name", hGameAssembly);
        IL2CPP_GET_ADDR(get_field_object, "IL2CPP_field_get_value_object", hGameAssembly);
        IL2CPP_GET_ADDR(get_field_type, "IL2CPP_field_get_type", hGameAssembly);
        IL2CPP_GET_ADDR(get_static_field, "IL2CPP_field_static_get_value", hGameAssembly);
        IL2CPP_GET_ADDR(set_static_field, "IL2CPP_field_static_set_value", hGameAssembly);
        IL2CPP_GET_ADDR(get_field_flags, "IL2CPP_field_get_flags", hGameAssembly);

        // strings
        IL2CPP_GET_ADDR(get_string_chars, "IL2CPP_string_chars", hGameAssembly);
        IL2CPP_GET_ADDR(create_string, "IL2CPP_string_new", hGameAssembly);

        initialized = true;
        return true;
    }   // initialize()


    class Method : public IMethod {
        private:
            void* obj;
        public:
            Method() = default;
            Method(void* obj) : obj(obj) {}
            // todo: these objects are just type-casted pointers returned from IL2CPP functions.
            // they get passed as "this" to various IL2CPP functions. however, we expect these to be classes
            // with virtual dispatch, but there's no actual vtable since the objects are not ours, but IL2CPP's.
            // so we need to create new objects and store the IL2CPP pointers in this->obj (which get passed to
            // IL2CPP functions instead of this)

            virtual void* invoke_static(void** params = nullptr) const {
                if (IL2CPP::method_call == nullptr || this == nullptr)
                    return nullptr;

                void* exc = nullptr;

                const auto result = IL2CPP::method_call(this, nullptr, params, &exc);

                if (exc != nullptr) {
                    GHLOG(GH_LOG_WARNING, GH_TAG) << "Method::invoke_static() raised exception: " << exc << "\n";
                }

                return result;
            }

            virtual const void* invoke(void* obj, void** params = nullptr) {
                if (IL2CPP::method_call == nullptr || this == nullptr)
                    return nullptr;

                void* exc = nullptr;

                const auto result = IL2CPP::method_call(this, obj, params, &exc);

                if (exc != nullptr) {
                    GHLOG(GH_LOG_WARNING, GH_TAG) << "Method::invoke(" << obj << ") raised exception: " << exc << "\n";
                }

                return result;
            }

            virtual uint32_t get_param_count() const {
                if (IL2CPP::get_method_param_count == nullptr || this == nullptr)
                    return -1;

                return IL2CPP::get_method_param_count(this);
            }

            virtual const char* get_param_name(int index) const {
                if (IL2CPP::get_method_param_name == nullptr || this == nullptr)
                    return nullptr;

                return IL2CPP::get_method_param_name(this, index);
            }

            virtual const char* get_param_type(int index) const {
                if (IL2CPP::get_method_param_type == nullptr || this == nullptr)
                    return nullptr;

                return IL2CPP::get_method_param_type(this, index);
            }

            virtual bool is_generic() const {
                if (IL2CPP::get_method_is_generic == nullptr || this == nullptr)
                    return false;

                return IL2CPP::get_method_is_generic(this);
            }

            virtual bool is_inflated() const {
                if (IL2CPP::get_method_is_inflated == nullptr || this == nullptr)
                    return false;

                return IL2CPP::get_method_is_inflated(this);
            }

            virtual bool is_instance() const {
                if (IL2CPP::get_method_is_instance == nullptr || this == nullptr)
                    return false;

                return IL2CPP::get_method_is_instance(this);
            }

            virtual const char* get_name() const {
                if (IL2CPP::get_method_name == nullptr)
                    return nullptr;;

                return IL2CPP::get_method_name(this);
            }
    };  // class Method


    class Field : public IField {
        public:
            Field() = default;

            virtual void* get_as_static() const {
                if (IL2CPP::get_static_field == nullptr)
                    return nullptr;

                void* val = NULL;

                IL2CPP::get_static_field(this, &val);

                return val;
            }

            virtual void* set_static_value(void* value) const {
                if (IL2CPP::set_static_field == nullptr)
                    return nullptr;

                return IL2CPP::set_static_field(this, value);
            }

            virtual size_t get_offset() const {
                if (IL2CPP::get_field_offset == nullptr)
                    return 0x0;

                return IL2CPP::get_field_offset(this);
            }
                
            virtual void* get_object(const void* obj) const {
                if (IL2CPP::get_field_object == nullptr)
                    return nullptr;

                return IL2CPP::get_field_object(this, obj);
            }

            virtual void* get_type() const {
                if (IL2CPP::get_field_type == nullptr)
                    return nullptr;

                return IL2CPP::get_field_type(this);
            }

            virtual const char* get_attribute() const {
                if (IL2CPP::get_field_flags == nullptr)
                    return nullptr;

                static char attr[100];
                memset(attr, 0, sizeof(attr));

                auto attrs = IL2CPP::get_field_flags(this);

                auto access = attrs & FIELD_ATTRIBUTE_FIELD_ACCESS_MASK;
                switch (access) {
                    case FIELD_ATTRIBUTE_PRIVATE:
                        strcpy(attr, "private ");
                        break;
                    case FIELD_ATTRIBUTE_PUBLIC:
                        strcpy(attr, "public ");
                        break;
                    case FIELD_ATTRIBUTE_FAMILY:
                        strcpy(attr, "protected ");
                        break;
                    case FIELD_ATTRIBUTE_ASSEMBLY:
                    case FIELD_ATTRIBUTE_FAM_AND_ASSEM:
                        strcpy(attr, "internal ");
                        break;
                    case FIELD_ATTRIBUTE_FAM_OR_ASSEM:
                        strcpy(attr, "protected internal ");
                        break;
                }

                if ((attrs & FIELD_ATTRIBUTE_LITERAL) == FIELD_ATTRIBUTE_LITERAL) {
                    strcpy(attr, "const ");
                }
                else if ((attrs & FIELD_ATTRIBUTE_STATIC) == FIELD_ATTRIBUTE_STATIC) {
                    strcpy(attr, "static ");
                }
                else if ((attrs & FIELD_ATTRIBUTE_INIT_ONLY) == FIELD_ATTRIBUTE_INIT_ONLY) {
                    strcpy(attr, "readonly ");
                }

                return attr;
            }

            virtual const char* get_name() const {
                if (IL2CPP::get_field_name == nullptr)
                    return nullptr;

                return IL2CPP::get_field_name(this);
            }
    };  // class Field


    class Class : public IClass {
        public:
            Class() = default;

            virtual IMethod* get_method(const char* name, int argc = 0) const {
                if (IL2CPP::get_method == nullptr)
                    return nullptr;

                auto method = IL2CPP::get_method(this, name, argc);

                return (IMethod*)method;
            }

            virtual IField* get_field(const char* name) const {
                if (IL2CPP::get_field == nullptr)
                    return nullptr;

                auto field = IL2CPP::get_field(this, name);

                return (IField*)field;
            }

            virtual std::vector<IField*> get_fields() const {
                static std::vector<IField*> m_fields{};
                if (m_fields.size())
                    return m_fields;

                if (IL2CPP::get_field_count == nullptr || IL2CPP::get_fields == nullptr)
                    return m_fields;

                const size_t count = IL2CPP::get_field_count(this);
                if (count)
                    m_fields.resize(count);

                void* iter = NULL;
                void* field = nullptr;

                size_t index = 0;

                while (field = IL2CPP::get_fields(this, &iter)) {
                    if (!field || field == NULL)
                        continue;

                    m_fields[index++] = (IField*)field;
                }

                return m_fields;
            }

            virtual IClass* get_nested_class(const char* name) const {
                if (IL2CPP::get_nested_types == nullptr ||
                    IL2CPP::get_class_name == nullptr)
                    return nullptr;

                void* iter = NULL;

                while (auto type = IL2CPP::get_nested_types(this, &iter)) {
                    const auto class_name = IL2CPP::get_class_name(type);

                    if (!strcmp(class_name, name))
                        return type;
                }

                return nullptr;
            }

            virtual const char* get_name() const {
                if (IL2CPP::get_class_name == nullptr)
                    return nullptr;

                return IL2CPP::get_class_name(this);
            }
    };  // class Class


    class Type : public IType {
        public:
            Type() = default;

            virtual IClass* get_class() const {
                if (IL2CPP::get_type_class == nullptr)
                    return nullptr;

                return IL2CPP::get_type_class(this);
            }

            virtual void* get_object() const {
                if (IL2CPP::get_type_object == nullptr)
                    return nullptr;

                return IL2CPP::get_type_object(this);
            }

            virtual int get_type() const {
                if (IL2CPP::get_type_type == nullptr)
                    return -1;

                return IL2CPP::get_type_type(this);
            }

            virtual uint32_t get_attributes() const {
                if (IL2CPP::get_type_attributes == nullptr)
                    return 0;

                return IL2CPP::get_type_attributes(this);
            }

            virtual bool compare_type(IType* type) {
                if (IL2CPP::get_type_equals == nullptr)
                    return false;

                return IL2CPP::get_type_equals(this, type);
            }

            virtual bool is_by_ref();
            virtual const char* get_name() const {
                if (IL2CPP::get_type_name == nullptr)
                    return nullptr;

                return IL2CPP::get_type_name(this);
            }

    };  // class Type

}   // namespace IL2CPP
