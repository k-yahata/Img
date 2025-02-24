cmake_minimum_required(VERSION 3.10)
project(ImgLib VERSION 1.0)

# C++17を要求 (std::filesystemを使用しているため)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ビルドタイプが指定されていない場合はRelease
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# コンパイラフラグ
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
  set(CMAKE_CXX_FLAGS_DEBUG "-g")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

# 必要な外部ライブラリの検索
find_package(PNG REQUIRED)
# TIFFライブラリを使用している場合（この場合必要）
find_package(TIFF REQUIRED)

# ソースファイル一覧
set(SOURCES
    Img.cpp
    Img_io.cpp
    img_io_png.cpp
    Img_basic_operations.cpp
    Img_imgproc.cpp
    Img_stat.hpp
)

# ライブラリをビルド
add_library(imglib ${SOURCES})

# インクルードディレクトリとリンクライブラリの設定
target_include_directories(imglib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PNG_INCLUDE_DIRS}
    ${TIFF_INCLUDE_DIRS}
)

target_link_libraries(imglib 
    ${PNG_LIBRARIES}
    ${TIFF_LIBRARIES}
)

# インストール設定（オプション）
install(TARGETS imglib
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(FILES Img.hpp 
    DESTINATION include
)

# ドキュメントのビルド（オプション）
option(BUILD_DOCS "Build documentation" OFF)
if(BUILD_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        # Doxygenの設定を構成
        set(DOXYGEN_INPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
        set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
                      ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
        add_custom_target(docs
            ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen" VERBATIM
        )
    endif()
endif()

# テストの構成（オプション）
option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# サンプルアプリケーションの構築（オプション）
option(BUILD_SAMPLES "Build sample applications" OFF)
if(BUILD_SAMPLES)
    add_subdirectory(samples)
endif()