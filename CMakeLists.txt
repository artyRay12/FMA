project(Fullmetal-Alchemist)
cmake_minimum_required(VERSION 2.8)

# ���� ����������� ������ ��������� SFML � � ������������
set(SFML_STATIC_LIBRARIES TRUE)
find_package(Freetype REQUIRED)
find_package(JPEG REQUIRED)
find_package(SFML 2 COMPONENTS window graphics system REQUIRED)

# ������� add_sample ��������� ���������� � �������� � ���������� SFML
# ��� ����������� ���������� ������ ���� ������ (� ������������ �����)
function(add_sample NAME)
    file(GLOB SAMPLE_SRC "${NAME}/*.cpp" "${NAME}/*.h")
    add_executable(${NAME} ${SAMPLE_SRC})
    target_include_directories(${NAME} PRIVATE ${SFML_INCLUDE_DIR} ${JPEG_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS})
    target_compile_features(${NAME} PUBLIC cxx_std_17)
    target_compile_definitions(${NAME} PRIVATE SFML_STATIC)
    target_link_libraries(${NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} ${JPEG_LIBRARIES} ${FREETYPE_LIBRARIES})
endfunction()

add_sample(game)


# ��������� ����������� ������ � �������� ����������
# file(COPY game/wallPaper DESTINATION ${CMAKE_BINARY_DIR})
# file(COPY game/sprites DESTINATION ${CMAKE_BINARY_DIR})
# file(COPY game/maps DESTINATION ${CMAKE_BINARY_DIR})
