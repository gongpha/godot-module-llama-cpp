#pragma once

#include "core/object/ref_counted.h"
#include "core/templates/local_vector.h"
#include "llama_common_defined.h"

struct common_chat_msg;
class LlamaCommonChatMessage;

class LlamaCommonChatMessages : public RefCounted {
	GDCLASS(LlamaCommonChatMessages, RefCounted);

	std::vector<common_chat_msg> messages;

protected:
	static void _bind_methods();

public:
	// Return by value so OOB can safely return a default when using ERR_FAIL_INDEX_V
	common_chat_msg get_msg(uint32_t index) const;
	void set_msg(uint32_t index, const common_chat_msg &p_msg);

	void set_role(uint32_t index, const String &p_role);
	String get_role(uint32_t index) const;

	void set_content(uint32_t index, const String &p_content);
	String get_content(uint32_t index) const;

	// Array of dictionaries with "type" and "text" keys
	void set_content_parts(uint32_t index, const Array &p_parts);
	Array get_content_parts(uint32_t index) const;

	void set_reasoning_content(uint32_t index, const String &p_content);
	String get_reasoning_content(uint32_t index) const;

	void set_tool_name(uint32_t index, const String &p_tool_name);
	String get_tool_name(uint32_t index) const;

	void set_tool_call_id(uint32_t index, const String &p_tool_call_id);
	String get_tool_call_id(uint32_t index) const;

	//

	void set_message(uint32_t index, const Ref<LlamaCommonChatMessage> &p_message);
	Ref<LlamaCommonChatMessage> get_as_message(uint32_t index) const; // return as a copy !

	void push_back(const Ref<LlamaCommonChatMessage> &p_message);
	void pop_back();

	size_t size() const;
	bool is_empty() const;

	void clear();

	static Ref<LlamaCommonChatMessages> from_msg_array(const TypedArray<LlamaCommonChatMessage> &p_array);
	TypedArray<LlamaCommonChatMessage> to_msg_array() const;

	const std::vector<common_chat_msg>& get_messages_as_std_vector() const;
	// Compatibility: older code referenced to_std_vector(); provide it as a by-value alias
	std::vector<common_chat_msg> to_std_vector() const;
	static Ref<LlamaCommonChatMessages> from_std_vector(const std::vector<common_chat_msg> &p_vec);

	static Ref<LlamaCommonChatMessages> create_from_dictionaries_oaicompat(const TypedArray<Dictionary> &p_dicts);
	TypedArray<Dictionary> to_dictionaries_oaicompat(bool concat_typed_text = false) const;

	LlamaCommonChatMessages();
	~LlamaCommonChatMessages();
};
