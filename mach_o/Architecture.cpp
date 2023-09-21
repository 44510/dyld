/*
 * Copyright (c) 2017 Apple Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_LICENSE_HEADER_END@
 */

#include <stdint.h>
#include <string.h>
#if __x86_64__
  #include <mach/mach.h>
  #include <mach/host_info.h>
  #include <mach/machine.h>
  #include <mach/mach_host.h>
#endif

#include "Architecture.h"


namespace mach_o {


#if !TARGET_OS_EXCLAVEKIT
Architecture::Architecture(const mach_header* mh)
: _cputype(mh->cputype), _cpusubtype(mh->cpusubtype)
{
    if ( (mh->magic == MH_CIGAM) || (mh->magic == MH_CIGAM_64) ) {
        _cputype    = OSSwapBigToHostInt32(mh->cputype);
        _cpusubtype = OSSwapBigToHostInt32(mh->cpusubtype);
    }
}

Architecture::Architecture(const fat_arch* fh)
: _cputype(OSSwapBigToHostInt32(fh->cputype)), _cpusubtype(OSSwapBigToHostInt32(fh->cpusubtype))
{
}

Architecture::Architecture(const fat_arch_64* fh)
: _cputype(OSSwapBigToHostInt32(fh->cputype)), _cpusubtype(OSSwapBigToHostInt32(fh->cpusubtype))
{
}

bool Architecture::isBigEndian() const
{
    // ppc[64] are only big endian arches supported
    return ((_cputype & ~CPU_ARCH_MASK) == CPU_TYPE_POWERPC);
}

void Architecture::set(mach_header& mh) const
{
    if ( isBigEndian() ) {
        mh.cputype    = OSSwapHostToBigInt32(_cputype);
        mh.cpusubtype = OSSwapHostToBigInt32(_cpusubtype);
    }
    else {
        mh.cputype    = _cputype;
        mh.cpusubtype = _cpusubtype;
    }
}

void Architecture::set(fat_arch& fa) const
{
    fa.cputype    = OSSwapHostToBigInt32(_cputype);
    fa.cpusubtype = OSSwapHostToBigInt32(_cpusubtype);
}

void Architecture::set(fat_arch_64& fa) const
{
    fa.cputype    = OSSwapHostToBigInt32(_cputype);
    fa.cpusubtype = OSSwapHostToBigInt32(_cpusubtype);
}

bool Architecture::is64() const
{
    return (_cputype & CPU_ARCH_ABI64);
}

Architecture Architecture::byName(const char* name)
{
    if ( strcmp(name, "x86_64") == 0 )
        return x86_64;
    else if ( strcmp(name, "x86_64h") == 0 )
        return x86_64h;
    else if ( strcmp(name, "i386") == 0 )
        return i386;
    else if ( strcmp(name, "arm64") == 0 )
        return arm64;
    else if ( strcmp(name, "arm64e") == 0 )
        return arm64e;
    else if ( strcmp(name, "arm64_32") == 0 )
        return arm64_32;
    else if ( strcmp(name, "armv6m") == 0 )
        return armv6m;
    else if ( (strcmp(name, "armv7k") == 0) || (strcmp(name, "thumbv7k") == 0) )
        return armv7k;
    else if ( (strcmp(name, "armv7m") == 0) || (strcmp(name, "thumbv7m") == 0) )
        return armv7m;
    else if ( (strcmp(name, "armv7em") == 0) || (strcmp(name, "thumbv7em") == 0) )
        return armv7em;
    else if ( (strcmp(name, "armv7s") == 0) || (strcmp(name, "thumbv7s") == 0) )
        return armv7s;
    else if ( (strcmp(name, "armv7") == 0) || (strcmp(name, "thumbv7") == 0) )
        return armv7;
    else if ( strcmp(name, "ppc") == 0 )
        return ppc;

    return invalid;
}


bool Architecture::operator==(const Architecture& other) const
{
    if ( _cputype != other._cputype )
        return false;
    if ( (_cpusubtype & ~CPU_SUBTYPE_MASK) != (other._cpusubtype & ~CPU_SUBTYPE_MASK) )
        return false;
    // for arm64 high byte of cpu subtype must match
    if ( (_cputype == CPU_TYPE_ARM64) && (_cpusubtype != other._cpusubtype) )
        return false;
    return true;
}


const char* Architecture::name() const
{
    if ( *this == x86_64 )
        return "x86_64";
    else if ( *this == x86_64h )
        return "x86_64h";
    else if ( *this == i386 )
        return "i386";
    else if ( *this == arm64 )
        return "arm64";
    else if ( *this == arm64e )
        return "arm64e";
    else if ( *this == arm64_32 )
        return "arm64_32";
    else if ( *this == armv6m )
        return "armv6m";
    else if ( *this == armv7k )
        return "armv7k";
    else if ( *this == armv7m )
        return "armv7m";
    else if ( *this == armv7em )
        return "armv7em";
    else if ( *this == armv7s )
        return "armv7s";
    else if ( *this == armv7 )
        return "armv7";
    else if ( *this == arm64_alt )
        return "arm64";
    else if ( *this == arm64_32_alt )
        return "arm64_32";
    else if ( *this == arm64e_v1 )
        return "arm64e.v1";
    else if ( *this == arm64e_old )
        return "arm64e.old";
    else if ( *this == arm64e_kernel )
        return "arm64e.kernel";
    else if ( *this == ppc )
        return "ppc";
    return "unknown";
}

const constinit Architecture Architecture::ppc(     CPU_TYPE_POWERPC,  CPU_SUBTYPE_POWERPC_ALL);
const constinit Architecture Architecture::i386(    CPU_TYPE_I386,     CPU_SUBTYPE_I386_ALL);
const constinit Architecture Architecture::x86_64(  CPU_TYPE_X86_64,   CPU_SUBTYPE_X86_64_ALL);
const constinit Architecture Architecture::x86_64h( CPU_TYPE_X86_64,   CPU_SUBTYPE_X86_64_H);
const constinit Architecture Architecture::armv7(   CPU_TYPE_ARM,      CPU_SUBTYPE_ARM_V7);
const constinit Architecture Architecture::armv7s(  CPU_TYPE_ARM,      CPU_SUBTYPE_ARM_V7S);
const constinit Architecture Architecture::arm64(   CPU_TYPE_ARM64,    CPU_SUBTYPE_ARM64_ALL);
const constinit Architecture Architecture::arm64e(  CPU_TYPE_ARM64,    CPU_SUBTYPE_ARM64E | 0x80000000);
const constinit Architecture Architecture::arm64_32(CPU_TYPE_ARM64_32, CPU_SUBTYPE_ARM64_32_V8);
const constinit Architecture Architecture::armv6m(  CPU_TYPE_ARM,      CPU_SUBTYPE_ARM_V6M);
const constinit Architecture Architecture::armv7k(  CPU_TYPE_ARM,      CPU_SUBTYPE_ARM_V7K);
const constinit Architecture Architecture::armv7m(  CPU_TYPE_ARM,      CPU_SUBTYPE_ARM_V7M);
const constinit Architecture Architecture::armv7em( CPU_TYPE_ARM,      CPU_SUBTYPE_ARM_V7EM);
const constinit Architecture Architecture::invalid(0,0);

// non-standard cpu subtypes
const constinit Architecture Architecture::arm64_alt(    CPU_TYPE_ARM64,      CPU_SUBTYPE_ARM64_V8);
const constinit Architecture Architecture::arm64_32_alt( CPU_TYPE_ARM64_32,   CPU_SUBTYPE_ARM64_32_ALL);
const constinit Architecture Architecture::arm64e_v1(    CPU_TYPE_ARM64,      CPU_SUBTYPE_ARM64E | 0x81000000);  // future ABI version not supported
const constinit Architecture Architecture::arm64e_old(   CPU_TYPE_ARM64,      CPU_SUBTYPE_ARM64E);               // pre-ABI versioned

const constinit Architecture Architecture::arm64e_kernel(CPU_TYPE_ARM64,      CPU_SUBTYPE_ARM64E | 0xC0000000);



#if __x86_64__
static bool isHaswell()
{
    // FIXME: figure out a commpage way to check this
    struct host_basic_info info;
    mach_msg_type_number_t count = HOST_BASIC_INFO_COUNT;
    mach_port_t hostPort = mach_host_self();
    kern_return_t result = host_info(hostPort, HOST_BASIC_INFO, (host_info_t)&info, &count);
    mach_port_deallocate(mach_task_self(), hostPort);
    return (result == KERN_SUCCESS) && (info.cpu_subtype == CPU_SUBTYPE_X86_64_H);
}
#endif

Architecture Architecture::current()
{
#if __ARM_ARCH_6m__
    return armv6m;
#elif __ARM_ARCH_7K__
    return armv7k;
#elif __ARM_ARCH_7M__
    return armv7m;
#elif __ARM_ARCH_7EM__
    return armv7em;
#elif __ARM_ARCH_7A__
    return armv7;
#elif __ARM_ARCH_7S__
    return armv7s;
#elif __arm64e__
    return arm64e;
#elif __arm64__
#if __LP64__
    return arm64;
#else
    return arm64_32;
#endif
#elif __x86_64__
    return isHaswell() ? x86_64h : x86_64;
#elif __i386__
    return i386;
#else
#error unknown arch
#endif
}

bool Architecture::usesArm64Instructions() const
{
    // true for: arm64, arm64e, arm64_32, arm64e_v1, arm64e_old, arm64_alt, arm64_32_alt
    return (_cputype == CPU_TYPE_ARM64) || (_cputype == CPU_TYPE_ARM64_32);
}

bool Architecture::usesArm64AuthPointers() const
{
    // true for: arm64e, arm64e_v1, arm64e_old
    return (_cputype == CPU_TYPE_ARM64) && ((_cpusubtype & ~CPU_SUBTYPE_MASK) == CPU_SUBTYPE_ARM64E);
}


bool Architecture::usesx86_64Instructions() const
{
    // true for: x86_64 and x86_64h
    return (_cputype == CPU_TYPE_X86_64);
}


#endif // !TARGET_OS_EXCLAVEKIT

} // namespace mach_o





