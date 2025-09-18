#include <vector>
#include "core/templates/local_vector.h"
#include "../../thirdparty/llama/vendor/nlohmann/json.hpp"

#include "llama_common_defined.h"
#include "llama_common_utils.h"

static const std::map<std::string, std::string>
godot_dict_to_std_map(const TypedDictionary<String, String> &dict) {
	std::map<std::string, std::string> map;
	for (auto it = dict.begin(); it != dict.end(); ++it) {
		String s_key = it->key;
		String s_value = it->value;

		if (s_key.is_empty()) continue;

		map.insert(std::pair<std::string, std::string>(
			std::string(s_key.utf8().get_data()),
			std::string(s_value.utf8().get_data())
		));
	}
	return map;
}

static const TypedDictionary<String, String>
std_map_to_godot_dict(const std::map<std::string, std::string> &map) {
	TypedDictionary<String, String> dict;
	for (const auto &pair : map) {
		dict[String::utf8(pair.first.c_str())] = String::utf8(pair.second.c_str());
	}
	return dict;
}

//

std::map<std::string, std::string> godot_typed_dict_to_std_map(const TypedDictionary<String, String> &dict) {
	return godot_dict_to_std_map(dict);
}

TypedDictionary<String, String> std_map_to_godot_typed_dict(const std::map<std::string, std::string> &map) {
	return std_map_to_godot_dict(map);
}

nlohmann::ordered_json godot_variant_to_nlohmann_json(const Variant &var) {
	if (var.get_type() == Variant::DICTIONARY) {
		Dictionary dict = var;
		nlohmann::ordered_json obj_json = nlohmann::ordered_json::object();
		Array keys = dict.keys();
		for (int i = 0; i < keys.size(); ++i) {
			Variant key = keys[i];
			if (key.get_type() != Variant::STRING) continue;
			String s_key = String(key);
			Variant value = dict[key];
			if (value.get_type() == Variant::DICTIONARY || value.get_type() == Variant::ARRAY) {
				obj_json[s_key.utf8().get_data()] = godot_variant_to_nlohmann_json(value);
			} else if (value.get_type() == Variant::STRING) {
				obj_json[s_key.utf8().get_data()] = String(value).utf8().get_data();
			} else if (value.get_type() == Variant::BOOL) {
				obj_json[s_key.utf8().get_data()] = bool(value);
			} else if (value.get_type() == Variant::INT) {
				obj_json[s_key.utf8().get_data()] = int64_t(value);
			} else if (value.get_type() == Variant::FLOAT) {
				obj_json[s_key.utf8().get_data()] = double(value);
			} else if (value.get_type() == Variant::NIL) {
				obj_json[s_key.utf8().get_data()] = nullptr;
			}
		}
		return obj_json;
	}
	if (var.get_type() == Variant::ARRAY) {
		Array arr = var;
		nlohmann::ordered_json arr_json = nlohmann::ordered_json::array();
		for (int i = 0; i < arr.size(); ++i) {
			Variant item = arr[i];
			if (item.get_type() == Variant::DICTIONARY || item.get_type() == Variant::ARRAY) {
				arr_json.push_back(godot_variant_to_nlohmann_json(item));
			} else if (item.get_type() == Variant::STRING) {
				arr_json.push_back(String(item).utf8().get_data());
			} else if (item.get_type() == Variant::BOOL) {
				arr_json.push_back(bool(item));
			} else if (item.get_type() == Variant::INT) {
				arr_json.push_back(int64_t(item));
			} else if (item.get_type() == Variant::FLOAT) {
				arr_json.push_back(double(item));
			} else if (item.get_type() == Variant::NIL) {
				arr_json.push_back(nullptr);
			}
		}
		return arr_json;
	}

	ERR_FAIL_V_MSG(nlohmann::ordered_json(),
		vformat("Unsupported Variant type for conversion to nlohmann::ordered_json: %d", var.get_type())
	);
}

Variant nlohmann_json_to_godot_variant(const nlohmann::ordered_json &json) {
	if (json.is_object()) {
		Dictionary dict;
		for (auto it = json.begin(); it != json.end(); ++it) {
			const std::string &key = it.key();
			const nlohmann::ordered_json &value = it.value();
			dict[String::utf8(key.c_str())] = nlohmann_json_to_godot_variant(value);
		}
		return dict;
	}
	if (json.is_array()) {
		Array arr;
		for (const auto &el : json) {
			arr.push_back(nlohmann_json_to_godot_variant(el));
		}
		return arr;
	}
	if (json.is_string()) {
		return String::utf8(json.get<std::string>().c_str());
	}
	if (json.is_boolean()) {
		return Variant(json.get<bool>());
	}
	if (json.is_number_integer()) {
		return Variant(json.get<int64_t>());
	}
	if (json.is_number_float()) {
		return Variant(json.get<double>());
	}
	if (json.is_null()) {
		return Variant();
	}

	ERR_FAIL_V_MSG(Variant(),
		vformat("Unsupported nlohmann::ordered_json type for conversion to Godot Variant: %d", json.type())
	);
}