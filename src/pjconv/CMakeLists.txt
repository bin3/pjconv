add_lib(pjconv "pj_converter.cpp" "protobuf json")

add_test(pj_converter_test "pjconv addressbook")

add_subdirectory(proto)

# Install
install(TARGETS pjconv DESTINATION lib)
install(FILES "pj_converter.h" DESTINATION include/pjconv)

