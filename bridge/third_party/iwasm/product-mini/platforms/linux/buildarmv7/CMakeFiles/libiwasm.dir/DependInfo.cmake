# The set of languages for which implicit dependencies are needed:
set(CMAKE_DEPENDS_LANGUAGES
  "ASM"
  "C"
  )
# The set of files for implicit dependencies of each language:
set(CMAKE_DEPENDS_CHECK_ASM
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/common/arch/invokeNative_arm.s" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/common/arch/invokeNative_arm.s.o"
  )
set(CMAKE_ASM_COMPILER_ID "GNU")

# Preprocessor definitions for this target.
set(CMAKE_TARGET_DEFINITIONS_ASM
  "BH_FREE=wasm_runtime_free"
  "BH_MALLOC=wasm_runtime_malloc"
  "BH_PLATFORM_LINUX"
  "BUILD_TARGET=\"ARM\""
  "BUILD_TARGET_ARM"
  "WASM_DISABLE_HW_BOUND_CHECK=0"
  "WASM_ENABLE_AOT=1"
  "WASM_ENABLE_BULK_MEMORY=0"
  "WASM_ENABLE_FAST_INTERP=1"
  "WASM_ENABLE_INTERP=1"
  "WASM_ENABLE_LIBC_BUILTIN=1"
  "WASM_ENABLE_LIBC_WASI=1"
  "WASM_ENABLE_MINI_LOADER=0"
  "WASM_ENABLE_MULTI_MODULE=0"
  "WASM_ENABLE_SHARED_MEMORY=0"
  "WASM_ENABLE_SIMD=1"
  "libiwasm_EXPORTS"
  )

# The include file search paths:
set(CMAKE_ASM_TARGET_INCLUDE_PATH
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/interpreter"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/aot"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/libraries/libc-builtin"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/include"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src"
  "../../../../core/iwasm/include"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/platform/linux"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/platform/linux/../include"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/mem-alloc"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/common"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils/uncommon"
  )
set(CMAKE_DEPENDS_CHECK_C
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/aot/aot_intrinsic.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/aot/aot_intrinsic.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/aot/aot_loader.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/aot/aot_loader.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/aot/aot_runtime.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/aot/aot_runtime.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/aot/arch/aot_reloc_arm.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/aot/arch/aot_reloc_arm.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/common/wasm_application.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/common/wasm_application.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/common/wasm_c_api.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/common/wasm_c_api.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/common/wasm_exec_env.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/common/wasm_exec_env.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/common/wasm_memory.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/common/wasm_memory.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/common/wasm_native.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/common/wasm_native.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/common/wasm_runtime_common.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/common/wasm_runtime_common.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/common/wasm_shared_memory.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/common/wasm_shared_memory.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/interpreter/wasm_interp_fast.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/interpreter/wasm_interp_fast.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/interpreter/wasm_loader.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/interpreter/wasm_loader.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/interpreter/wasm_runtime.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/interpreter/wasm_runtime.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/libraries/libc-builtin/libc_builtin_wrapper.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/libraries/libc-builtin/libc_builtin_wrapper.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/libraries/libc-wasi/libc_wasi_wrapper.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/libraries/libc-wasi/libc_wasi_wrapper.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/random.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/random.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/str.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/str.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/mem-alloc/ems/ems_alloc.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/mem-alloc/ems/ems_alloc.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/mem-alloc/ems/ems_hmu.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/mem-alloc/ems/ems_hmu.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/mem-alloc/ems/ems_kfc.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/mem-alloc/ems/ems_kfc.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/mem-alloc/mem_alloc.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/mem-alloc/mem_alloc.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/platform/common/posix/posix_malloc.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/platform/common/posix/posix_malloc.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/platform/common/posix/posix_memmap.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/platform/common/posix/posix_memmap.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/platform/common/posix/posix_socket.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/platform/common/posix/posix_socket.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/platform/common/posix/posix_thread.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/platform/common/posix/posix_thread.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/platform/common/posix/posix_time.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/platform/common/posix/posix_time.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/platform/linux/platform_init.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/platform/linux/platform_init.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils/bh_assert.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils/bh_assert.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils/bh_common.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils/bh_common.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils/bh_hashmap.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils/bh_hashmap.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils/bh_list.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils/bh_list.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils/bh_log.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils/bh_log.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils/bh_queue.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils/bh_queue.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils/bh_vector.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils/bh_vector.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils/runtime_timer.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/product-mini/platforms/linux/buildX64/CMakeFiles/libiwasm.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils/runtime_timer.c.o"
  )
set(CMAKE_C_COMPILER_ID "GNU")

# Preprocessor definitions for this target.
set(CMAKE_TARGET_DEFINITIONS_C
  "BH_FREE=wasm_runtime_free"
  "BH_MALLOC=wasm_runtime_malloc"
  "BH_PLATFORM_LINUX"
  "BUILD_TARGET=\"ARM\""
  "BUILD_TARGET_ARM"
  "WASM_DISABLE_HW_BOUND_CHECK=0"
  "WASM_ENABLE_AOT=1"
  "WASM_ENABLE_BULK_MEMORY=0"
  "WASM_ENABLE_FAST_INTERP=1"
  "WASM_ENABLE_INTERP=1"
  "WASM_ENABLE_LIBC_BUILTIN=1"
  "WASM_ENABLE_LIBC_WASI=1"
  "WASM_ENABLE_MINI_LOADER=0"
  "WASM_ENABLE_MULTI_MODULE=0"
  "WASM_ENABLE_SHARED_MEMORY=0"
  "WASM_ENABLE_SIMD=1"
  "libiwasm_EXPORTS"
  )

# The include file search paths:
set(CMAKE_C_TARGET_INCLUDE_PATH
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/interpreter"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/aot"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/libraries/libc-builtin"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/include"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src"
  "../../../../core/iwasm/include"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/platform/linux"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/platform/linux/../include"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/mem-alloc"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/common"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils/uncommon"
  )

# Targets to which this target links.
set(CMAKE_TARGET_LINKED_INFO_FILES
  )

# Fortran module output directory.
set(CMAKE_Fortran_TARGET_MODULE_DIR "")
