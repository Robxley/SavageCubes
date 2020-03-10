# CMakeList.txt : projet CMake pour SavageCube, incluez la source et définissez
# la logique spécifique au projet ici.
#
cmake_minimum_required (VERSION 3.8)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

project ("SavageCube")
set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(sdl2-image REQUIRED)
find_package(glm REQUIRED)
find_package(imgui REQUIRED)

include_directories(
    include
    "biohazard3d/include/"
    "biohazard3d/include/imgui"
)

set(vlink "mklink /d \"${PROJECT_BINARY_DIR}/data3d\" \"${PROJECT_SOURCE_DIR}/data3d\"")
message("msg - cmd /c ${vlink}")
execute_process(COMMAND cmd /c "${vlink}")


# Crée des variables avec les fichiers à compiler
file(GLOB SOURCEFILES
          "*.cpp")

add_subdirectory(biohazard3d)

# Ajoutez une source à l'exécutable de ce projet.
add_executable (SavageCube ${SOURCEFILES})
target_link_libraries(SavageCube libbiohazard3d)
target_link_libraries(SavageCube glm)
target_link_libraries(SavageCube SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(SavageCube SDL2::SDL2_image)
target_link_libraries(SavageCube imgui::imgui)

# TODO: Ajoutez des tests et installez des cibles si nécessaire.
