#include "stpch.h"
#include "Sentinel/Common/CPU/CPUInfo.h"

#if defined(__GNUC__) && defined(i386)
    #define cpuid(func, a, b, c, d)              \
        __asm__ __volatile__(                    \
            "        pushl %%ebx        \n"      \
            "        xorl %%ecx,%%ecx   \n"      \
            "        cpuid              \n"      \
            "        movl %%ebx, %%esi  \n"      \
            "        popl %%ebx         \n"      \
            : "=a"(a), "=S"(b), "=c"(c), "=d"(d) \
            : "a"(func))
#elif defined(__GNUC__) && defined(__x86_64__)
    #define cpuid(func, a, b, c, d)              \
        __asm__ __volatile__(                    \
            "        pushq %%rbx        \n"      \
            "        xorq %%rcx,%%rcx   \n"      \
            "        cpuid              \n"      \
            "        movq %%rbx, %%rsi  \n"      \
            "        popq %%rbx         \n"      \
            : "=a"(a), "=S"(b), "=c"(c), "=d"(d) \
            : "a"(func))
#elif (defined(_MSC_VER) && defined(_M_IX86)) || defined(__WATCOMC__)
    #define cpuid(func, a, b, c, d) \
        __asm { \
        __asm mov eax, func \
        __asm xor ecx, ecx \
        __asm cpuid \
        __asm mov a, eax \
        __asm mov b, ebx \
        __asm mov c, ecx \
        __asm mov d, edx}
#elif defined(_MSC_VER) && defined(_M_X64)
    #include <intrin.h>
    #define cpuid(func, a, b, c, d) \
        {                           \
            int CPUInfo[4];         \
            __cpuid(CPUInfo, func); \
            a = CPUInfo[0];         \
            b = CPUInfo[1];         \
            c = CPUInfo[2];         \
            d = CPUInfo[3];         \
        }
#else
    #define cpuid(func, a, b, c, d) \
        do {                        \
            a = b = c = d = 0;      \
            (void)a;                \
            (void)b;                \
            (void)c;                \
            (void)d;                \
        } while (0)
#endif

namespace Sentinel {
    CChar* CPUInfo::GetCPUType() {
        static Char type[13];

        if (!type[0]) {
            UInt32 i = 0;

            CalculateFeatures();
            if (CPUIDMaxFunction > 0) {
                UInt32 a, b, c, d;
                cpuid(0x00000000, a, b, c, d);
                (void)a;
                type[i++] = (Char)(b & 0xff);
                b >>= 8;
                type[i++] = (Char)(b & 0xff);
                b >>= 8;
                type[i++] = (Char)(b & 0xff);
                b >>= 8;
                type[i++] = (Char)(b & 0xff);

                type[i++] = (Char)(d & 0xff);
                d >>= 8;
                type[i++] = (Char)(d & 0xff);
                d >>= 8;
                type[i++] = (Char)(d & 0xff);
                d >>= 8;
                type[i++] = (Char)(d & 0xff);

                type[i++] = (Char)(c & 0xff);
                c >>= 8;
                type[i++] = (Char)(c & 0xff);
                c >>= 8;
                type[i++] = (Char)(c & 0xff);
                c >>= 8;
                type[i++] = (Char)(c & 0xff);
            }

            if (!type[0]) { strcpy_s(type, sizeof(type), "Unknown"); }
        }

        return type;
    }

    UInt32 CPUInfo::GetL1CacheLineSize() {
        CChar* type = GetCPUType();
        int a, b, c, d;
        (void)a;
        (void)b;
        (void)c;
        (void)d;
        if (strcmp(type, "GenuineIntel") == 0) {
            cpuid(0x00000001, a, b, c, d);
            return (((b >> 8) & 0xff) * 8);
        } else if (strcmp(type, "AuthenticAMD") == 0) {
            cpuid(0x80000005, a, b, c, d);
            return (c & 0xff);
        } else {
            return 1024;
        }
    }

    Int32 CPUInfo::HasCPUID() {
        int value = 0;
#if defined(__GNUC__) && defined(i386)
        __asm__(
            "pushfl                       \n"
            "popl      %%eax              \n"
            "movl      %%eax,%%ecx        \n"
            "xorl      $0x200000,%%eax    \n"
            "pushl     %%eax              \n"
            "popfl                        \n"
            "pushfl                       \n"
            "popl      %%eax              \n"
            "xorl      %%ecx,%%eax        \n"
            "jz        1f                 \n"
            "movl      $1,%0              \n"
            "1:
            : "=m"(value)
            :
            : "%eax", "%ecx");
#elif (defined(_MSC_VER) && defined(_M_IX86)) || defined(__WATCOMC__)
        __asm {
			pushfd
			pop      eax
			mov      ecx, eax
			xor eax, 200000h
			push     eax
			popfd
			pushfd
			pop      eax
			xor eax, ecx
			jz done
			mov value, 1
        }
#elif defined(_MSC_VER) && defined(_M_X64)
        value = 1;
#elif defined(__sun) && defined(__i386)
        __asm(
            "pushfl						  \n"
            "popl    %eax             \n"
            "movl    %eax,%ecx        \n"
            "xorl    $0x200000, % eax \n"
            "pushl   %eax             \n"
            "popfl                    \n"
            "pushfl                   \n"
            "popl    %eax             \n"
            "xorl    %ecx,%eax        \n"
            "jz      1f               \n"
            "movl    $1,-8(%ebp)      \n"
            "1:                       \n");
#elif defined(__sun) && defined(__amd64)
        __asm(
            "pushfq                     \n"
            "popq    %rax           \n"
            "movq    %rax,%rcx      \n"
            "xorl    $0x200000,%eax \n"
            "pushq   %rax           \n"
            "popfq                  \n"
            "pushfq                 \n"
            "popq    %rax           \n"
            "xorl    %ecx,%eax      \n"
            "jz      1f             \n"
            "movl    $1,-8(%rbp)    \n"
            "1:                     \n");
#endif
        return value;
    }

    void CPUInfo::CalculateFeatures() {
        static Bool checked = false;
        if (!checked) {
            checked = true;
            if (HasCPUID()) {
                Int32 a, b, c, d;
                cpuid(0, a, b, c, d);
                CPUIDMaxFunction = a;
                if (CPUIDMaxFunction >= 1) {
                    cpuid(1, a, b, c, d);
                    CPUIDFeatures[0] = a;
                    CPUIDFeatures[1] = b;
                    CPUIDFeatures[2] = c;
                    CPUIDFeatures[3] = d;

                    if (c & 0x08000000) {
#if defined(__GNUC__) && (defined(i386) || defined(__x86_64__))
                        __asm__(".byte 0x0f, 0x01, 0xd0" : "=a"(a) : "c"(0) : "%edx");
#elif defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64)) && (_MSC_FULL_VER >= 160040219)
                        a = (int)_xgetbv(0);
#elif (defined(_MSC_VER) && defined(_M_IX86)) || defined(__WATCOMC__)
                        __asm
                            {
							xor ecx, ecx
							_asm _emit 0x0f _asm _emit 0x01 _asm _emit 0xd0
							mov a, eax
                            }
#endif
                        OSSavesYMM = ((a & 6) == 6) ? true : false;
                    }
                }
            }
        }
    }
}  // namespace Sentinel
