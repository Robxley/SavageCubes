# CMakeList.txt : projet CMake pour SavageCube, incluez la source et définissez
# la logique spécifique au projet ici.
#
cmake_minimum_required (VERSION 3.8)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

project ("libbiohazard3d")
set(CMAKE_CXX_STANDARD 17)  

find_package(OpenGL REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)


include_directories(
    include
    "include/"
    "include/glad"
    "include/imgui")

# Crée des variables avec les fichiers à compiler
file(GLOB SOURCEFILES
          "src/*.cpp"
          "src/win32/*.cpp"
          "src/imgui/*.cpp")

function (ListToString result delim)
    list(GET ARGV 2 temp)
    math(EXPR N "${ARGC}-1")
    foreach(IDX RANGE 3 ${N})
        list(GET ARGV ${IDX} STR)
        set(temp "${temp}${delim}${STR}")
    endforeach()
    set(${result} "${temp}" PARENT_SCOPE)
endfunction(ListToString)

message("------------------")
ListToString(str "\n" ${SOURCEFILES})
message("${str}")


add_library(libbiohazard3d ${SOURCEFILES})
target_link_libraries(libbiohazard3d PRIVATE glm)
target_link_libraries(libbiohazard3d PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(libbiohazard3d PRIVATE SDL2::SDL2_image)
target_link_libraries(libbiohazard3d PRIVATE imgui::imgui)

