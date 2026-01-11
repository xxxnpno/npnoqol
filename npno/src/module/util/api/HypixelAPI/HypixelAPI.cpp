#include "HypixelAPI.h"

auto HypixelAPI::CheckKey() -> bool
{
	try
	{
		httplib::Result res = cli.Get(std::format("/v2/key?key={}", apiKey));

		if (res->status == 200)
		{
			nlohmann::json jsonResponse = nlohmann::json::parse(res->body);

			if (jsonResponse["cause"] == "Invalid API key")
			{
				return false;
			}

			return true;
		}

		return false;
	}
	catch (...)
	{
		return false;
	}
}

auto HypixelAPI::IsNicked(const nlohmann::json& json) -> bool
{
	try
	{
		return json["player"].is_null();
	}
	catch(...)
	{
		return false;
	}
}

auto HypixelAPI::GetPlayerStats(const std::string& playerName) -> nlohmann::json
{
	try
	{
		httplib::Result res = cli.Get(std::format("/player?key={}&name={}", apiKey, playerName));

		if (res->status == 200)
		{
			return nlohmann::json::parse(res->body);
		}
		else
		{
			return nlohmann::json{};
		}
	}
	catch (...)
	{
		return nlohmann::json{};
	}
}