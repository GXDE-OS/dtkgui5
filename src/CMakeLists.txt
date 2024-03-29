add_library(${LIB_NAME} SHARED)

include(dbus/dbus.cmake)
include(filedrag/dfiledrag.cmake)
include(kernel/kernel.cmake)
include(private/private.cmake)
include(util/util.cmake)

target_sources(${LIB_NAME} PRIVATE
    ${dbus_SRC}
    ${filedrag_SRC}
    ${kernel_SRC}
    ${util_SRC}
    ${private_SRC}
)

target_include_directories(${LIB_NAME} PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/util>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/global>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/kernel>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/filedrag>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/dbus>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/private>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/util/private>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/DtkGui>
)

target_link_libraries(${LIB_NAME}
PUBLIC
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Network
    Dtk${DTK_VERSION_MAJOR}::Core
PRIVATE
    Qt${QT_VERSION_MAJOR}::GuiPrivate
    Qt${QT_VERSION_MAJOR}::CorePrivate
    Qt${QT_VERSION_MAJOR}::DBus
    PkgConfig::librsvg
)

if(NOT DTK_DISABLE_EX_IMAGE_FORMAT AND EX_IMAGE_FORMAT_LIBS_FOUND)
    target_link_libraries(${LIB_NAME} PRIVATE
        ${libraw_LIBRARIES}
        FreeImage::FreeImage
    )
    target_include_directories(${LIB_NAME} PRIVATE
        ${libraw_INCLUDE_DIRS}
    )
endif()

if(NOT DTK_DISABLE_LIBXDG)
    target_link_libraries(${LIB_NAME} PRIVATE
        Qt${QT_VERSION_MAJOR}XdgIconLoader
    )
endif()

set_target_properties(${LIB_NAME} PROPERTIES
    VERSION ${CMAKE_PROJECT_VERSION}
    SOVERSION ${CMAKE_PROJECT_VERSION_MAJOR}
    EXPORT_NAME Gui
)

target_include_directories(${LIB_NAME} INTERFACE
    $<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>
)

target_link_directories(${LIB_NAME} INTERFACE
    $<INSTALL_INTERFACE:${LIBRARY_INSTALL_DIR}>
)

install(TARGETS ${LIB_NAME} EXPORT Dtk${DTK_VERSION_MAJOR}GuiTargets DESTINATION "${LIBRARY_INSTALL_DIR}")
install(EXPORT Dtk${DTK_VERSION_MAJOR}GuiTargets NAMESPACE Dtk${DTK_VERSION_MAJOR}:: FILE Dtk${DTK_VERSION_MAJOR}GuiTargets.cmake DESTINATION "${CONFIG_INSTALL_DIR}")
install(DIRECTORY ../include/util/ DESTINATION "${INCLUDE_INSTALL_DIR}" FILES_MATCHING PATTERN "*")
install(DIRECTORY ../include/DtkGui/ DESTINATION "${INCLUDE_INSTALL_DIR}" FILES_MATCHING PATTERN "*")
install(DIRECTORY ../include/global/ DESTINATION "${INCLUDE_INSTALL_DIR}" FILES_MATCHING PATTERN "*")
install(DIRECTORY ../include/kernel/ DESTINATION "${INCLUDE_INSTALL_DIR}" FILES_MATCHING PATTERN "*")
install(DIRECTORY ../include/filedrag/ DESTINATION "${INCLUDE_INSTALL_DIR}" FILES_MATCHING PATTERN "*")

# install dconfig meta file in dtkcommon
# dconfig_meta_files(COMMONID org.deepin.dtk FILES ${PROJECT_SOURCE_DIR}/misc/org.deepin.dtk.ui.preference.json)
