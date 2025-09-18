#pragma once



#include "../thirdparty/llama/common/chat.h"

#ifdef LLAMA_COMMON_USE_UNTYPED_DICTIONARY
#include "core/variant/dictionary.h"
typedef Dictionary LlamaCommonJSON;
#else
#include "core/variant/typed_dictionary.h"
typedef TypedDictionary<String, Variant> LlamaCommonJSON;
#endif