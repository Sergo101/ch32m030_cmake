
set(CMAKE_SYSTEM_NAME Generic)
# set(CMAKE_SYSTEM_VERSION 1)
# set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
cmake_minimum_required(VERSION 3.20)

set(link_LIBS
peripherial_ch32v203
)

include("cmake/riscv-none-elf.cmake")

project(ch32m030_pwmtest C CXX ASM)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

set(include_DIRS
    main/inc
    Core
    Debug 
)

file(GLOB_RECURSE SOURCES 
    "main/src/*.c"
    "Core/core_riscv.c" 
    "Debug/debug.c"
    "Startup/startup_ch32m030.S"
    )

set(LINKER_SCRIPT  ${CMAKE_SOURCE_DIR}/Ld/Link.ld)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

# Add sources to executable
add_subdirectory(Peripherial)


add_compile_definitions(
	-DCH32M030
	-DUSE_WCH_INTERRUPT_FAST_ATTR
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${include_DIRS})

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    ${symbols_SYMB}
    $<$<COMPILE_LANGUAGE:C>: ${symbols_c_SYMB}>
    $<$<COMPILE_LANGUAGE:CXX>: ${symbols_cxx_SYMB}>
    $<$<COMPILE_LANGUAGE:ASM>: ${symbols_asm_SYMB}>

    # Configuration specific
    $<$<CONFIG:Debug>: >
    $<$<CONFIG:Release>: >
)



# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME} ${link_LIBS})

# Compiler options
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    -march=rv32imc_zba_zbb_zbc_zbs_xw
	-mabi=ilp32
    -mcmodel=medany
    -msmall-data-limit=8
    -mno-save-restore
    -fmessage-length=0
    -fsigned-char
    -ffunction-sections
    -fdata-sections
    -fno-common
    -Wall
    $<$<COMPILE_LANGUAGE:C>: >
    $<$<COMPILE_LANGUAGE:CXX>:

    >
    $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp -MMD -MP>
    $<$<CONFIG:Debug>:-Og -g3 -ggdb>
    $<$<CONFIG:Release>:-Og -g0>
)

# Linker options
target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
    -T ${LINKER_SCRIPT}
    ${cpu_PARAMS}
    ${linker_OPTS}
    -march=rv32imc_zba_zbb_zbc_zbs_xw
	-mabi=ilp32
    -nostartfiles 
    -Xlinker --gc-sections  
    -Wl,--print-memory-usage
    -Wl,-Map,${PROJECT_NAME}.map 
    --specs=nano.specs 
    --specs=nosys.specs
)

# Execute post-build to print size, generate hex and bin
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> IMAGE.BIN
)
