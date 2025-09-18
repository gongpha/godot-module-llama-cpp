#pragma once

#include <map>
#include <nlohmann/json.hpp>

#include "core/variant/typed_dictionary.h"
#include "core/variant/dictionary.h"
#include "core/variant/array.h"

// Conversions between Godot typed dictionary and std::map
std::map<std::string, std::string> godot_typed_dict_to_std_map(const TypedDictionary<String, String> &dict);
TypedDictionary<String, String> std_map_to_godot_typed_dict(const std::map<std::string, std::string> &map);

nlohmann::ordered_json godot_variant_to_nlohmann_json(const Variant &var);
Variant nlohmann_json_to_godot_variant(const nlohmann::ordered_json &json);
