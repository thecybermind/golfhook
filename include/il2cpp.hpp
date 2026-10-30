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
 */

#ifndef GOLFHOOK_IL2CPP_HPP
#define GOLFHOOK_IL2CPP_HPP

#include <cstdint>
#include <vector>

namespace IL2CPP {
    bool initialize();
    void shutdown();

    class IMethod {
        public:
            virtual void* invoke_static(void** params = nullptr) const = 0;
            virtual const void* invoke(void* obj, void** params = nullptr) = 0;
            virtual uint32_t get_param_count() const = 0;
            virtual const char* get_param_name(int index) const = 0;
            virtual const char* get_param_type(int index) const = 0;
            virtual bool is_generic() const = 0;
            virtual bool is_inflated() const = 0;
            virtual bool is_instance() const = 0;
            virtual const char* get_name() const = 0;
    };

    class IField {
        public:
            virtual void* get_as_static() const = 0;
            virtual void* set_static_value(void* value) const = 0;
            virtual size_t get_offset() const = 0;
            virtual void* get_object(const void* obj) const = 0;
            virtual void* get_type() const = 0;
            virtual const char* get_attribute() const = 0;
            virtual const char* get_name() const = 0;
    };

    class IClass {
        public:
            virtual IMethod* get_method(const char* name, int argc = 0) const = 0;
            virtual IField* get_field(const char* name) const = 0;
            virtual std::vector<IField*> get_fields() const = 0;
            virtual IClass* get_nested_class(const char* name) const = 0;
            virtual const char* get_name() const = 0;
    };

    class IType {
        public:
            virtual IClass* get_class() const = 0;
            virtual void* get_object() const = 0;
            virtual int get_type() = 0;
            virtual uint32_t get_attributes() const = 0;
            virtual bool compare_type(IType* type) = 0;
            virtual bool is_by_ref() = 0;
            virtual const char* get_name() const = 0;
    };

    class IImage {
        public:
            virtual IClass* get_class(const char* name, const char* name_space = "") const = 0;
            virtual std::vector<IClass*> get_classes() const = 0;
            virtual const char* get_name() const = 0;
    };

    class IManager {
        public:
            virtual IImage* get_image(const char* name) const = 0;
            virtual std::vector<IImage*> get_images() const = 0;
            virtual bool is_debugger_attached() const = 0;
    };

    class Manager : public IManager {
        private:
            std::vector<IImage*> m_images;
        public:
            Manager();

            virtual IImage* get_image(const char* name) const;
            virtual std::vector<IImage*> get_images() const;
            virtual bool is_debugger_attached() const;
    };
} // namespace IL2CPP

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

#endif // GOLFHOOK_IL2CPP_HPP
