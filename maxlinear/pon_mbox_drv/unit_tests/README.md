Building tests
==============
To build test, you need:
- gcc5+
- lcov for generating reports

Steps
-----
To build test, just run 'make test'. It will:
- download GoogleTest repository (git shallow clone for release-1.8)
- make if needed
- run the test

If you want to use your own copy of gtest, you can prepare gtest sources
in directory deps/googletest/ (eg. link to directories provided by
distribution package.

Coverage test
-------------
All tests are run by default with checking for coverage. To see actual result,
instead calling 'make test', run 'make report'. It will generate report file
in html/ directory (start with index.html).
