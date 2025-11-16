# 设置CMake的最低版本要求为3.16
# 这确保CMake能够理解后续使用的所有命令和特性
cmake_minimum_required(VERSION 3.16)
# 定义项目基本信息
# 1project(idokeno项目名称: idokeno
# 2VERSION 0.1版本号: 0.1
# 3LANGUAGES CXX编程语言: C++
project(idokeno VERSION 0.1 LANGUAGES CXX)
# 启用Qt的自动UI编译功能CMAKE_AUTOUIC
# 自动将.ui文件编译成对应的头文件:
set(CMAKE_AUTOUIC ON)
# 启用Qt的自动元对象编译功能CMAKE_AUTOMOC
# 自动处理包含Q_OBJECT宏的类，生成moc文件:
set(CMAKE_AUTOMOC ON)
# 启用Qt的自动资源编译功能CMAKE_AUTORCC
# 自动将.qrc资源文件编译成C++代码:
set(CMAKE_AUTORCC ON)
# 设置C++语言标准为C++17:
set(CMAKE_CXX_STANDARD 17)
# 要求必须支持C++17标准，如果不支持则报错:
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 查找Qt库，优先查找Qt6，如果找不到则查找Qt5
# 必需的组件：Widgets(窗口部件)、Multimedia(多媒体)、MultimediaWidgets(多媒体窗口部件)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)
# 根据找到的Qt主版本号，查找对应版本的Qt库# 这里再次指定需要的组件
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)

# 定义项目的源文件列表
# 包含：主程序文件、Mp4类的实现文件、头文件和UI设计文件
set(PROJECT_SOURCES
        main.cpp
        Mp4.cpp
        Mp4.h
        Mp4.ui
)
# 判断Qt主版本号是否大于等于6（即使用的是Qt6还是Qt5）
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    # 如果是Qt6，使用新的qt_add_executable命令创建可执行文件
       # MANUAL_FINALIZATION表示需要手动完成最终配置
    qt_add_executable(idokeno
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

   # 注释：为Android平台定义目标属性的示例（Qt6）
   # 如果需要为Android打包，可以取消注释并设置Android包源目录
   #    set_property(TARGET idokeno APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
   #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
   # 更多信息参见Qt6文档
else()
     # 如果是Qt5，检查是否为Android平台
    if(ANDROID)
            # 在Android平台上，创建共享库而不是可执行文件
        add_library(idokeno SHARED
            ${PROJECT_SOURCES}
        )
    # 注释：为Android平台定义属性的示例（Qt5）
    # 在find_package调用后设置Android包源目录
    #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
                # 在非Android平台上，创建普通的可执行文件
        add_executable(idokeno
            ${PROJECT_SOURCES}
        )
    endif()
endif()
# 为目标链接必要的Qt库
# PRIVATE表示这些库只用于构建目标本身，不会传递给依赖项
target_link_libraries(idokeno PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets# Qt窗口部件库
    Qt${QT_VERSION_MAJOR}::Multimedia# Qt多媒体库
    Qt${QT_VERSION_MAJOR}::MultimediaWidgets# Qt多媒体窗口部件库
)

# 注释：从Qt 6.1开始，iOS平台会自动设置MACOSX_BUNDLE_GUI_IDENTIFIER
# 如果您为iOS或macOS开发，还是应该考虑手动设置一个明确的、固定的包标识符

# 如果Qt版本低于6.1.0，需要手动设置macOS包标识符
if(${QT_VERSION} VERSION_LESS 6.1.0)
      # 设置macOS应用的包标识符（类似于Java的包名）
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.idokeno)
endif()

# 设置目标的各种属性
set_target_properties(idokeno PROPERTIES
    ${BUNDLE_ID_OPTION}                          # macOS包标识符（如果设置了的话）
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}           # macOS应用的版本号
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}  # macOS应用的短版本号
    MACOSX_BUNDLE TRUE                           # 在macOS上创建应用包（.app）
    WIN32_EXECUTABLE TRUE                        # 在Windows上创建图形界面程序（不显示控制台窗口）
)

# 包含GNU标准安装目录的定义
# 这会定义如CMAKE_INSTALL_BINDIR、CMAKE_INSTALL_LIBDIR等变量
include(GNUInstallDirs)

# 定义安装规则
install(TARGETS idokeno
    BUNDLE DESTINATION .                         # macOS应用包安装到根目录
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}  # 库文件安装到标准库目录
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}  # 可执行文件安装到标准二进制目录
)

# 如果是Qt6，执行最终的可执行文件配置
# 这会处理一些平台特定的细节配置
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(idokeno)
endif()
