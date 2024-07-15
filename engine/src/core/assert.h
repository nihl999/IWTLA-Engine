#include <core/defines.h>

#define ASSERTIONS_ENABLED
#ifdef ASSERTIONS_ENABLED
#if _MSC_VER
#include <intrin.h>
/** @brief Causes a debug breakpoint to be hit. */
#define debugBreak() __debugbreak()
#else
/** @brief Causes a debug breakpoint to be hit. */
#define debugBreak() __builtin_trap()
#endif

/**
 * @brief Reports an assertion failure. Note that this is not the assertion itself,
 * just a reporting of an assertion failure that has already occurred.
 * @param expression The expression to be reported.
 * @param message A custom message to be reported, if provided.
 * @param file The path and name of the file containing the expression.
 * @param line The line number in the file where the assertion failure occurred.
 */
void report_assertion_failure(const char *expression, const char *message, const char *file, i32 line);

/**
 * @brief Asserts the provided expression to be true, and logs a failure if not.
 * Also triggers a breakpoint if debugging.
 * @param expr The expression to be evaluated.
 */
#define ASSERT(expr)                                                     \
    {                                                                    \
        if (expr)                                                        \
        {                                                                \
        }                                                                \
        else                                                             \
        {                                                                \
            /*report_assertion_failure(#expr, "", __FILE__, __LINE__);*/ \
            debugBreak();                                                \
        }                                                                \
    }

#endif