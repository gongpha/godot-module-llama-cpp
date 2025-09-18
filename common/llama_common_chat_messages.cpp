#include "llama_common_chat_messages.h"
#include "llama_common_chat_message.h"
#include "llama_common_chat_tool.h"
#include "llama_common_utils.h"
#include "llama_common_defined.h"

#include "core/object/class_db.h"

void LlamaCommonChatMessages::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_role", "msg_index", "role"), &LlamaCommonChatMessages::set_role);
	ClassDB::bind_method(D_METHOD("get_role", "msg_index"), &LlamaCommonChatMessages::get_role);
	ClassDB::bind_method(D_METHOD("set_content", "msg_index", "content"), &LlamaCommonChatMessages::set_content);
	ClassDB::bind_method(D_METHOD("get_content", "msg_index"), &LlamaCommonChatMessages::get_content);
	ClassDB::bind_method(D_METHOD("set_content_parts", "msg_index", "parts"), &LlamaCommonChatMessages::set_content_parts);
	ClassDB::bind_method(D_METHOD("get_content_parts", "msg_index"), &LlamaCommonChatMessages::get_content_parts);
	ClassDB::bind_method(D_METHOD("set_reasoning_content", "msg_index", "content"), &LlamaCommonChatMessages::set_reasoning_content);
	ClassDB::bind_method(D_METHOD("get_reasoning_content", "msg_index"), &LlamaCommonChatMessages::get_reasoning_content);
	ClassDB::bind_method(D_METHOD("set_tool_name", "msg_index", "name"), &LlamaCommonChatMessages::set_tool_name);
	ClassDB::bind_method(D_METHOD("get_tool_name", "msg_index"), &LlamaCommonChatMessages::get_tool_name);
	ClassDB::bind_method(D_METHOD("set_tool_call_id", "msg_index", "id"), &LlamaCommonChatMessages::set_tool_call_id);
	ClassDB::bind_method(D_METHOD("get_tool_call_id", "msg_index"), &LlamaCommonChatMessages::get_tool_call_id);

	ClassDB::bind_method(D_METHOD("set_message", "index", "message"), &LlamaCommonChatMessages::set_message);
	ClassDB::bind_method(D_METHOD("get_as_message", "index"), &LlamaCommonChatMessages::get_as_message);
	ClassDB::bind_method(D_METHOD("push_back", "message"), &LlamaCommonChatMessages::push_back);
	ClassDB::bind_method(D_METHOD("pop_back"), &LlamaCommonChatMessages::pop_back);
	ClassDB::bind_method(D_METHOD("size"), &LlamaCommonChatMessages::size);
	ClassDB::bind_method(D_METHOD("is_empty"), &LlamaCommonChatMessages::is_empty);
	ClassDB::bind_method(D_METHOD("clear"), &LlamaCommonChatMessages::clear);
	ClassDB::bind_method(D_METHOD("to_msg_array"), &LlamaCommonChatMessages::to_msg_array);

	ClassDB::bind_static_method("LlamaCommonChatMessages", D_METHOD("from_msg_array", "array"), &LlamaCommonChatMessages::from_msg_array);
	ClassDB::bind_static_method("LlamaCommonChatMessages", D_METHOD("create_from_dictionaries_oaicompat", "dicts"), &LlamaCommonChatMessages::create_from_dictionaries_oaicompat);
	ClassDB::bind_method(D_METHOD("to_dictionaries_oaicompat", "concat_typed_text"), &LlamaCommonChatMessages::to_dictionaries_oaicompat, DEFVAL(false));

	// Note: Properties with index parameters aren't directly exponible as standard properties.
}

common_chat_msg LlamaCommonChatMessages::get_msg(uint32_t index) const {
	ERR_FAIL_INDEX_V(index, messages.size(), common_chat_msg());
	return messages[index];
}
void LlamaCommonChatMessages::set_msg(uint32_t index, const common_chat_msg &p_msg) {
	ERR_FAIL_INDEX(index, messages.size());
	messages[index] = p_msg;
}

void LlamaCommonChatMessages::set_role(uint32_t index, const String &p_role) {
	ERR_FAIL_INDEX(index, messages.size());
	messages[index].role = p_role.utf8().get_data();
}
String LlamaCommonChatMessages::get_role(uint32_t index) const {
	ERR_FAIL_INDEX_V(index, messages.size(), String());
	return String::utf8(messages[index].role.c_str());
}

void LlamaCommonChatMessages::set_content(uint32_t index, const String &p_content) {
	ERR_FAIL_INDEX(index, messages.size());
	messages[index].content = p_content.utf8().get_data();
}
String LlamaCommonChatMessages::get_content(uint32_t index) const {
	ERR_FAIL_INDEX_V(index, messages.size(), String());
	return String::utf8(messages[index].content.c_str());
}

void LlamaCommonChatMessages::set_content_parts(uint32_t index, const Array &p_parts) {
	ERR_FAIL_INDEX(index, messages.size());
	messages[index].content_parts.clear();
	for (int i = 0; i < p_parts.size(); ++i) {
		Dictionary d = p_parts[i];
		common_chat_msg_content_part part;
		if (d.has("type")) part.type = String(d["type"]).utf8().get_data();
		if (d.has("text")) part.text = String(d["text"]).utf8().get_data();
		messages[index].content_parts.push_back(part);
	}
}

Array LlamaCommonChatMessages::get_content_parts(uint32_t index) const {
	ERR_FAIL_INDEX_V(index, messages.size(), Array());
	Array arr;
	for (const auto &part : messages[index].content_parts) {
		Dictionary d; d["type"] = String::utf8(part.type.c_str()); d["text"] = String::utf8(part.text.c_str()); arr.push_back(d);
	}
	return arr;
}

void LlamaCommonChatMessages::set_reasoning_content(uint32_t index, const String &p_content) {
	ERR_FAIL_INDEX(index, messages.size());
	messages[index].reasoning_content = p_content.utf8().get_data();
}
String LlamaCommonChatMessages::get_reasoning_content(uint32_t index) const {
	ERR_FAIL_INDEX_V(index, messages.size(), String());
	return String::utf8(messages[index].reasoning_content.c_str());
}

void LlamaCommonChatMessages::set_tool_name(uint32_t index, const String &p_tool_name) {
	ERR_FAIL_INDEX(index, messages.size());
	messages[index].tool_name = p_tool_name.utf8().get_data();
}
String LlamaCommonChatMessages::get_tool_name(uint32_t index) const {
	ERR_FAIL_INDEX_V(index, messages.size(), String());
	return String::utf8(messages[index].tool_name.c_str());
}

void LlamaCommonChatMessages::set_tool_call_id(uint32_t index, const String &p_tool_call_id) {
	ERR_FAIL_INDEX(index, messages.size());
	messages[index].tool_call_id = p_tool_call_id.utf8().get_data();
}
String LlamaCommonChatMessages::get_tool_call_id(uint32_t index) const {
	ERR_FAIL_INDEX_V(index, messages.size(), String());
	return String::utf8(messages[index].tool_call_id.c_str());
}

void LlamaCommonChatMessages::set_message(uint32_t index, const Ref<LlamaCommonChatMessage> &p_message) {
	ERR_FAIL_INDEX(index, messages.size());
	if (p_message.is_valid()) {
		messages[index] = p_message->get_msg();
	}
}
Ref<LlamaCommonChatMessage> LlamaCommonChatMessages::get_as_message(uint32_t index) const {
	ERR_FAIL_INDEX_V(index, messages.size(), Ref<LlamaCommonChatMessage>());
	Ref<LlamaCommonChatMessage> r; r.instantiate(); r->set_msg(messages[index]); return r;
}

void LlamaCommonChatMessages::push_back(const Ref<LlamaCommonChatMessage> &p_message) {
	ERR_FAIL_COND(!p_message.is_valid());
	messages.push_back(p_message->get_msg());
}
void LlamaCommonChatMessages::pop_back() {
	ERR_FAIL_COND(messages.empty());
	messages.pop_back();
}

size_t LlamaCommonChatMessages::size() const {
	return messages.size();
}

bool LlamaCommonChatMessages::is_empty() const {
	return messages.empty();
}

void LlamaCommonChatMessages::clear() {
	messages.clear();
}

Ref<LlamaCommonChatMessages> LlamaCommonChatMessages::from_msg_array(const TypedArray<LlamaCommonChatMessage> &p_array) {
	Ref<LlamaCommonChatMessages> r; r.instantiate();
	for (int i = 0; i < p_array.size(); ++i) {
		Ref<LlamaCommonChatMessage> m = p_array[i]; if (m.is_valid()) r->messages.push_back(m->get_msg());
	}
	return r;
}

TypedArray<LlamaCommonChatMessage> LlamaCommonChatMessages::to_msg_array() const {
	TypedArray<LlamaCommonChatMessage> r;
	r.resize(messages.size());
	for (uint32_t i = 0; i < messages.size(); ++i) {
		Ref<LlamaCommonChatMessage> m;
		m.instantiate();
		m->set_msg(messages[i]);
		r[i] = m;
	}
	return r;
}

Ref<LlamaCommonChatMessages> LlamaCommonChatMessages::create_from_dictionaries_oaicompat(const TypedArray<Dictionary> &p_dicts) {
	using json = nlohmann::ordered_json;
	json arr = json::array();
	for (int i = 0; i < p_dicts.size(); ++i) arr.push_back(godot_variant_to_nlohmann_json(p_dicts[i]));
	auto msgs = common_chat_msgs_parse_oaicompat<json>(arr);
	return from_std_vector(msgs);
}

TypedArray<Dictionary> LlamaCommonChatMessages::to_dictionaries_oaicompat(bool concat_typed_text) const {
	using json = nlohmann::ordered_json;
	std::vector<common_chat_msg> msgs;
	msgs.reserve(messages.size());
	for (uint32_t i = 0; i < messages.size(); ++i)
		msgs.push_back(messages[i]);
	json j = common_chat_msgs_to_json_oaicompat<json>(msgs, concat_typed_text);
	TypedArray<Dictionary> out;
	for (const auto &el : j)
		out.push_back(nlohmann_json_to_godot_variant(el));
	return out;
}

LlamaCommonChatMessages::LlamaCommonChatMessages() {}
LlamaCommonChatMessages::~LlamaCommonChatMessages() {}

const std::vector<common_chat_msg>& LlamaCommonChatMessages::get_messages_as_std_vector() const {
	return messages;
}

std::vector<common_chat_msg> LlamaCommonChatMessages::to_std_vector() const {
	return messages;
}

Ref<LlamaCommonChatMessages> LlamaCommonChatMessages::from_std_vector(const std::vector<common_chat_msg> &p_vec) {
	Ref<LlamaCommonChatMessages> r; r.instantiate();
	r->messages.resize((uint32_t)p_vec.size());
	for (size_t i = 0; i < p_vec.size(); ++i) r->messages[(uint32_t)i] = p_vec[i];
	return r;
}
