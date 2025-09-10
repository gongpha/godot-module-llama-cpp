#pragma once

#include "core/object/ref_counted.h"
#include "core/variant/callable.h"
#include "core/variant/typed_array.h"

// NOTE: Name follows user request (double L): LLamaTool, LlamaToolLibrary

class LLamaTool : public RefCounted {
	GDCLASS(LLamaTool, RefCounted);

	String name;
	String description;
	String json_schema; // JSON Schema describing the arguments
	Callable handler;   // Godot callable to execute the tool

protected:
	static void _bind_methods();

public:
	void set_name(const String &p_name) { name = p_name; }
	String get_name() const { return name; }

	void set_description(const String &p_desc) { description = p_desc; }
	String get_description() const { return description; }

	void set_json_schema(const String &p_schema) { json_schema = p_schema; }
	String get_json_schema() const { return json_schema; }

	void set_handler(const Callable &p_callable) { handler = p_callable; }
	Callable get_handler() const { return handler; }

	// Invoke with a Dictionary of arguments, return Variant result (String recommended)
	Variant invoke(const Dictionary &p_args) const;
};

class LlamaToolLibrary : public RefCounted {
	GDCLASS(LlamaToolLibrary, RefCounted);

	TypedArray<LLamaTool> tools;

protected:
	static void _bind_methods();

public:
	void add_tool(const Ref<LLamaTool> &p_tool);
	void clear();
	int get_count() const { return tools.size(); }
	Ref<LLamaTool> get_tool(int p_index) const;
	Ref<LLamaTool> find_tool(const String &p_name) const;
	TypedArray<LLamaTool> get_tools() const { return tools; }
};
