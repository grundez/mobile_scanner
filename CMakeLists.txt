cmake_minimum_required(VERSION 3.16)

project(mush_app VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

include_directories(
    ${CMAKE_SOURCE_DIR}/tflite_headers           # tensorflow lite headers
    ${CMAKE_SOURCE_DIR}/tflite_headers/c         #
    ${CMAKE_SOURCE_DIR}/third-party/tensorflow   #
    ${CMAKE_SOURCE_DIR}/third-party/flatbuffers/include  # flatbuffers headers
)


find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appmush_app
    model.cpp
    main.cpp
)

qt_add_resources(appmush_app "resources"
    PREFIX "/"
    FILES
        best_usable_float32.tflite
)

qt_add_qml_module(appmush_app
    URI mush_app
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES
        SOURCES model.h model.cpp
)

if(ANDROID)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=options")
    set(CMAKE_ANDROID_ARCH_ABI armeabi-v7a)

    add_library(tensorflowlite SHARED IMPORTED)
    set_target_properties(tensorflowlite PROPERTIES
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/android/libs/arm32/libtensorflowlite.so
    )

    file(GLOB TFLITE_LIBRARIES "${CMAKE_SOURCE_DIR}/android/libs/arm32/*.so")
    file(COPY ${TFLITE_LIBRARIES} DESTINATION ${CMAKE_BINARY_DIR}/android-build/libs/armeabi-v7a/)

    target_link_libraries(appmush_app PRIVATE
        tensorflowlite
        Qt6::Quick
    )
    else()
        target_link_libraries(appmush_app PRIVATE Qt6::Quick)
endif()

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appmush_app PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appmush_app
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)


include(GNUInstallDirs)
install(TARGETS appmush_app
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
