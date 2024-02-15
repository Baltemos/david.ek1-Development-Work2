#include "JsonUtil.h"

nlohmann::json JsonMerge(const nlohmann::json& aFirst, const nlohmann::json& aSecond, bool aSkipIfNull)
{
	nlohmann::json result = aFirst;

	if (aFirst.is_object() && aSecond.is_object())
	{
		for (auto& entry : aSecond.items())
		{
			if (result.contains(entry.key()))
			{
				result[entry.key()] = JsonMerge(result[entry.key()], entry.value(), false);
			}
			else
			{
				result[entry.key()] = entry.value();
			}
		}
	}
	else if (!aSkipIfNull || !aSecond.is_null())
	{
		result = aSecond;
	}

	return result;
}
