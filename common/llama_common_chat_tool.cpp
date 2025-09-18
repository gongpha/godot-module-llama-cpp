#include "llama_common_chat_tool.h"
#include "llama_common_utils.h"
#include "llama_common_defined.h"

#include "../../thirdparty/llama/common/chat.h"

#include "core/object/class_db.h"

void LlamaCommonChatTool::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_tool_name", "name"), &LlamaCommonChatTool::set_name);
	ClassDB::bind_method(D_METHOD("get_tool_name"), &LlamaCommonChatTool::get_name);
	ClassDB::bind_method(D_METHOD("set_description", "description"), &LlamaCommonChatTool::set_description);
	ClassDB::bind_method(D_METHOD("get_description"), &LlamaCommonChatTool::get_description);
	ClassDB::bind_method(D_METHOD("set_parameters", "parameters"), &LlamaCommonChatTool::set_parameters);
	ClassDB::bind_method(D_METHOD("get_parameters"), &LlamaCommonChatTool::get_parameters);

	ClassDB::bind_method(D_METHOD("create_from_dictionary_oaicompat", "dict"), &LlamaCommonChatTool::create_from_dictionary_oaicompat);
	ClassDB::bind_method(D_METHOD("to_dictionary_oaicompat"), &LlamaCommonChatTool::to_dictionary_oaicompat);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "tool_name"), "set_tool_name", "get_tool_name");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "description"), "set_description", "get_description");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "parameters"), "set_parameters", "get_parameters");
}

void LlamaCommonChatTool::set_tool(const common_chat_tool &p_tool) {
	tool = p_tool;
}
const common_chat_tool &LlamaCommonChatTool::get_tool() const {
	return tool;
}

void LlamaCommonChatTool::set_name(const String &p_name) { tool.name = p_name.utf8().get_data(); }
String LlamaCommonChatTool::get_name() const { return String::utf8(tool.name.c_str()); }

void LlamaCommonChatTool::set_description(const String &p_description) { tool.description = p_description.utf8().get_data(); }
String LlamaCommonChatTool::get_description() const { return String::utf8(tool.description.c_str()); }

void LlamaCommonChatTool::set_parameters(const String &p_parameters) { tool.parameters = p_parameters.utf8().get_data(); }
String LlamaCommonChatTool::get_parameters() const { return String::utf8(tool.parameters.c_str()); }

bool LlamaCommonChatTool::operator==(const LlamaCommonChatTool &other) const { return tool.name == other.tool.name && tool.description == other.tool.description && tool.parameters == other.tool.parameters; }

Ref<LlamaCommonChatTool> LlamaCommonChatTool::create_from_dictionary_oaicompat(const Dictionary &p_dict) {
	using json = nlohmann::ordered_json;
	Ref<LlamaCommonChatTool> res;
	res.instantiate();
	// Expect OpenAI tool format {type:"function", function: { name, description, parameters }} (parameters as JSON string)
	if (p_dict.has("function")) {
		Dictionary fn = p_dict["function"];
		if (fn.has("name")) res->set_name(fn["name"]);
		if (fn.has("description")) res->set_description(fn["description"]);
		if (fn.has("parameters")) {
			nlohmann::ordered_json pj = godot_variant_to_nlohmann_json(fn["parameters"]);
			res->set_parameters(String::utf8(pj.dump().c_str()));
		}
	}
	return res;
}

LlamaCommonJSON LlamaCommonChatTool::to_dictionary_oaicompat() const {
	using json = nlohmann::ordered_json;
	json j = {
		{"type", "function"},
		{"function", {
			{"name", tool.name},
			{"description", tool.description},
			{"parameters", json::parse(tool.parameters.empty() ? std::string("{}") : tool.parameters)}
		}}
	};
	return nlohmann_json_to_godot_variant(j);
}

LlamaCommonChatTool::LlamaCommonChatTool() {
}
LlamaCommonChatTool::~LlamaCommonChatTool() {
}

// ToolCall

void LlamaCommonChatToolCall::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_tool_name", "name"), &LlamaCommonChatToolCall::set_name);
	ClassDB::bind_method(D_METHOD("get_tool_name"), &LlamaCommonChatToolCall::get_name);
	ClassDB::bind_method(D_METHOD("set_arguments", "arguments"), &LlamaCommonChatToolCall::set_arguments);
	ClassDB::bind_method(D_METHOD("get_arguments"), &LlamaCommonChatToolCall::get_arguments);
	ClassDB::bind_method(D_METHOD("set_id", "id"), &LlamaCommonChatToolCall::set_id);
	ClassDB::bind_method(D_METHOD("get_id"), &LlamaCommonChatToolCall::get_id);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "tool_name"), "set_tool_name", "get_tool_name");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "arguments"), "set_arguments", "get_arguments");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "id"), "set_id", "get_id");
}

void LlamaCommonChatToolCall::set_name(const String &p_name) { tool_call.name = p_name.utf8().get_data(); }
String LlamaCommonChatToolCall::get_name() const { return String::utf8(tool_call.name.c_str()); }

void LlamaCommonChatToolCall::set_arguments(const String &p_arguments) { tool_call.arguments = p_arguments.utf8().get_data(); }
String LlamaCommonChatToolCall::get_arguments() const { return String::utf8(tool_call.arguments.c_str()); }

void LlamaCommonChatToolCall::set_id(const String &p_id) { tool_call.id = p_id.utf8().get_data(); }
String LlamaCommonChatToolCall::get_id() const { return String::utf8(tool_call.id.c_str()); }

bool LlamaCommonChatToolCall::operator==(const LlamaCommonChatToolCall &other) const { return tool_call == other.tool_call; }

LlamaCommonChatToolCall::LlamaCommonChatToolCall() {
}
LlamaCommonChatToolCall::~LlamaCommonChatToolCall() {
}
