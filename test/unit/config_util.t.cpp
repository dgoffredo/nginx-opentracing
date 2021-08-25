#include <config_util.h>

#include <cstdlib>
#include <iostream>
#include <sstream>

#define ASSERT_EQUAL(LEFT, RIGHT, ...) \
    assert_equal(LEFT, #LEFT, RIGHT, #RIGHT, __FILE__, __LINE__)

namespace {

template <typename Left, typename Right>
void assert_equal(Left&& left, const char* leftName, Right&& right, const char* rightName, const char* file, int line) {
    if (left == right) {
        return;
    }

    std::ostringstream message;
    message << "ASSERT_EQUAL failed on line " << line << " of " << file << ":\n"
        << "    " << leftName << " != " << rightName << '\n'
        << "with\n"
        << "    " << leftName << " = " << left << '\n'
        << "and\n"
        << "    " << rightName << " = " << right << std::endl;

    std::abort();
}

void test_empty_json() {
    std::stringstream stream;
    stream << "{}";
    stream.ignore(); // skip the leading "{"

    std::string actual;
    std::string error;
    using namespace ngx_opentracing;
    scan_config_block(stream, actual, error, CommentPolicy::OMIT);
    ASSERT_EQUAL(actual, "{}");
}

int run_test(int test_case) {
    bool run_all = false;
    const int num_cases = 1;

#define CASE(NUMBER, TEST_FUNC) \
    case NUMBER: \
        std::cout << NUMBER << ". " << #TEST_FUNC << std::endl; \
        TEST_FUNC(); \
        if (!run_all) { break; }

    switch (test_case) {
    case 0:
        run_all = true;
    CASE(1, test_empty_json)
    // CASE(2, TODO)
    // CASE(3, TODO)
    default:
        return 1;
    }

#undef CASE

    return 0;
}

} // namespace

int main(int argc, char *argv[]) {
    int test_case = 0;
    if (argc > 1) {
        test_case = std::atoi(argv[argc - 1]);
    }

    const int rc = run_test(test_case);
    if (rc) {
        std::cerr << "Unknown test case: " << test_case << "\n";
    }
    return rc;
}