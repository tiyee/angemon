# Download and build gtest

set(GTEST_VERSION 1.8.0)
set(GTEST_NAME googletest-release-${GTEST_VERSION})
set(GTEST_TAR_PATH ${DEP_ROOT_DIR}/${GTEST_NAME}.tar.gz)
set(GTEST_TAR_EXPECT_MD5 "16877098823401d1bf2ed7891d7dce36")
if(NOT EXISTS ${GTEST_TAR_PATH})
    message(STATUS "Downloading Google Test...")
    file(DOWNLOAD https://github.com/google/googletest/archive/release-${GTEST_VERSION}.tar.gz ${GTEST_TAR_PATH} EXPECTED_MD5 ${GTEST_TAR_EXPECT_MD5})
endif()
file(MD5 ${GTEST_TAR_PATH} GTEST_TAR_MD5)
message(STATUS "${GTEST_NAME} md5 is: " ${GTEST_TAR_MD5})
if (NOT ${GTEST_TAR_EXPECT_MD5} STREQUAL ${GTEST_TAR_MD5})
    execute_process(COMMAND ${CMAKE_COMMAND} -E rm ${GTEST_TAR_PATH} WORKING_DIRECTORY ${DEP_ROOT_DIR})
    message(FATAL_ERROR "File ${GTEST_NAME} md5 is not Identify, Download again")
endif ()

if(NOT EXISTS ${DEP_ROOT_DIR}/${GTEST_NAME})
    message(STATUS "Extracting Google Test...")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${GTEST_TAR_PATH} WORKING_DIRECTORY ${DEP_ROOT_DIR})
endif()

if(NOT EXISTS ${DEP_ROOT_DIR}/googletest-release-${GTEST_VERSION}/googletest/build)
    message("Configuring Google Test...")
    file(MAKE_DIRECTORY ${DEP_ROOT_DIR}/googletest-release-${GTEST_VERSION}/googletest/build)
    execute_process(COMMAND ${CMAKE_COMMAND}
            "-H${DEP_ROOT_DIR}/googletest-release-${GTEST_VERSION}/googletest"
            "-B${DEP_ROOT_DIR}/googletest-release-${GTEST_VERSION}/googletest/build"
            RESULT_VARIABLE
            GOOGLETEST_CONFIGURE)
    if(NOT GOOGLETEST_CONFIGURE EQUAL 0)
        message(FATAL_ERROR "Google Test Configure failed!")
    endif()

    if(BUILD_DEPS STREQUAL "yes")
        message("Building Google Test locally...")
        execute_process(COMMAND ${CMAKE_COMMAND} --build
                "${DEP_ROOT_DIR}/googletest-release-${GTEST_VERSION}/googletest/build"
                RESULT_VARIABLE
                GOOGLETEST_BUILD)
        if(NOT GOOGLETEST_BUILD EQUAL 0)
            message(FATAL_ERROR "Google Test build failed!")
        endif()
    endif()
endif()