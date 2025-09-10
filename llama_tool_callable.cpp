#include "llama_tool_callable.h"

#include "core/object/class_db.h"
#include "core/object/object.h"
#include "core/variant/variant.h"
#include "core/variant/array.h"
#include "core/io/json.h"

static String _variant_to_json_type(Variant::Type t) {
	switch (t) {
	case Variant::Type::INT:
			return "integer";
		case Variant::Type::FLOAT:
			return "number";
		case Variant::Type::BOOL:
			return "boolean";
		case Variant::Type::STRING:
			return "string";
		case Variant::Type::DICTIONARY:
			return "object";
		case Variant::Type::ARRAY:
			return "array";
		default:
			return "string";
	}
}

void LlamaToolCallable::_bind_methods() {}

void LlamaToolCallable::set_handler(const Callable &p_callable) {
	LLamaTool::set_handler(p_callable);

	// attempt to infer name if not set
	String tool_name = get_name();
	if (tool_name.is_empty()) {
		tool_name = p_callable.get_method();
		if (tool_name.is_empty()) tool_name = "callable_tool";
		set_name(tool_name);
	}

	// build a minimal JSON Schema from callable metadata
	bool valid = false;
	const int argc = p_callable.get_argument_count(&valid);
	Array bound = p_callable.get_bound_arguments();
	const int bound_count = bound.size();
	const int unbound = argc - bound_count;

	Dictionary props;
	Array required;

	// we cannot reliably get argument Variant::Type from Callable alone in this context.
	// so we create a permissive schema but mark required based on unbound count
	for (int i = 0; i < unbound; ++i) {
		const String argn = vformat("arg%d", i + 1);
		Dictionary spec;
		spec["type"] = "string"; // best-effort default
		props[argn] = spec;
		required.push_back(argn);
	}

	Dictionary schema;
	schema["type"] = "object";
	schema["properties"] = props;
	schema["required"] = required;

	String schema_json = JSON::stringify(schema);
	set_json_schema(schema_json);
}
