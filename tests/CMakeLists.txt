add_subdirectory("${PROJECT_SOURCE_DIR}/extern/googletest" "extern/googletest")

mark_as_advanced(
    BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
    gmock_build_tests gtest_build_samples gtest_build_tests
    gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)

set_target_properties(gtest PROPERTIES FOLDER extern)
set_target_properties(gtest_main PROPERTIES FOLDER extern)
set_target_properties(gmock PROPERTIES FOLDER extern)
set_target_properties(gmock_main PROPERTIES FOLDER extern)

macro(package_add_test_with_libraries TESTNAME FILES LIBRARIES TEST_WORKING_DIRECTORY)
    add_executable(${TESTNAME} ${FILES})
    set_property(TARGET ${TESTNAME} PROPERTY CXX_STANDARD 17)
    target_include_directories(${TESTNAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)
    target_link_libraries(${TESTNAME} gtest gmock gtest_main ${LIBRARIES})
    gtest_discover_tests(${TESTNAME}
        WORKING_DIRECTORY ${TEST_WORKING_DIRECTORY}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${TEST_WORKING_DIRECTORY}"
    )
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

package_add_test_with_libraries(SingleThreadTests SingleThreadTests.cpp Transaction "${PROJECT_SOURCE_DIR}/tests")
package_add_test_with_libraries(MultiThreadTests MultiThreadTests.cpp Transaction "${PROJECT_SOURCE_DIR}/tests")
package_add_test_with_libraries(TransactionManagerTests TransactionManagerTests.cpp Transaction "${PROJECT_SOURCE_DIR}/tests")
