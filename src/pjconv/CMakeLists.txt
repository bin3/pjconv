add_lib(pjconv "pjconv.cpp" "protobuf json")

add_test(pjconv_test "pjconv addressbook")

add_subdirectory(proto)

# Install
install(TARGETS pjconv DESTINATION lib)
install(FILES "pjconv.h" DESTINATION include/pjconv)

