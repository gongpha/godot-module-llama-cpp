#include "thirdparty/llama/common/common.h"

int LLAMA_BUILD_NUMBER = LLAMA_CPP_BUILD_NUMBER;
const char * LLAMA_COMMIT = LLAMA_CPP_COMMIT;
const char * LLAMA_COMPILER = LLAMA_CPP_COMPILER;
const char * LLAMA_BUILD_TARGET = LLAMA_CPP_BUILD_TARGET;

extern "C" {
struct common_log { };
struct common_log * common_log_main(void) { return nullptr; }
void common_log_add(struct common_log *, ggml_log_level, const char *, ...) {}
}
