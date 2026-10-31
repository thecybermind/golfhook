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
 * I made this a virtual interface scheme so that plugins don't need [separate] implementations of the IL2CPP stuff.
 * Each object captures the actual pointer coming from the IL2CPP functions, and also stores "this" in a static
 * vector for deleting during shutdown.
 * 
 * The general way other libraries do it is just cast the pointers from IL2CPP into the desired types, but this
 * requires each module to have its own knowledge of the internal implementation (including DLL export pointers)
 * which does not work well across plugin boundaries.
 * 
 * Also the Aetherim Image class references the first member of the IL2CPP object directly as a const char* name
 * that is never set. Since we have separate objects now, we must call il2cpp_image_get_name to get the name.
 */

#define _CRT_SECURE_NO_WARNINGS 1
#include <cstring>
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
    IL2CPP_API(add_icall, void, (const char* name, const void* method_ptr));
    IL2CPP_API(resolve_icall, void, (const char* name));
    IL2CPP_API(init_class, void, (void* _class));
    IL2CPP_API(init_object, void, (void* obj));

    // thread, domain, and assembly
    IL2CPP_API(thread_attach, void*, (void* domain));
    IL2CPP_API(get_domain, void*, (void));
    IL2CPP_API(get_assemblies, void**, (const void* domain, size_t* count));

    // images
    IL2CPP_API(get_image, void*, (const void* assembly));
    IL2CPP_API(get_image_name, const char*, (const void* image));
    IL2CPP_API(get_class_count, size_t, (const void* image));
    IL2CPP_API(get_class_from_image, void*, (const void* image, size_t index));

    // objects
    IL2CPP_API(object_create, void*, (void* domain, void* _class));
    IL2CPP_API(object_unbox, void*, (void* obj));
    IL2CPP_API(object_box, void*, (void* _class, void* data));
    IL2CPP_API(object_get_size, uint32_t, (void* obj));
    IL2CPP_API(object_get_class, void*, (void* obj));
    IL2CPP_API(object_get_virtual_method, void*, (void* obj, void* method));

    // arrays
    IL2CPP_API(get_array_element_size, int, (const void* array_class));
    IL2CPP_API(get_array_class, void*, (const void* element_class, uint32_t rank));
    IL2CPP_API(get_array_length, uint32_t, (const void* array));
    IL2CPP_API(array_new, void*, (void* element_type_infof, size_t length));

    // class
    IL2CPP_API(get_class, void*, (const void* image, const char* _namespace, const char* name));
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
    IL2CPP_API(get_nested_types, void*, (const void* _class, void*));
    IL2CPP_API(get_type_attributes, uint32_t, (const void* type));
    IL2CPP_API(get_type_class, void*, (const void* type));
    IL2CPP_API(get_type_name, const char*, (const void* type));
    IL2CPP_API(get_type_object, void*, (const void* type));
    IL2CPP_API(get_type_type, int, (const void* type));
    IL2CPP_API(get_type_equals, bool, (void* type_1, void* type_2));
    IL2CPP_API(get_type_is_byref, bool, (void* type));

    // methods
    IL2CPP_API(method_call, void*, (const void* method, void* obj, void** params, void** excption));
    IL2CPP_API(get_method, void*, (const void* _class, const char* name, int params));
    IL2CPP_API(get_methods, void*, (const void* _class, void* iter));
    IL2CPP_API(get_method_name, const char*, (const void* method));
    IL2CPP_API(get_method_is_generic, bool, (const void* method));
    IL2CPP_API(get_method_is_inflated, bool, (const void* method));
    IL2CPP_API(get_method_is_instance, bool, (const void* method));
    IL2CPP_API(get_method_param_count, int, (const void* method));
    IL2CPP_API(get_method_param_name, const char*, (const void* method, int index));
    IL2CPP_API(get_method_param_type, const char*, (const void* method, int index));
    IL2CPP_API(get_method_return_type, void*, (const void* method));

    // fields
    IL2CPP_API(get_fields, void*, (const void* _class, void* iter));
    IL2CPP_API(get_field, void*, (const void* _class, const char* name));
    IL2CPP_API(get_field_offset, size_t, (const void* field));
    IL2CPP_API(get_field_count, size_t, (const void* _class));
    IL2CPP_API(get_field_name, const char*, (const void* field));
    IL2CPP_API(get_field_value, void, (const void* self, const void* field, void* value));
    IL2CPP_API(set_field_value, void, (const void* self, const void* field, void* value));
    IL2CPP_API(get_field_object, void*, (const void* field, const void* self));
    IL2CPP_API(set_field_object, void, (const void* field, const void* self, void* value));
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
        IL2CPP_GET_ADDR(is_debugger_attached, "il2cpp_is_debugger_attached", hGameAssembly);

        // internal calls and runtime
        IL2CPP_GET_ADDR(add_icall, "il2cpp_add_internal_call", hGameAssembly);
        IL2CPP_GET_ADDR(resolve_icall, "il2cpp_resolve_icall", hGameAssembly);
        IL2CPP_GET_ADDR(init_class, "il2cpp_runtime_class_init", hGameAssembly);
        IL2CPP_GET_ADDR(init_object, "il2cpp_runtime_object_init", hGameAssembly);

        // thread, domains, and assemblies
        IL2CPP_GET_ADDR(thread_attach, "il2cpp_thread_attach", hGameAssembly);
        IL2CPP_GET_ADDR(get_domain, "il2cpp_domain_get", hGameAssembly);
        IL2CPP_GET_ADDR(get_assemblies, "il2cpp_domain_get_assemblies", hGameAssembly);

        // images
        IL2CPP_GET_ADDR(get_image, "il2cpp_assembly_get_image", hGameAssembly);
        IL2CPP_GET_ADDR(get_image_name, "il2cpp_image_get_name", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_from_image, "il2cpp_image_get_class", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_count, "il2cpp_image_get_class_count", hGameAssembly);

        // objects
        IL2CPP_GET_ADDR(object_create, "il2cpp_object_new", hGameAssembly);
        IL2CPP_GET_ADDR(object_box, "il2cpp_object_box", hGameAssembly);
        IL2CPP_GET_ADDR(object_unbox, "il2cpp_object_unbox", hGameAssembly);
        IL2CPP_GET_ADDR(object_get_size, "il2cpp_object_get_size", hGameAssembly);
        IL2CPP_GET_ADDR(object_get_class, "il2cpp_object_get_class", hGameAssembly);
        IL2CPP_GET_ADDR(object_get_virtual_method, "il2cpp_object_get_virtual_method", hGameAssembly);

        // arrays
        IL2CPP_GET_ADDR(get_array_element_size, "il2cpp_array_element_size", hGameAssembly);
        IL2CPP_GET_ADDR(get_array_class, "il2cpp_array_class_get", hGameAssembly);
        IL2CPP_GET_ADDR(get_array_length, "il2cpp_array_length", hGameAssembly);
        IL2CPP_GET_ADDR(array_new, "il2cpp_array_new", hGameAssembly);

        // classes
        IL2CPP_GET_ADDR(get_class, "il2cpp_class_from_name", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_name, "il2cpp_class_get_name", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_namespace, "il2cpp_class_get_namespace", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_type, "il2cpp_class_enum_basetype", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_generic, "il2cpp_class_is_generic", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_inflated, "il2cpp_class_is_inflated", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_has_parent, "il2cpp_has_parent", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_get_parent, "il2cpp_class_get_parent", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_is_subclass_of, "il2cpp_class_is_subclass_of", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_from_type, "il2cpp_class_from_IL2CPP_type", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_event_info, "il2cpp_class_get_events", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_interfaces, "il2cpp_class_get_interfaces", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_properties, "il2cpp_class_get_properties", hGameAssembly);
        IL2CPP_GET_ADDR(get_class_property_from_name, "il2cpp_class_property_from_name", hGameAssembly);

        // types
        IL2CPP_GET_ADDR(get_nested_types, "il2cpp_class_get_nested_types", hGameAssembly);
        IL2CPP_GET_ADDR(get_type_attributes, "il2cpp_type_get_attrs", hGameAssembly);
        IL2CPP_GET_ADDR(get_type_class, "il2cpp_type_get_class_or_element_class", hGameAssembly);
        IL2CPP_GET_ADDR(get_type_name, "il2cpp_type_get_name", hGameAssembly);
        IL2CPP_GET_ADDR(get_type_object, "il2cpp_type_get_object", hGameAssembly);
        IL2CPP_GET_ADDR(get_type_type, "il2cpp_type_get_type", hGameAssembly);
        IL2CPP_GET_ADDR(get_type_equals, "il2cpp_type_equals", hGameAssembly);
        IL2CPP_GET_ADDR(get_type_is_byref, "il2cpp_type_is_byref", hGameAssembly);

        // methods
        IL2CPP_GET_ADDR(method_call, "il2cpp_runtime_invoke", hGameAssembly);
        IL2CPP_GET_ADDR(get_method, "il2cpp_class_get_method_from_name", hGameAssembly);
        IL2CPP_GET_ADDR(get_methods, "il2cpp_class_get_methods", hGameAssembly);
        IL2CPP_GET_ADDR(get_method_name, "il2cpp_method_get_name", hGameAssembly);
        IL2CPP_GET_ADDR(get_method_is_generic, "il2cpp_method_is_generic", hGameAssembly);
        IL2CPP_GET_ADDR(get_method_is_inflated, "il2cpp_method_is_inflated", hGameAssembly);
        IL2CPP_GET_ADDR(get_method_is_instance, "il2cpp_method_is_instance", hGameAssembly);
        IL2CPP_GET_ADDR(get_method_param_count, "il2cpp_method_get_param_count", hGameAssembly);
        IL2CPP_GET_ADDR(get_method_param_name, "il2cpp_method_get_param_name", hGameAssembly);
        IL2CPP_GET_ADDR(get_method_param_type, "il2cpp_get_param", hGameAssembly);
        IL2CPP_GET_ADDR(get_method_return_type, "il2cpp_method_get_return_type", hGameAssembly);

        // fields
        IL2CPP_GET_ADDR(get_fields, "il2cpp_class_get_fields", hGameAssembly);
        IL2CPP_GET_ADDR(get_field, "il2cpp_class_get_field_from_name", hGameAssembly);
        IL2CPP_GET_ADDR(get_field_offset, "il2cpp_field_get_offset", hGameAssembly);
        IL2CPP_GET_ADDR(get_field_count, "il2cpp_class_num_fields", hGameAssembly);
        IL2CPP_GET_ADDR(get_field_name, "il2cpp_field_get_name", hGameAssembly);
        IL2CPP_GET_ADDR(get_field_object, "il2cpp_field_get_value_object", hGameAssembly);
        IL2CPP_GET_ADDR(set_field_object, "il2cpp_field_set_value_object", hGameAssembly);
        IL2CPP_GET_ADDR(get_field_value, "il2cpp_field_get_value", hGameAssembly);
        IL2CPP_GET_ADDR(set_field_value, "il2cpp_field_set_value", hGameAssembly);
        IL2CPP_GET_ADDR(get_field_type, "il2cpp_field_get_type", hGameAssembly);
        IL2CPP_GET_ADDR(get_static_field, "il2cpp_field_static_get_value", hGameAssembly);
        IL2CPP_GET_ADDR(set_static_field, "il2cpp_field_static_set_value", hGameAssembly);
        IL2CPP_GET_ADDR(get_field_flags, "il2cpp_field_get_flags", hGameAssembly);

        // strings
        IL2CPP_GET_ADDR(get_string_chars, "il2cpp_string_chars", hGameAssembly);
        IL2CPP_GET_ADDR(create_string, "il2cpp_string_new", hGameAssembly);

        initialized = true;
        return true;
    }   // initialize()


    class Method : public IMethod {
        private:
            void* obj;
            static std::vector<IMethod*> objects;
        public:
            Method() = delete;
            Method(void* obj) : obj(obj) { objects.push_back(this); }

            static void Cleanup() {
                for (auto i : objects) {
                    delete i;
                }
                objects.clear();
            }

            virtual void* invoke_static(void** params = nullptr) const {
                if (!IL2CPP::method_call || !obj)
                    return nullptr;

                void* exc = nullptr;

                const auto result = IL2CPP::method_call(obj, nullptr, params, &exc);

                if (exc != nullptr) {
                    GHLOG(GH_LOG_WARNING, GH_TAG) << "Method::invoke_static() raised exception: " << exc << "\n";
                }

                return result;
            }

            virtual const void* invoke(void* self, void** params = nullptr) {
                if (!IL2CPP::method_call || !obj)
                    return nullptr;

                void* exc = nullptr;

                const auto result = IL2CPP::method_call(obj, self, params, &exc);

                if (exc != nullptr) {
                    GHLOG(GH_LOG_WARNING, GH_TAG) << "Method::invoke(" << self << ") raised exception: " << exc << "\n";
                }

                return result;
            }

            virtual uint32_t get_param_count() const {
                if (!IL2CPP::get_method_param_count || !obj)
                    return -1;

                return IL2CPP::get_method_param_count(obj);
            }

            virtual const char* get_param_name(int index) const {
                if (!IL2CPP::get_method_param_name || !obj)
                    return nullptr;

                return IL2CPP::get_method_param_name(obj, index);
            }

            virtual const char* get_param_type(int index) const {
                if (!IL2CPP::get_method_param_type || !obj)
                    return nullptr;

                return IL2CPP::get_method_param_type(obj, index);
            }

            virtual bool is_generic() const {
                if (!IL2CPP::get_method_is_generic || !obj)
                    return false;

                return IL2CPP::get_method_is_generic(obj);
            }

            virtual bool is_inflated() const {
                if (!IL2CPP::get_method_is_inflated || !obj)
                    return false;

                return IL2CPP::get_method_is_inflated(obj);
            }

            virtual bool is_instance() const {
                if (!IL2CPP::get_method_is_instance || !obj)
                    return false;

                return IL2CPP::get_method_is_instance(obj);
            }

            virtual const char* get_name() const {
                if (!IL2CPP::get_method_name || !obj)
                    return nullptr;;

                return IL2CPP::get_method_name(obj);
            }
    };  // class Method
    std::vector<IMethod*> Method::objects;


    class Field : public IField {
        private:
            void* obj;
            static std::vector<IField*> objects;
        public:
            Field() = delete;
            Field(void* obj) : obj(obj) { objects.push_back(this); }

            static void Cleanup() {
                for (auto i : objects) {
                    delete i;
                }
                objects.clear();
            }

            virtual void* get_static_value() const {
                if (!IL2CPP::get_static_field || !obj)
                    return nullptr;

                void* val = NULL;

                IL2CPP::get_static_field(obj, &val);

                return val;
            }

            virtual void* set_static_value(void* value) const {
                if (!IL2CPP::set_static_field || !obj)
                    return nullptr;

                return IL2CPP::set_static_field(obj, value);
            }

            virtual size_t get_offset() const {
                if (!IL2CPP::get_field_offset || !obj)
                    return 0x0;

                return IL2CPP::get_field_offset(obj);
            }
            
            virtual void* get_object(const void* self) const {
                if (!IL2CPP::get_field_object || !obj)
                    return nullptr;

                return IL2CPP::get_field_object(obj, self);
            }

            virtual void* get_type() const {
                if (!IL2CPP::get_field_type || !obj)
                    return nullptr;

                return IL2CPP::get_field_type(obj);
            }

            virtual const char* get_attribute() const {
                if (!IL2CPP::get_field_flags || !obj)
                    return nullptr;

                // static buffer to return but need to re-get data every time
                // if we store it in a class member, this can not be a const function
                static char attr[100];
                memset(attr, 0, sizeof(attr));

                auto attrs = IL2CPP::get_field_flags(obj);

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
                    strcat(attr, "const ");
                }
                else if ((attrs & FIELD_ATTRIBUTE_STATIC) == FIELD_ATTRIBUTE_STATIC) {
                    strcat(attr, "static ");
                }
                else if ((attrs & FIELD_ATTRIBUTE_INIT_ONLY) == FIELD_ATTRIBUTE_INIT_ONLY) {
                    strcat(attr, "readonly ");
                }

                return attr;
            }

            virtual const char* get_name() const {
                if (!IL2CPP::get_field_name || !obj)
                    return nullptr;

                return IL2CPP::get_field_name(obj);
            }
    };  // class Field
    std::vector<IField*> Field::objects;


    class Class : public IClass {
        private:
            void* obj;
            static std::vector<IClass*> objects;
        public:
            Class() = delete;
            Class(void* obj) : obj(obj) { objects.push_back(this); }

            static void Cleanup() {
                for (auto i : objects) {
                    delete i;
                }
                objects.clear();
            }

            virtual IMethod* get_method(const char* name, int argc = 0) const {
                if (!IL2CPP::get_method || !obj)
                    return nullptr;

                auto method = IL2CPP::get_method(obj, name, argc);

                return new Method(method);
            }

            virtual IField* get_field(const char* name) const {
                if (!IL2CPP::get_field || !obj)
                    return nullptr;

                auto field = IL2CPP::get_field(obj, name);

                return new Field(field);
            }

            virtual std::vector<IField*> get_fields() const {
                static std::vector<IField*> m_fields{};
                if (m_fields.size())
                    return m_fields;

                if (!IL2CPP::get_field_count || !IL2CPP::get_fields || !obj)
                    return m_fields;

                const size_t count = IL2CPP::get_field_count(obj);
                if (count)
                    m_fields.resize(count);

                void* iter = NULL;
                void* field = nullptr;

                size_t index = 0;

                while (field = IL2CPP::get_fields(obj, &iter)) {
                    m_fields[index++] = new Field(field);
                }

                return m_fields;
            }

            virtual IClass* get_nested_class(const char* name) const {
                if (!IL2CPP::get_nested_types || !IL2CPP::get_class_name || !obj)
                    return nullptr;

                void* iter = NULL;

                while (auto type = IL2CPP::get_nested_types(obj, &iter)) {
                    const auto class_name = IL2CPP::get_class_name(type);

                    if (strcmp(class_name, name) == 0)
                        return new Class(type);
                }

                return nullptr;
            }

            virtual const char* get_name() const {
                if (!IL2CPP::get_class_name || !obj)
                    return nullptr;

                return IL2CPP::get_class_name(obj);
            }
    };  // class Class
    std::vector<IClass*> Class::objects;


    class Type : public IType {
        private:
            void* obj;
            static std::vector<IType*> objects;
        public:
            Type() = delete;
            Type(void* obj) : obj(obj) { objects.push_back(this); }

            static void Cleanup() {
                for (auto i : objects) {
                    delete i;
                }
                objects.clear();
            }

            virtual IClass* get_class() const {
                if (!IL2CPP::get_type_class || !obj)
                    return nullptr;

                auto type = IL2CPP::get_type_class(obj);

                return new Class(type);
            }

            virtual void* get_object() const {
                if (!IL2CPP::get_type_object || !obj)
                    return nullptr;

                return IL2CPP::get_type_object(obj);
            }

            virtual int get_type() const {
                if (!IL2CPP::get_type_type || !obj)
                    return -1;

                return IL2CPP::get_type_type(obj);
            }

            virtual uint32_t get_attributes() const {
                if (!IL2CPP::get_type_attributes || !obj)
                    return 0;

                return IL2CPP::get_type_attributes(obj);
            }

            virtual bool compare_type(IType* other) {
                if (!IL2CPP::get_type_equals || !obj)
                    return false;

                // we need to look into other's object
                Type* _other = (Type*)other;

                return IL2CPP::get_type_equals(obj, _other->obj);
            }

            virtual bool is_by_ref() {
                if (!IL2CPP::get_type_is_byref || !obj)
                    return false;

                return IL2CPP::get_type_is_byref(obj);
            }

            virtual const char* get_name() const {
                if (!IL2CPP::get_type_name || !obj)
                    return nullptr;

                return IL2CPP::get_type_name(obj);
            }
    };  // class Type
    std::vector<IType*> Type::objects;


    class Image : public IImage {
        private:
            void* obj;
            static std::vector<IImage*> objects;
        public:
            Image() = delete;
            Image(void* obj) : obj(obj) { objects.push_back(this); }

            static void Cleanup() {
                for (auto i : objects) {
                    delete i;
                }
                objects.clear();
            }

            virtual IClass* get_class(const char* name, const char* name_space = "") const {
                if (!IL2CPP::get_class || !obj)
                    return nullptr;

                auto _class = IL2CPP::get_class(obj, name_space, name);
                return new Class(_class);
            }

            virtual std::vector<IClass*> get_classes() const {
                static std::vector<IClass*> m_classes{};
                if (m_classes.size())
                    return m_classes;

                if (!IL2CPP::get_class_count || !IL2CPP::get_class_from_image || !IL2CPP::get_class_name || !obj)
                    return m_classes;

                size_t count = IL2CPP::get_class_count(obj);
                if (count)
                    m_classes.resize(count);

                size_t valid_classes = 0;

                for (size_t index = 0U; index < count; ++index) {
                    auto address = IL2CPP::get_class_from_image(obj, index);
                    if (!address)
                        continue;

                    auto name = IL2CPP::get_class_name(address);
                    if (!name)
                        continue;

                    if (!strcmp(name, "<Module>"))
                        continue;

                    m_classes[valid_classes++] = new Class(address);
                }

                return m_classes;
            }

            virtual const char* get_name() const {
                if (!IL2CPP::get_image_name || !obj)
                    return nullptr;

                return IL2CPP::get_image_name(obj);
            }
    };  // class Image
    std::vector<IImage*> Image::objects;

    
    Manager::Manager() : m_images({}) {}


    Manager::~Manager() {
        for (auto& i : m_images) {
            delete i;
        }
        m_images.clear();
    }

    void Manager::InitAssemblies() {
        (void)initialize();

        const auto domain = IL2CPP::get_domain();
        IL2CPP::thread_attach(domain);

        size_t count = 0U;

        const auto assemblies = IL2CPP::get_assemblies(domain, &count);

        for (size_t index = 0U; index < count; ++index) {
            auto image = IL2CPP::get_image(assemblies[index]);
            if (image)
                m_images.push_back(new Image(image));
        }
    }


    IImage* Manager::get_image(const char* name) const {
        for (const auto image : m_images) {
            if (!strcmp(name, image->get_name()))
                return image;
        }

        return nullptr;
    }


    std::vector<IImage*> Manager::get_images() const {
        return m_images;
    }

    
    bool Manager::is_debugger_attached() const {
        return IL2CPP::is_debugger_attached();
    }


    void shutdown() {
        Method::Cleanup();
        Field::Cleanup();
        Class::Cleanup();
        Type::Cleanup();
        Image::Cleanup();
    }
}   // namespace IL2CPP
