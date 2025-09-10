#pragma once

#include "llama_tool.h"

#include "core/variant/callable.h"

// LlamaToolCallable: simpler tool where you only set a Callable handler.
// It auto-generates a JSON Schema for the tool's arguments from ClassDB metadata.
class LlamaToolCallable : public LLamaTool {
	GDCLASS(LlamaToolCallable, LLamaTool);

protected:
	static void _bind_methods();

public:
	// Override to auto-generate schema when handler is set.
	void set_handler(const Callable &p_callable);
};
