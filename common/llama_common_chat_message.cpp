#include "llama_common_chat_message.h"
#include "llama_common_chat_tool.h"
#include "llama_common_utils.h"
#include "llama_common_defined.h"

#include "core/object/class_db.h"
#include "core/variant/typed_array.h"

void LlamaCommonChatMessage::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_role", "role"), &LlamaCommonChatMessage::set_role);
	ClassDB::bind_method(D_METHOD("get_role"), &LlamaCommonChatMessage::get_role);
	ClassDB::bind_method(D_METHOD("set_content", "content"), &LlamaCommonChatMessage::set_content);
	ClassDB::bind_method(D_METHOD("get_content"), &LlamaCommonChatMessage::get_content);
	ClassDB::bind_method(D_METHOD("set_content_parts", "parts"), &LlamaCommonChatMessage::set_content_parts);
	ClassDB::bind_method(D_METHOD("get_content_parts"), &LlamaCommonChatMessage::get_content_parts);
	ClassDB::bind_method(D_METHOD("set_tool_calls", "tool_calls"), &LlamaCommonChatMessage::set_tool_calls);
	ClassDB::bind_method(D_METHOD("get_tool_calls"), &LlamaCommonChatMessage::get_tool_calls);
	ClassDB::bind_method(D_METHOD("set_reasoning_content", "content"), &LlamaCommonChatMessage::set_reasoning_content);
	ClassDB::bind_method(D_METHOD("get_reasoning_content"), &LlamaCommonChatMessage::get_reasoning_content);
	ClassDB::bind_method(D_METHOD("set_tool_name", "name"), &LlamaCommonChatMessage::set_tool_name);
	ClassDB::bind_method(D_METHOD("get_tool_name"), &LlamaCommonChatMessage::get_tool_name);
	ClassDB::bind_method(D_METHOD("set_tool_call_id", "id"), &LlamaCommonChatMessage::set_tool_call_id);
	ClassDB::bind_method(D_METHOD("get_tool_call_id"), &LlamaCommonChatMessage::get_tool_call_id);

	ClassDB::bind_method(D_METHOD("is_empty"), &LlamaCommonChatMessage::is_empty);

	ClassDB::bind_method(D_METHOD("to_dictionary_oaicompat"), &LlamaCommonChatMessage::to_dictionary_oaicompat);

	ClassDB::bind_static_method("LlamaCommonChatMessage", D_METHOD("create", "role", "content"), &LlamaCommonChatMessage::create);
	ClassDB::bind_static_method("LlamaCommonChatMessage", D_METHOD("create_from_dictionary_oaicompat", "dict"), &LlamaCommonChatMessage::create_from_dictionary_oaicompat);
	ClassDB::bind_static_method("LlamaCommonChatMessage", D_METHOD("parse", "input", "is_partial", "syntax"), &LlamaCommonChatMessage::parse);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "role"), "set_role", "get_role");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "content"), "set_content", "get_content");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "content_parts"), "set_content_parts", "get_content_parts");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "tool_calls"), "set_tool_calls", "get_tool_calls");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "reasoning_content"), "set_reasoning_content", "get_reasoning_content");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "tool_name"), "set_tool_name", "get_tool_name");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "tool_call_id"), "set_tool_call_id", "get_tool_call_id");
}

const common_chat_msg &LlamaCommonChatMessage::get_msg() const { return m_msg; }
void LlamaCommonChatMessage::set_msg(const common_chat_msg &p_msg) { m_msg = p_msg; }

void LlamaCommonChatMessage::set_role(const String &p_role) { m_msg.role = p_role.utf8().get_data(); }
String LlamaCommonChatMessage::get_role() const { return String::utf8(m_msg.role.c_str()); }

void LlamaCommonChatMessage::set_content(const String &p_content) { m_msg.content = p_content.utf8().get_data(); }
String LlamaCommonChatMessage::get_content() const { return String::utf8(m_msg.content.c_str()); }

void LlamaCommonChatMessage::set_content_parts(const Array &p_parts) {
	m_msg.content_parts.clear();
	for (int i = 0; i < p_parts.size(); ++i) {
		Dictionary d = p_parts[i];
		common_chat_msg_content_part part;
		if (d.has("type")) part.type = String(d["type"]).utf8().get_data();
		if (d.has("text")) part.text = String(d["text"]).utf8().get_data();
	m_msg.content_parts.push_back(part);
	}
}

Array LlamaCommonChatMessage::get_content_parts() const {
	Array arr;
	for (const auto &part : m_msg.content_parts) {
		Dictionary d;
		d["type"] = String::utf8(part.type.c_str());
		d["text"] = String::utf8(part.text.c_str());
		arr.push_back(d);
	}
	return arr;
}

void LlamaCommonChatMessage::set_tool_calls(const TypedArray<LlamaCommonChatToolCall> &p_tool_calls) {
	m_msg.tool_calls.clear();
	for (int i = 0; i < p_tool_calls.size(); ++i) {
		Ref<LlamaCommonChatToolCall> tc = p_tool_calls[i];
		if (tc.is_valid()) {
			m_msg.tool_calls.push_back(tc->get_tool_call());
		}
	}
}
TypedArray<LlamaCommonChatToolCall> LlamaCommonChatMessage::get_tool_calls() const {
	TypedArray<LlamaCommonChatToolCall> arr;
	for (const auto &tool_call : m_msg.tool_calls) {
		Ref<LlamaCommonChatToolCall> tc; tc.instantiate();
		tc->set_tool_call(tool_call);
		arr.push_back(tc);
	}
	return arr;
}

void LlamaCommonChatMessage::set_reasoning_content(const String &p_content) { m_msg.reasoning_content = p_content.utf8().get_data(); }
String LlamaCommonChatMessage::get_reasoning_content() const { return String(m_msg.reasoning_content.c_str()); }

void LlamaCommonChatMessage::set_tool_name(const String &p_tool_name) { m_msg.tool_name = p_tool_name.utf8().get_data(); }
String LlamaCommonChatMessage::get_tool_name() const { return String::utf8(m_msg.tool_name.c_str()); }

void LlamaCommonChatMessage::set_tool_call_id(const String &p_tool_call_id) { m_msg.tool_call_id = p_tool_call_id.utf8().get_data(); }
String LlamaCommonChatMessage::get_tool_call_id() const { return String::utf8(m_msg.tool_call_id.c_str()); }

bool LlamaCommonChatMessage::is_empty() const {
	return m_msg.empty();
}

Dictionary LlamaCommonChatMessage::to_dictionary_oaicompat() const {
	using json = nlohmann::ordered_json;
	json j = m_msg.to_json_oaicompat<json>();
	return nlohmann_json_to_godot_variant(j);
}

Ref<LlamaCommonChatMessage> LlamaCommonChatMessage::create(const String &p_role, const String &p_content) {
	Ref<LlamaCommonChatMessage> r; r.instantiate();
	r->m_msg.role = p_role.utf8().get_data();
	r->m_msg.content = p_content.utf8().get_data();
	return r;
}

Ref<LlamaCommonChatMessage> LlamaCommonChatMessage::create_from_dictionary_oaicompat(const Dictionary &p_dict) {
	Ref<LlamaCommonChatMessage> r; r.instantiate();
	if (p_dict.has("role")) r->m_msg.role = String(p_dict["role"]).utf8().get_data();
	if (p_dict.has("content")) r->m_msg.content = String(p_dict["content"]).utf8().get_data();
	if (p_dict.has("reasoning_content")) r->m_msg.reasoning_content = String(p_dict["reasoning_content"]).utf8().get_data();
	if (p_dict.has("tool_calls")) {
		Array tcs = p_dict["tool_calls"];
		for (int i = 0; i < tcs.size(); ++i) {
			Dictionary d = tcs[i];
			common_chat_tool_call tc;
			if (d.has("id")) tc.id = String(d["id"]).utf8().get_data();
			if (d.has("function")) {
				Dictionary fn = d["function"];
				if (fn.has("name")) tc.name = String(fn["name"]).utf8().get_data();
				if (fn.has("arguments")) tc.arguments = String(fn["arguments"]).utf8().get_data();
			}
			r->m_msg.tool_calls.push_back(tc);
		}
	}
	return r;
}

Ref<LlamaCommonChatMessage> LlamaCommonChatMessage::parse(const String &p_input, bool p_is_partial, const Ref<LlamaCommonChatSyntax> &p_syntax) {
	ERR_FAIL_COND_V(p_syntax.is_null(), Ref<LlamaCommonChatMessage>());
	Ref<LlamaCommonChatMessage> message;
	message.instantiate();

	message->set_msg(
		common_chat_parse(
			p_input.utf8().get_data(),
			p_is_partial,
			p_syntax->get_syntax()
		)
	);

	return message;
}

LlamaCommonChatMessage::LlamaCommonChatMessage() {
}

LlamaCommonChatMessage::~LlamaCommonChatMessage() {
}

void LlamaCommonChatMessageDiff::_bind_methods() {

	ClassDB::bind_method(D_METHOD("set_reasoning_content_delta", "content"), &LlamaCommonChatMessageDiff::set_reasoning_content_delta);
	ClassDB::bind_method(D_METHOD("get_reasoning_content_delta"), &LlamaCommonChatMessageDiff::get_reasoning_content_delta);
	ClassDB::bind_method(D_METHOD("set_content_delta", "content"), &LlamaCommonChatMessageDiff::set_content_delta);
	ClassDB::bind_method(D_METHOD("get_content_delta"), &LlamaCommonChatMessageDiff::get_content_delta);
	ClassDB::bind_method(D_METHOD("set_tool_call_index", "index"), &LlamaCommonChatMessageDiff::set_tool_call_index);
	ClassDB::bind_method(D_METHOD("get_tool_call_index"), &LlamaCommonChatMessageDiff::get_tool_call_index);
	ClassDB::bind_method(D_METHOD("set_tool_call_delta", "tool_call"), &LlamaCommonChatMessageDiff::set_tool_call_delta);
	ClassDB::bind_method(D_METHOD("get_tool_call_delta"), &LlamaCommonChatMessageDiff::get_tool_call_delta);
	// Bind methods are defined elsewhere in the file per summary; add properties for the set/get pairs
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "reasoning_content_delta"), "set_reasoning_content_delta", "get_reasoning_content_delta");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "content_delta"), "set_content_delta", "get_content_delta");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "tool_call_index"), "set_tool_call_index", "get_tool_call_index");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "tool_call_delta", PROPERTY_HINT_RESOURCE_TYPE, "LlamaCommonChatToolCall"), "set_tool_call_delta", "get_tool_call_delta");

	ClassDB::bind_static_method("LlamaCommonChatMessageDiff", D_METHOD("create_from_dictionary_oaicompat", "dict"), &LlamaCommonChatMessageDiff::create_from_dictionary_oaicompat);
	ClassDB::bind_method(D_METHOD("to_dictionary_oaicompat"), &LlamaCommonChatMessageDiff::to_dictionary_oaicompat);
	ClassDB::bind_static_method("LlamaCommonChatMessageDiff", D_METHOD("compute_diffs", "old_msg", "new_msg"), &LlamaCommonChatMessageDiff::compute_diffs);
}

const common_chat_msg_diff &LlamaCommonChatMessageDiff::get_msg_diff() const { return m_msg_diff; }
void LlamaCommonChatMessageDiff::set_msg_diff(const common_chat_msg_diff &p_msg_diff) { m_msg_diff = p_msg_diff; }

void LlamaCommonChatMessageDiff::set_reasoning_content_delta(const String &p_content) { m_msg_diff.reasoning_content_delta = p_content.utf8().get_data(); }
String LlamaCommonChatMessageDiff::get_reasoning_content_delta() const { return String::utf8(m_msg_diff.reasoning_content_delta.c_str()); }

void LlamaCommonChatMessageDiff::set_content_delta(const String &p_content) { m_msg_diff.content_delta = p_content.utf8().get_data(); }
String LlamaCommonChatMessageDiff::get_content_delta() const { return String::utf8(m_msg_diff.content_delta.c_str()); }

void LlamaCommonChatMessageDiff::set_tool_call_index(int p_index) { m_msg_diff.tool_call_index = p_index < 0 ? std::string::npos : (size_t)p_index; }
int LlamaCommonChatMessageDiff::get_tool_call_index() const { return m_msg_diff.tool_call_index == std::string::npos ? -1 : (int)m_msg_diff.tool_call_index; }

void LlamaCommonChatMessageDiff::set_tool_call_delta(const Ref<LlamaCommonChatToolCall> &p_tool_call) {
	ERR_FAIL_COND(p_tool_call.is_null());
	m_msg_diff.tool_call_delta = p_tool_call->get_tool_call(); 
}
Ref<LlamaCommonChatToolCall> LlamaCommonChatMessageDiff::get_tool_call_delta() const {
	if (m_msg_diff.tool_call_delta.name.empty() && m_msg_diff.tool_call_delta.arguments.empty() && m_msg_diff.tool_call_delta.id.empty()) {
		return Ref<LlamaCommonChatToolCall>();
	}

	Ref<LlamaCommonChatToolCall> r;
	r.instantiate();
	r->set_tool_call(m_msg_diff.tool_call_delta);
	return r;
}

Ref<LlamaCommonChatMessageDiff> LlamaCommonChatMessageDiff::create_from_dictionary_oaicompat(const Dictionary &p_dict) {
	using json = nlohmann::ordered_json;
	json j = godot_variant_to_nlohmann_json(p_dict);
	Ref<LlamaCommonChatMessageDiff> r; r.instantiate();
	r->m_msg_diff.reasoning_content_delta = j.contains("reasoning_content") ? j.at("reasoning_content").get<std::string>() : std::string();
	r->m_msg_diff.content_delta = j.contains("content") ? j.at("content").get<std::string>() : std::string();
	if (j.contains("tool_calls")) {
		const auto &arr = j.at("tool_calls");
		if (arr.is_array() && !arr.empty()) {
			const auto &tc = arr.at(0);
			r->m_msg_diff.tool_call_index = tc.contains("index") ? (size_t)tc.at("index").get<size_t>() : std::string::npos;
			if (tc.contains("id")) r->m_msg_diff.tool_call_delta.id = tc.at("id");
			if (tc.contains("function")) {
				const auto &fn = tc.at("function");
				if (fn.contains("name")) r->m_msg_diff.tool_call_delta.name = fn.at("name");
				if (fn.contains("arguments")) r->m_msg_diff.tool_call_delta.arguments = fn.at("arguments");
			}
		}
	}
	return r;
}

LlamaCommonJSON LlamaCommonChatMessageDiff::to_dictionary_oaicompat() const {
	using json = nlohmann::ordered_json;
	json j = common_chat_msg_diff_to_json_oaicompat<json>(m_msg_diff);
	return nlohmann_json_to_godot_variant(j);
}

TypedArray<LlamaCommonChatMessageDiff> LlamaCommonChatMessageDiff::compute_diffs(
	const Ref<LlamaCommonChatMessage> &old_msg,
	const Ref<LlamaCommonChatMessage> &new_msg
) {
	ERR_FAIL_COND_V(old_msg.is_null() || new_msg.is_null(), TypedArray<LlamaCommonChatMessageDiff>());
	TypedArray<LlamaCommonChatMessageDiff> ret;
	common_chat_msg prev_msg, curr_msg;
	if (old_msg.is_valid()) {
		prev_msg = old_msg->get_msg();
	}
	if (new_msg.is_valid()) {
		curr_msg = new_msg->get_msg();
	}
	auto diffs = common_chat_msg_diff::compute_diffs(prev_msg, curr_msg);
	for (auto &d : diffs) { Ref<LlamaCommonChatMessageDiff> r; r.instantiate(); r->set_msg_diff(d); ret.push_back(r); }
	return ret;
}

bool LlamaCommonChatMessageDiff::operator==(const LlamaCommonChatMessageDiff &other) const {
	return m_msg_diff == other.m_msg_diff;
}

LlamaCommonChatMessageDiff::LlamaCommonChatMessageDiff() {
}

LlamaCommonChatMessageDiff::~LlamaCommonChatMessageDiff() {
}