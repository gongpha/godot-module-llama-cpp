#include "llama_cpp.h"

#include "core/error/error_macros.h"
#include "core/string/print_string.h"

#include "core/object/class_db.h"
#include "core/variant/typed_array.h"

#include "thirdparty/llama/include/llama.h"

LlamaCPP *LlamaCPP::singleton = nullptr;

void LlamaCPP::_llama_cpp_log_adapter(ggml_log_level level, const char* text, void* user) {
	LlamaCPP::_log_forward(level, text, user);
}

void LlamaCPP::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_logging_level_none_enabled", "enabled"), &LlamaCPP::set_logging_level_none_enabled);
	ClassDB::bind_method(D_METHOD("is_logging_level_none_enabled"), &LlamaCPP::is_logging_level_none_enabled);
	ClassDB::bind_method(D_METHOD("set_logging_level_error_enabled", "enabled"), &LlamaCPP::set_logging_level_error_enabled);
	ClassDB::bind_method(D_METHOD("is_logging_level_error_enabled"), &LlamaCPP::is_logging_level_error_enabled);
	ClassDB::bind_method(D_METHOD("set_logging_level_warn_enabled", "enabled"), &LlamaCPP::set_logging_level_warn_enabled);
	ClassDB::bind_method(D_METHOD("is_logging_level_warn_enabled"), &LlamaCPP::is_logging_level_warn_enabled);
	ClassDB::bind_method(D_METHOD("set_logging_level_info_enabled", "enabled"), &LlamaCPP::set_logging_level_info_enabled);
	ClassDB::bind_method(D_METHOD("is_logging_level_info_enabled"), &LlamaCPP::is_logging_level_info_enabled);
	ClassDB::bind_method(D_METHOD("set_logging_level_debug_enabled", "enabled"), &LlamaCPP::set_logging_level_debug_enabled);
	ClassDB::bind_method(D_METHOD("is_logging_level_debug_enabled"), &LlamaCPP::is_logging_level_debug_enabled);
	//ClassDB::bind_method(D_METHOD("enable_logging"), &LlamaCPP::enable_logging);
	//ClassDB::bind_method(D_METHOD("disable_logging"), &LlamaCPP::disable_logging);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "logging_level_none_enabled"), "set_logging_level_none_enabled", "is_logging_level_none_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "logging_level_error_enabled"), "set_logging_level_error_enabled", "is_logging_level_error_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "logging_level_warn_enabled"), "set_logging_level_warn_enabled", "is_logging_level_warn_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "logging_level_info_enabled"), "set_logging_level_info_enabled", "is_logging_level_info_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "logging_level_debug_enabled"), "set_logging_level_debug_enabled", "is_logging_level_debug_enabled");

	ADD_SIGNAL(MethodInfo("llama_log", PropertyInfo(Variant::INT, "level"), PropertyInfo(Variant::STRING, "message")));

	ClassDB::bind_method(D_METHOD("get_builtin_templates"), &LlamaCPP::get_builtin_templates);
	ClassDB::bind_method(D_METHOD("apply_template", "template", "messages", "add_assistant"), &LlamaCPP::apply_template);


	BIND_ENUM_CONSTANT(LOG_LEVEL_NONE);
	BIND_ENUM_CONSTANT(LOG_LEVEL_ERROR);
	BIND_ENUM_CONSTANT(LOG_LEVEL_WARN);
	BIND_ENUM_CONSTANT(LOG_LEVEL_INFO);
	BIND_ENUM_CONSTANT(LOG_LEVEL_DEBUG);
}

LlamaCPP::LlamaCPP() {
	ERR_FAIL_COND(singleton != nullptr);
	singleton = this;
}

LlamaCPP::~LlamaCPP() {
	if (llama_backend_initialized) {
		llama_backend_free();
		llama_backend_initialized = false;
	}

	if (singleton == this) singleton = nullptr;
}

void LlamaCPP::_log_forward(ggml_log_level p_level, const char *p_text, void *p_user) {
	switch (p_level) {
		case GGML_LOG_LEVEL_NONE:
			if (singleton && !singleton->logging_level_none_enabled) {
				return;
			}
			break;
		case GGML_LOG_LEVEL_ERROR:
			if (singleton && !singleton->logging_level_error_enabled) {
				return;
			}
			break;
		case GGML_LOG_LEVEL_WARN:
			if (singleton && !singleton->logging_level_warn_enabled) {
				return;
			}
			break;
		case GGML_LOG_LEVEL_INFO:
			if (singleton && !singleton->logging_level_info_enabled) {
				return;
			}
			break;
		case GGML_LOG_LEVEL_DEBUG:
			if (singleton && !singleton->logging_level_debug_enabled) {
				return;
			}
			break;
		case GGML_LOG_LEVEL_CONT:
			// always allow continuation logs
			break;
		default:
			// unknown level, allow
			break;
	}

	// Emit to Godot log and as signal
	String msg = String::utf8(p_text ? p_text : "");
	// heuristic: treat warnings/errors differently
	switch (p_level) {
		case GGML_LOG_LEVEL_ERROR:
		case GGML_LOG_LEVEL_NONE:
			ERR_PRINT(msg);
			break;
		case GGML_LOG_LEVEL_WARN:
			WARN_PRINT(msg);
			break;
		default:
			print_line(msg);
			break;
	}
	if (singleton) {
		singleton->emit_signal("llama_log", (LogLevel)p_level, msg);
	}
}

void LlamaCPP::ensure_llama_backend_initialized() {
	if (llama_backend_initialized) {
		return;
	}

	llama_backend_initialized = true;

	llama_log_set(_llama_cpp_log_adapter, nullptr);
	ggml_set_abort_callback([](const char* msg) {
		CRASH_NOW_MSG("LLAMA/ggml fatal error: " + String(msg ? msg : "(no message)"));
	});

	llama_backend_init();

	_log_forward(GGML_LOG_LEVEL_INFO, "Llama C++ backend initialized", nullptr);
}

//

PackedStringArray LlamaCPP::get_builtin_templates() const {
	int32_t total = llama_chat_builtin_templates(nullptr, 0);
	PackedStringArray out;
	if (total <= 0) return out;

	const char** templates = memnew_arr(const char*, total);

	int32_t got = llama_chat_builtin_templates(templates, total);
	got = got < 0 ? 0 : got;
	for (int32_t i = 0; i < got; ++i) {
		if (templates[(size_t)i]) out.push_back(String::utf8(templates[(size_t)i]));
	}

	memdelete_arr(templates);

	return out;
}

String LlamaCPP::apply_template(const String &tmpl, const TypedArray<Dictionary> &messages, bool add_assistant) {
	LocalVector<const char *> roles;
	roles.reserve(messages.size());
	LocalVector<const char *> contents;
	contents.reserve(messages.size());
	LocalVector<llama_chat_message> msgs;
	msgs.reserve(messages.size());

	size_t buf_sz = 0;

	for (int i = 0; i < messages.size(); ++i) {
		Dictionary m = messages[i];
		String role = m.get("role", String("user"));
		String content = m.get("content", String());
		roles.push_back(role.utf8().get_data());
		contents.push_back(content.utf8().get_data());
		buf_sz += (size_t)role.length() + (size_t)content.length();
	}
	for (size_t i = 0; i < roles.size(); ++i) {
		llama_chat_message msg;
		msg.role = roles[i];
		msg.content = contents[i];
		msgs.push_back(msg);
	}

	CharString t = tmpl.utf8();
	// Estimate buffer: roughly 2x total input size; grow if needed per return value
	if (buf_sz < 1024)
		buf_sz = 1024; // minimum
	buf_sz *= 2;

	std::string out;
	out.resize(buf_sz);
	int32_t written = llama_chat_apply_template(t.get_data(), msgs.ptr(), msgs.size(), add_assistant, out.data(), (int32_t)out.size());
	if (written < 0) {
		// Negative means required size. Resize and try once more.
		size_t need = (size_t)(-written);
		if (need == 0) return String();
		out.resize(need + 1);
		written = llama_chat_apply_template(t.get_data(), msgs.ptr(), msgs.size(), add_assistant, out.data(), (int32_t)out.size());
		if (written < 0) return String();
	}
	out.resize((size_t)written);
	return String::utf8(out.c_str());
}
