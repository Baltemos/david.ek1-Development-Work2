#include "JsonUtil.h"

nlohmann::json JsonMerge(const nlohmann::json& aFirst, const nlohmann::json& aSecond, bool aTotalObjectMerge)
{
	nlohmann::json result;

	if (aFirst.is_array())
	{
		result = nlohmann::json(nlohmann::detail::value_t::array);

		if (aSecond.is_array())
		{
			for (size_t ind = 0; ind < aSecond.size(); ind++)
			{
				auto& value = aSecond[ind];

				if (aFirst.size() > ind)
				{
					if (value.is_null() == false)
					{
						result += JsonMerge(aFirst[ind], value, aTotalObjectMerge);
					}
					else
					{
						result += aFirst[ind];
					}
				}
				else
				{
					result += value;
				}

			}
		}
		else
		{
			result = aFirst;
		}
	}
	else if (aFirst.is_object())
	{
		result = aFirst;

		if (aSecond.is_object())
		{
			for (auto& entry : aSecond.items())
			{
				if (result.contains(entry.key()))
				{
					result[entry.key()] = JsonMerge(result[entry.key()], entry.value(), aTotalObjectMerge);
				}
				else if (aTotalObjectMerge)
				{
					result[entry.key()] = entry.value();
				}
			}
		}
	}
	else
	{
		result = aSecond;
	}

	return result;
}
