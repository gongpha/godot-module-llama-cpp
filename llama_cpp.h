#pragma once

#include "core/object/object.h"
#include "core/variant/binder_common.h"
#include "core/string/ustring.h"

#include "thirdparty/llama/include/llama.h"

class LlamaCPP : public Object {
	GDCLASS(LlamaCPP, Object);

	static LlamaCPP *singleton;

	bool llama_backend_initialized = false;

	bool logging_level_none_enabled = false;
	bool logging_level_error_enabled = false;
	bool logging_level_warn_enabled = false;
	bool logging_level_info_enabled = false;
	bool logging_level_debug_enabled = false;

	static void _llama_cpp_log_adapter(ggml_log_level level, const char* text, void* user);

public:
	enum LogLevel {
		LOG_LEVEL_NONE = GGML_LOG_LEVEL_NONE,
		LOG_LEVEL_ERROR = GGML_LOG_LEVEL_ERROR,
		LOG_LEVEL_WARN = GGML_LOG_LEVEL_WARN,
		LOG_LEVEL_INFO = GGML_LOG_LEVEL_INFO,
		LOG_LEVEL_DEBUG = GGML_LOG_LEVEL_DEBUG,
	};

protected:
	static void _bind_methods();

	// Forward from llama/ggml into Godot
	static void _log_forward(ggml_log_level p_level, const char *p_text, void *p_user);

public:
	LlamaCPP();
	~LlamaCPP();

	void set_logging_level_none_enabled(bool p_enabled) { logging_level_none_enabled = p_enabled; }
	bool is_logging_level_none_enabled() const { return logging_level_none_enabled; }

	void set_logging_level_error_enabled(bool p_enabled) { logging_level_error_enabled = p_enabled; }
	bool is_logging_level_error_enabled() const { return logging_level_error_enabled; }

	void set_logging_level_warn_enabled(bool p_enabled) { logging_level_warn_enabled = p_enabled; }
	bool is_logging_level_warn_enabled() const { return logging_level_warn_enabled; }

	void set_logging_level_info_enabled(bool p_enabled) { logging_level_info_enabled = p_enabled; }
	bool is_logging_level_info_enabled() const { return logging_level_info_enabled; }

	void set_logging_level_debug_enabled(bool p_enabled) { logging_level_debug_enabled = p_enabled; }
	bool is_logging_level_debug_enabled() const { return logging_level_debug_enabled; }

	static LlamaCPP *get_singleton() { return singleton; }

	void ensure_llama_backend_initialized();

	//

	PackedStringArray get_builtin_templates() const;
	String apply_template(const String &tmpl, const TypedArray<Dictionary> &m_messages, bool add_assistant);
};

VARIANT_ENUM_CAST(LlamaCPP::LogLevel);