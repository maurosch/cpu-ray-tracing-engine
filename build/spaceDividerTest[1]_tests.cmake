add_test( SpaceDivider.BasicAssertions /home/mauro/Documents/computacion-grafica/tp-final/build/spaceDividerTest [==[--gtest_filter=SpaceDivider.BasicAssertions]==] --gtest_also_run_disabled_tests)
set_tests_properties( SpaceDivider.BasicAssertions PROPERTIES WORKING_DIRECTORY /home/mauro/Documents/computacion-grafica/tp-final/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set( spaceDividerTest_TESTS SpaceDivider.BasicAssertions)
