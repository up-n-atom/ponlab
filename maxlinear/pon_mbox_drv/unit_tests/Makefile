GIT?=git
RUNNER?=

all:  deps/googletest run_tests

CXXFLAGS = \
	-Ideps/googletest/googletest/include/ \
	-Ideps/googletest/googletest/ \
	-Ideps/googletest/googlemock/include/ \
	-Ideps/googletest/googlemock/ \
	-I../src \
	-I../include \
	-I. \
	-Wall -Wextra  \
	-DLINUX \
	-pthread -O2 -g -std=c++14 \
	-Wno-missing-field-initializers -fpermissive \
	-fprofile-arcs -ftest-coverage

LDFLAGS = \
	-lpthread -pthread \
	-lgcov --coverage

deps:
	mkdir -p deps

deps/googletest: deps
	cd deps; $(GIT) clone https://github.com/google/googletest --single-branch --branch release-1.8.0 --depth 1

gtest-all.cc: deps/googletest/googletest/src/gtest-all.cc
	ln -s $^ $@

gtest_main.cc: deps/googletest/googletest/src/gtest_main.cc
	ln -s $^ $@

gmock-all.cc: deps/googletest/googlemock/src/gmock-all.cc
	ln -s $^ $@

run_tests: gtest-all.o gtest_main.o gmock-all.o mocks.o test_pon_mbox_drv.o test_pon_mbox_drv_counters_autoupdate.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	rm -rf gmock-all.cc
	rm -rf gtest-all.cc
	rm -rf gtest_main.cc
	rm -rf *.o *.gcda *.gcno
	rm -rf run_tests
	rm -rf deps
	rm -rf html

test: deps/googletest run_tests
	$(RUNNER) ./run_tests $(ARGS)

report: all
	rm -rf html
	mkdir html
	lcov -f --capture --initial --directory . --output-file html/cov_empty.info
	# exclude files which are compiled in unit_tests using symlinks
	lcov --remove html/cov_empty.info '*src/pon_mbox_drv.c' --output-file html/cov_empty.info
	$(MAKE) test
	lcov -f --capture --directory . --output-file html/cov.info
	lcov --add html/cov_empty.info --add html/cov.info --output-file html/cov.info
	lcov --remove html/cov.info '/usr/include/*' '*googletest/*' '*unit_tests/gtest-all.cc' \
			'*unit_tests/gtest_main.cc' '*unit_tests/gmock-all.cc' '*unit_tests/utest.cpp' --output-file html/cov.info
	genhtml html/cov.info --output-directory html