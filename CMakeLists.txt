project(matrix)
add_executable(matrix main.cpp frac.cpp frac_math.cpp matrix.cpp)
set (CMAKE_CXX_STANDARD 17)
add_compile_options(-Werror)