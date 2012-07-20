if (${STATIC_BUILD})
  SET(BOOST_STATIC 1)
else()
  find_package(Boost
    COMPONENTS filesystem thread system)

  if (${Boost_VERSION} LESS 104800)
    # boost::locale is only available from 1.48.00
    message("Too old version of Boost (${Boost_LIB_VERSION}): Building the static version")
    SET(BOOST_STATIC 1)
  else()
    SET(BOOST_STATIC 0)

    add_definitions(
      -DBOOST_FILESYSTEM_VERSION=3
      )

    include_directories(${Boost_INCLUDE_DIRS})
    link_libraries(${Boost_LIBRARIES})
  endif()
endif()


if (BOOST_STATIC)
  SET(BOOST_NAME boost_1_49_0)
  SET(BOOST_SOURCES_DIR ${CMAKE_BINARY_DIR}/${BOOST_NAME})
  DownloadPackage("http://switch.dl.sourceforge.net/project/boost/boost/1.49.0/${BOOST_NAME}.tar.gz" "${BOOST_SOURCES_DIR}" "${BOOST_PRELOADED}" "${BOOST_NAME}/boost ${BOOST_NAME}/libs/thread/src ${BOOST_NAME}/libs/system/src ${BOOST_NAME}/libs/filesystem/v3/src ${BOOST_NAME}/libs/locale/src")

  if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    list(APPEND THIRD_PARTY_SOURCES 
      ${BOOST_SOURCES_DIR}/libs/thread/src/pthread/once.cpp
      ${BOOST_SOURCES_DIR}/libs/thread/src/pthread/thread.cpp
      )
    add_definitions(
      -DBOOST_LOCALE_WITH_ICONV=1
      )
  elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    list(APPEND THIRD_PARTY_SOURCES 
      ${BOOST_SOURCES_DIR}/libs/thread/src/win32/tss_dll.cpp
      ${BOOST_SOURCES_DIR}/libs/thread/src/win32/thread.cpp
      ${BOOST_SOURCES_DIR}/libs/thread/src/win32/tss_pe.cpp
      ${BOOST_SOURCES_DIR}/libs/filesystem/v3/src/windows_file_codecvt.cpp
      )
    add_definitions(
      -DBOOST_LOCALE_WITH_WCONV=1
      )
  else()
    message(FATAL_ERROR "Support your platform here")
  endif()

  list(APPEND THIRD_PARTY_SOURCES 
    ${BOOST_SOURCES_DIR}/libs/system/src/error_code.cpp
    ${BOOST_SOURCES_DIR}/libs/filesystem/v3/src/path.cpp
    ${BOOST_SOURCES_DIR}/libs/filesystem/v3/src/path_traits.cpp
    ${BOOST_SOURCES_DIR}/libs/filesystem/v3/src/operations.cpp
    ${BOOST_SOURCES_DIR}/libs/filesystem/v3/src/codecvt_error_category.cpp
    ${BOOST_SOURCES_DIR}/libs/locale/src/encoding/codepage.cpp
    )

  add_definitions(
    # Static build of Boost
    -DBOOST_ALL_NO_LIB 
    -DBOOST_ALL_NOLIB 
    -DBOOST_DATE_TIME_NO_LIB 
    -DBOOST_THREAD_BUILD_LIB
    -DBOOST_PROGRAM_OPTIONS_NO_LIB
    -DBOOST_REGEX_NO_LIB
    -DBOOST_SYSTEM_NO_LIB
    -DBOOST_LOCALE_NO_LIB
    )

  include_directories(
    ${BOOST_SOURCES_DIR}
    )

  source_group(ThirdParty\\Boost REGULAR_EXPRESSION ${BOOST_SOURCES_DIR}/.*)
endif()