# The set of languages for which implicit dependencies are needed:
set(CMAKE_DEPENDS_LANGUAGES
  "C"
  )
# The set of files for implicit dependencies of each language:
set(CMAKE_DEPENDS_CHECK_C
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils/uncommon/bh_getopt.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/samples/basic/cmake_build/CMakeFiles/basic.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils/uncommon/bh_getopt.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils/uncommon/bh_read_file.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/samples/basic/cmake_build/CMakeFiles/basic.dir/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils/uncommon/bh_read_file.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/samples/basic/src/main.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/samples/basic/cmake_build/CMakeFiles/basic.dir/src/main.c.o"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/samples/basic/src/native_impl.c" "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/samples/basic/cmake_build/CMakeFiles/basic.dir/src/native_impl.c.o"
  )
set(CMAKE_C_COMPILER_ID "GNU")

# Preprocessor definitions for this target.
set(CMAKE_TARGET_DEFINITIONS_C
  "BH_FREE=wasm_runtime_free"
  "BH_MALLOC=wasm_runtime_malloc"
  "BH_PLATFORM_LINUX"
  "BUILD_TARGET_X86_64"
  "WASM_DISABLE_HW_BOUND_CHECK=0"
  "WASM_ENABLE_AOT=1"
  "WASM_ENABLE_BULK_MEMORY=0"
  "WASM_ENABLE_FAST_INTERP=0"
  "WASM_ENABLE_INTERP=1"
  "WASM_ENABLE_LIBC_BUILTIN=1"
  "WASM_ENABLE_LIBC_WASI=1"
  "WASM_ENABLE_MINI_LOADER=0"
  "WASM_ENABLE_MULTI_MODULE=0"
  "WASM_ENABLE_SHARED_MEMORY=0"
  )

# The include file search paths:
set(CMAKE_C_TARGET_INCLUDE_PATH
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/interpreter"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/aot"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/libraries/libc-builtin"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/include"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src"
  "../../../core/iwasm/include"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/platform/linux"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/platform/linux/../include"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/mem-alloc"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/iwasm/common"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils"
  "../src"
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/core/shared/utils/uncommon"
  )

# Targets to which this target links.
set(CMAKE_TARGET_LINKED_INFO_FILES
  "/mnt/data1/source/wasm-micro-runtime-WAMR-05-18-2022/samples/basic/cmake_build/CMakeFiles/vmlib.dir/DependInfo.cmake"
  )

# Fortran module output directory.
set(CMAKE_Fortran_TARGET_MODULE_DIR "")
