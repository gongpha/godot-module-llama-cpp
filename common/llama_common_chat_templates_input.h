#pragma once

#include "core/object/ref_counted.h"
#include "core/variant/typed_dictionary.h"

#include "llama_common_chat.h"

struct common_chat_templates_inputs;

class LlamaCommonChatMessages;
class LlamaCommonChatTool;

class LlamaCommonChatTemplatesInput : public RefCounted {
	GDCLASS(LlamaCommonChatTemplatesInput, RefCounted);

	common_chat_templates_inputs inputs;

protected:
	static void _bind_methods();

public:
	void set_inputs(const common_chat_templates_inputs &p_inputs);
	common_chat_templates_inputs get_inputs() const;

	void set_messages(const Ref<LlamaCommonChatMessages> &p_messages);
	Ref<LlamaCommonChatMessages> get_messages() const;

	void set_grammar(const String &p_grammar);
	String get_grammar() const;

	void set_json_schema(const String &p_json_schema);
	String get_json_schema() const;

	void set_add_generation_prompt(bool p_add);
	bool get_add_generation_prompt() const;

	void set_use_jinja(bool p_use);
	bool get_use_jinja() const;

	void set_tools(const TypedArray<LlamaCommonChatTool> &p_tools);
	TypedArray<LlamaCommonChatTool> get_tools() const;

	void set_tool_choice(LlamaCommonChat::ChatToolChoice p_tool_choice);
	LlamaCommonChat::ChatToolChoice get_tool_choice() const;

	void set_parallel_tool_calls(bool p_parallel);
	bool get_parallel_tool_calls() const;

	void set_reasoning_format(LlamaCommon::ReasoningFormat p_reasoning_format);
	LlamaCommon::ReasoningFormat get_reasoning_format() const;

	void set_enable_thinking(bool p_enable);
	bool get_enable_thinking() const;

	void set_chat_template_kwargs_dictionary(const Dictionary &p_kwargs);
	void set_chat_template_kwargs(const TypedDictionary<String, String> &p_kwargs);
	TypedDictionary<String, String> get_chat_template_kwargs() const;

	void set_enable_bos(bool p_enable);
	bool get_enable_bos() const;

	void set_enable_eos(bool p_enable);
	bool get_enable_eos() const;
	
	LlamaCommonChatTemplatesInput();
	~LlamaCommonChatTemplatesInput();
};