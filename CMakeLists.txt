cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME Grafika)

project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Подключаем SDK
if (NOT DEFINED SDK_DIR)
	set(SDK_DIR $ENV{SDK_DIR})
	if (NOT SDK_DIR)
		set(SDK_DIR D:/sdk)
	endif ()
endif ()

set(QT_MAJOR 5)
set(QT_MINOR 15.2)
set(QT_DIR "${SDK_DIR}/Qt/Qt${QT_MAJOR}/${QT_MAJOR}.${QT_MINOR}/msvc2019_64")
set(QWT_DIR "${SDK_DIR}/Qwt/Qt5")
set(EXPRTK_DIR "${SDK_DIR}/exprtk/include")

list(APPEND CMAKE_PREFIX_PATH ${QT_DIR})
list(APPEND CMAKE_MODULE_PATH ${QWT_DIR})

find_package(Qt${QT_MAJOR} COMPONENTS REQUIRED
	Core
	Gui
	Widgets
)

find_package(Qwt REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_compile_options(
	/MP     # multi-processor compilation
	/W4		# warnings level 4
	/WX		# warning is error
	/we5038 # reorder
	/wd4250 # inherits via dominance
	/wd4251 # member (ala std::unique_ptr<some>) needs to have dll-interface to be used by clients of class 'some'
	/wd4275 # non dll-interface class 'ISome' used as base for dll-interface class 'Some'
	/wd4503	# decorated name length exceeded, name was truncated
	/bigobj # number of sections exceeded object file format limit
)

add_executable(${PROJECT_NAME} WIN32
	"src/app/main.cpp"
	"src/app/MainWindow.cpp"
	"src/app/FunctionSettings.cpp"
	"src/app/FunctionSyntaxDialog.cpp"
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt::Core Qt::Gui Qt::Widgets Qwt)
target_include_directories(${PROJECT_NAME} PRIVATE ${EXPRTK_DIR})

set(QT_PREFIX ${QT_DIR}/bin/Qt${QT_MAJOR})
file(COPY ${QT_PREFIX}Core.dll ${QT_PREFIX}Gui.dll ${QT_PREFIX}Widgets.dll ${QT_PREFIX}OpenGL.dll ${QT_PREFIX}Svg.dll ${qwt_ROOT}/lib/qwt.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
file(COPY ${QT_DIR}/plugins/platforms/qwindows.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release/platforms/)
file(COPY ${QT_DIR}/plugins/styles/qwindowsvistastyle.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release/styles/)

file(COPY ${QT_PREFIX}Cored.dll ${QT_PREFIX}Guid.dll ${QT_PREFIX}Widgetsd.dll ${QT_PREFIX}OpenGLd.dll ${QT_PREFIX}Svgd.dll ${qwt_ROOT}/lib/qwtd.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY ${QT_DIR}/plugins/platforms/qwindowsd.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug/platforms/)
file(COPY ${QT_DIR}/plugins/styles/qwindowsvistastyled.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug/styles/)
