# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# compile ASM with /mnt/data1/software/minasdk/sysroots/x86_64-minasdk-linux/usr/bin/arm-minamllib32-linux-gnueabi/arm-minamllib32-linux-gnueabi-gcc
# compile C with /mnt/data1/software/minasdk/sysroots/x86_64-minasdk-linux/usr/bin/arm-minamllib32-linux-gnueabi/arm-minamllib32-linux-gnueabi-gcc
ASM_FLAGS =  -O2 -pipe -g -feliminate-unused-debug-types  -O3 -DNDEBUG  

ASM_DEFINES = -DBH_FREE=wasm_runtime_free -DBH_MALLOC=wasm_runtime_malloc -DBH_PLATFORM_LINUX -DBUILD_TARGET=\"ARM\" -DBUILD_TARGET_ARM -DWASM_DISABLE_HW_BOUND_CHECK=0 -DWASM_ENABLE_AOT=1 -DWASM_ENABLE_BULK_MEMORY=0 -DWASM_ENABLE_FAST_INTERP=1 -DWASM_ENABLE_INTERP=1 -DWASM_ENABLE_LIBC_BUILTIN=1 -DWASM_ENABLE_LIBC_WASI=1 -DWASM_ENABLE_MINI_LOADER=0 -DWASM_ENABLE_MULTI_MODULE=0 -DWASM_ENABLE_SHARED_MEMORY=0 -DWASM_ENABLE_SIMD=1

ASM_INCLUDES = -I/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/interpreter -I/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/aot -I/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/libraries/libc-builtin -I/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/include -I/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src -I/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/../../../core/iwasm/include -I/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/platform/linux -I/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/platform/linux/../include -I/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/mem-alloc -I/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/common -I/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils -I/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils/uncommon 

C_FLAGS =  -O2 -pipe -g -feliminate-unused-debug-types  -std=gnu99 -ffunction-sections -fdata-sections                                          -Wall -Wno-unused-parameter -Wno-pedantic -marm -Wall -Wextra -Wformat -Wformat-security -Wshadow -O3 -DNDEBUG   -std=gnu99

C_DEFINES = -DBH_FREE=wasm_runtime_free -DBH_MALLOC=wasm_runtime_malloc -DBH_PLATFORM_LINUX -DBUILD_TARGET=\"ARM\" -DBUILD_TARGET_ARM -DWASM_DISABLE_HW_BOUND_CHECK=0 -DWASM_ENABLE_AOT=1 -DWASM_ENABLE_BULK_MEMORY=0 -DWASM_ENABLE_FAST_INTERP=1 -DWASM_ENABLE_INTERP=1 -DWASM_ENABLE_LIBC_BUILTIN=1 -DWASM_ENABLE_LIBC_WASI=1 -DWASM_ENABLE_MINI_LOADER=0 -DWASM_ENABLE_MULTI_MODULE=0 -DWASM_ENABLE_SHARED_MEMORY=0 -DWASM_ENABLE_SIMD=1

C_INCLUDES = -I/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/interpreter -I/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/aot -I/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/libraries/libc-builtin -I/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/include -I/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src -I/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/../../../core/iwasm/include -I/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/platform/linux -I/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/platform/linux/../include -I/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/mem-alloc -I/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/common -I/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils -I/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils/uncommon 
