# CMake generated Testfile for 
# Source directory: /home/wojtek/Work/MyLevelDB/leveldb
# Build directory: /home/wojtek/Work/MyLevelDB/leveldb/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(leveldb_tests "/home/wojtek/Work/MyLevelDB/leveldb/build/leveldb_tests")
set_tests_properties(leveldb_tests PROPERTIES  _BACKTRACE_TRIPLES "/home/wojtek/Work/MyLevelDB/leveldb/CMakeLists.txt;365;add_test;/home/wojtek/Work/MyLevelDB/leveldb/CMakeLists.txt;0;")
add_test(c_test "/home/wojtek/Work/MyLevelDB/leveldb/build/c_test")
set_tests_properties(c_test PROPERTIES  _BACKTRACE_TRIPLES "/home/wojtek/Work/MyLevelDB/leveldb/CMakeLists.txt;391;add_test;/home/wojtek/Work/MyLevelDB/leveldb/CMakeLists.txt;394;leveldb_test;/home/wojtek/Work/MyLevelDB/leveldb/CMakeLists.txt;0;")
add_test(env_posix_test "/home/wojtek/Work/MyLevelDB/leveldb/build/env_posix_test")
set_tests_properties(env_posix_test PROPERTIES  _BACKTRACE_TRIPLES "/home/wojtek/Work/MyLevelDB/leveldb/CMakeLists.txt;391;add_test;/home/wojtek/Work/MyLevelDB/leveldb/CMakeLists.txt;402;leveldb_test;/home/wojtek/Work/MyLevelDB/leveldb/CMakeLists.txt;0;")
subdirs("third_party/googletest")
subdirs("third_party/benchmark")
