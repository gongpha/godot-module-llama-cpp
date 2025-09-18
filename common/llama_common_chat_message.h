#pragma once

#include "core/io/resource.h"
#include "core/variant/typed_array.h"

#include "llama_common_defined.h"
#include "llama_common_chat_tool.h"
#include "llama_common_chat.h"

class LlamaCommonChatMessage : public Resource {
	GDCLASS(LlamaCommonChatMessage, Resource);

	common_chat_msg m_msg;

protected:
	static void _bind_methods();

public:
	const common_chat_msg &get_msg() const;
	void set_msg(const common_chat_msg &p_msg);

	void set_role(const String &p_role);
	String get_role() const;

	void set_content(const String &p_content);
	String get_content() const;

	void set_content_parts(const Array &p_parts);
	Array get_content_parts() const;

	void set_tool_calls(const TypedArray<LlamaCommonChatToolCall> &p_tool_calls);
	TypedArray<LlamaCommonChatToolCall> get_tool_calls() const;

	void set_reasoning_content(const String &p_content);
	String get_reasoning_content() const;

	void set_tool_name(const String &p_tool_name);
	String get_tool_name() const;

	void set_tool_call_id(const String &p_tool_call_id);
	String get_tool_call_id() const;

	bool is_empty() const;

	Dictionary to_dictionary_oaicompat() const;

	static Ref<LlamaCommonChatMessage> create(const String &p_role, const String &p_content);
	static Ref<LlamaCommonChatMessage> create_from_dictionary_oaicompat(const Dictionary &p_dict);

	static Ref<LlamaCommonChatMessage> parse(const String &p_input, bool p_is_partial, const Ref<LlamaCommonChatSyntax> &p_syntax);

	LlamaCommonChatMessage();
	~LlamaCommonChatMessage();
};

class LlamaCommonChatMessageDiff : public RefCounted {
	GDCLASS(LlamaCommonChatMessageDiff, RefCounted);

	common_chat_msg_diff m_msg_diff;

protected:
	static void _bind_methods();

public:
	const common_chat_msg_diff &get_msg_diff() const;
	void set_msg_diff(const common_chat_msg_diff &p_msg_diff);

	void set_reasoning_content_delta(const String &p_content);
	String get_reasoning_content_delta() const;

	void set_content_delta(const String &p_content);
	String get_content_delta() const;

	void set_tool_call_index(int p_index);
	int get_tool_call_index() const;

	void set_tool_call_delta(const Ref<LlamaCommonChatToolCall> &p_tool_call);
	Ref<LlamaCommonChatToolCall> get_tool_call_delta() const;
	
	static Ref<LlamaCommonChatMessageDiff> create_from_dictionary_oaicompat(const Dictionary &p_dict);
	LlamaCommonJSON to_dictionary_oaicompat() const;
	
	static TypedArray<LlamaCommonChatMessageDiff> compute_diffs(
		const Ref<LlamaCommonChatMessage> &old_msg,
		const Ref<LlamaCommonChatMessage> &new_msg
	);

	bool operator==(const LlamaCommonChatMessageDiff &other) const;

	LlamaCommonChatMessageDiff();
	~LlamaCommonChatMessageDiff();

};