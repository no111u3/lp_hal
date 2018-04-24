file(GLOB_RECURSE SOURCES "src/*.cc")

add_executable(lp_hal_tests ${SOURCES})

target_link_libraries(lp_hal_tests
    PRIVATE lp::hal
    PRIVATE lp::cc_lib
)

target_compile_features(lp_hal_tests PUBLIC cxx_std_14)
