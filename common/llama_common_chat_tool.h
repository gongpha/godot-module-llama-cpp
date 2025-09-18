#pragma once

#include "core/io/resource.h"
#include "llama_common_defined.h"

// bring in llama.cpp common chat types
#include "common/chat.h"

struct common_chat_tool;
struct common_chat_tool_call;

class LlamaCommonChatTool : public Resource {
	GDCLASS(LlamaCommonChatTool, Resource);

	common_chat_tool tool;

protected:
	static void _bind_methods();

public:
	void set_tool(const common_chat_tool &p_tool);
	const common_chat_tool &get_tool() const;

	void set_name(const String &p_name);
	String get_name() const;

	void set_description(const String &p_description);
	String get_description() const;

	void set_parameters(const String &p_parameters);
	String get_parameters() const;

	bool operator==(const LlamaCommonChatTool &other) const;

	Ref<LlamaCommonChatTool> create_from_dictionary_oaicompat(const Dictionary &p_dict);
	LlamaCommonJSON to_dictionary_oaicompat() const;

	LlamaCommonChatTool();
	~LlamaCommonChatTool();
};

class LlamaCommonChatToolCall : public Resource {
	GDCLASS(LlamaCommonChatToolCall, Resource);

	common_chat_tool_call tool_call;

protected:
	static void _bind_methods();

public:
	void set_name(const String &p_name);
	String get_name() const;

	void set_arguments(const String &p_arguments);
	String get_arguments() const;

	void set_id(const String &p_id);
	String get_id() const;

	// internal: expose for wrappers
	const common_chat_tool_call &get_tool_call() const { return tool_call; }
	void set_tool_call(const common_chat_tool_call &tc) { tool_call = tc; }

	bool operator==(const LlamaCommonChatToolCall &other) const;

	LlamaCommonChatToolCall();
	~LlamaCommonChatToolCall();
};
