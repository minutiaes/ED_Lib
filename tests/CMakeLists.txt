add_executable(test_marmelade
        test_marmelade.cpp
        test_marmelade.h)

target_link_libraries(test_marmelade ${OpenCV_LIBS} EDLib)