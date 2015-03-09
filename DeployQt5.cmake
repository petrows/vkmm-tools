function(install_qt5_file srcPath fileName destPath)
    set(QT_PREFIX "")

    if(CMAKE_BUILD_TYPE STREQUAL Debug)
        set(QT_PREFIX "d")
    endif(CMAKE_BUILD_TYPE STREQUAL Debug)

    install(FILES "${srcPath}/${fileName}${QT_PREFIX}.dll" DESTINATION ${destPath} COMPONENT Runtime)
endfunction(install_qt5_file)

function(install_qt5_lib DEST)
    set (FilesToInstall)
    foreach(Qt5Lib ${ARGN})
        get_target_property(Qt5LibLocation Qt5::${Qt5Lib} LOCATION_${CMAKE_BUILD_TYPE})
        set (FilesToInstall ${FilesToInstall} ${Qt5LibLocation})
    endforeach(Qt5Lib ${ARGN})
    install(FILES ${FilesToInstall} DESTINATION ${DEST})
endfunction(install_qt5_lib)

function(install_qt5_dbdrivers dest)
    foreach(driver ${ARGN})
        install_qt5_file(${_qt5Core_install_prefix}/plugins/sqldrivers ${driver}  ${dest}/sqldrivers)
    endforeach(driver)
endfunction(install_qt5_dbdrivers)

function(install_qt5_imageformats dest)
    foreach(imgformat ${ARGN})
        install_qt5_file(${_qt5Core_install_prefix}/plugins/imageformats ${imgformat}  ${dest}/imageformats)
    endforeach(imgformat)
endfunction(install_qt5_imageformats)

function(install_qt5_platform dest)
    install_qt5_file(${_qt5Core_install_prefix}/plugins/platforms "qwindows" ${dest}/platforms)
endfunction(install_qt5_platform)

function(install_qt5_qml_plugin_qtquick2 dest)
    install_qt5_file(${_qt5Core_install_prefix}/qml/QtQuick.2 "qtquick2plugin" ${dest}/qml/QtQuick.2)
    install(FILES ${_qt5Core_install_prefix}/qml/QtQuick.2/qmldir DESTINATION ${dest}/qml/QtQuick.2)
endfunction(install_qt5_qml_plugin_qtquick2)

function(install_qt5_qml_plugin dest)
    foreach(plugin ${ARGN})
        if(${plugin} STREQUAL "QtQuick2")
            install_qt5_qml_plugin_qtquick2 (${dest})
        endif()
    endforeach(plugin)
endfunction(install_qt5_qml_plugin)

function(install_qt5_V8 dest)
    install_qt5_file(${_qt5Core_install_prefix}/bin Qt5V8 ${dest})
endfunction(install_qt5_V8)

function(install_qt5_icu dest)
    install(FILES ${_qt5Core_install_prefix}/bin/icudt51.dll
                  ${_qt5Core_install_prefix}/bin/icuin51.dll
                  ${_qt5Core_install_prefix}/bin/icuuc51.dll
            DESTINATION ${dest})
endfunction(install_qt5_icu)

function(install_qt_mingw_rt dest)
    install(FILES ${_qt5Core_install_prefix}/bin/libgcc_s_dw2-1.dll
                  ${_qt5Core_install_prefix}/bin/libstdc++-6.dll
                  ${_qt5Core_install_prefix}/bin/libwinpthread-1.dll
            DESTINATION ${dest})
endfunction(install_qt_mingw_rt)
