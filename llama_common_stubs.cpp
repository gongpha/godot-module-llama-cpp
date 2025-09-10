// Lightweight stubs to satisfy unresolved symbols from llama.cpp common utilities
// when integrating as a Godot module without upstream-generated build info or log.
// These are linked into the engine core so external modules can rely on them.

#include <cstdarg>

// Forward declarations matching llama.cpp/common signatures.
struct common_log;

// Keep enum name to match mangled symbol expected by llama.cpp objects.
// Underlying type defaults to int on MSVC.
enum ggml_log_level {
    GGML_LOG_LEVEL_ERROR = 2,
    GGML_LOG_LEVEL_WARN = 3,
    GGML_LOG_LEVEL_INFO = 4,
    GGML_LOG_LEVEL_DEBUG = 5,
};

// Build info symbols expected by common.cpp in llama.cpp.
// Provide benign defaults when not provided by upstream build.
int LLAMA_BUILD_NUMBER = LLAMA_CPP_BUILD_NUMBER;
const char * LLAMA_COMMIT = LLAMA_CPP_COMMIT;
const char * LLAMA_COMPILER = LLAMA_CPP_COMPILER;
const char * LLAMA_BUILD_TARGET = LLAMA_CPP_BUILD_TARGET;

// Verbosity threshold variable used by common/log.cpp. Define with a default.
int common_log_verbosity_thold = GGML_LOG_LEVEL_INFO;

// Minimal no-op implementations of the common logging entry points
// referenced from llama.cpp's common.cpp.
common_log * common_log_main() {
    static common_log dummy; // opaque; size unused, pointer-only across TU
    return &dummy;
}

void common_log_add(common_log *, ggml_log_level, const char *, ...) {
    // Swallow log messages. Upstream routes to stderr; we choose to no-op here.
    // If you want integration with Godot logging, wire this to print_verbose.
    va_list _;
}
