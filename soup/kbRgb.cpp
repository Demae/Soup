#include "kbRgb.hpp"

#include "base.hpp"
#if !SOUP_WASM

#include "json.hpp"
#include "kbRgbCorsairCue.hpp"
#include "kbRgbRazerChroma.hpp"
#include "kbRgbWooting.hpp"

namespace soup
{
	std::vector<UniquePtr<kbRgb>> kbRgb::getAll(bool include_razer_chroma, bool include_no_permission)
	{
		std::vector<UniquePtr<kbRgb>> res{};

		if (include_razer_chroma
			&& kbRgbRazerChroma::isAvailable()
			)
		{
			res.emplace_back(soup::make_unique<kbRgbRazerChroma>());
		}

		for (auto& hid : hwHid::getAll())
		{
			if (include_no_permission || hid.havePermission())
			{
				if (hid.usage_page == 0x1337
					&& hid.vendor_id == 0x31E3
					)
				{
					if ((hid.product_id & 0xFFF0) == 0x1100)
					{
						// Untested
						res.emplace_back(soup::make_unique<kbRgbWooting>("Wooting One", false, std::move(hid)));
					}
					else if ((hid.product_id & 0xFFF0) == 0x1200)
					{
						res.emplace_back(soup::make_unique<kbRgbWooting>("Wooting Two", true, std::move(hid)));
					}
					else if ((hid.product_id & 0xFFF0) == 0x1220)
					{
						res.emplace_back(soup::make_unique<kbRgbWooting>("Wooting Two HE", true, std::move(hid)));
					}
				}
			}
		}

#if SOUP_WINDOWS
		if (kbRgbCorsairCue::ensureConnected())
		{
			kbRgbCorsairCue::send(R"({"method":1,"params":{"deviceTypeMask":1,"sizeMax":10},"code":2})");
			auto jr = json::decode(kbRgbCorsairCue::receive());
			for (const auto& dev : jr->asObj().at("result").asObj().at("deviceInfos").asArr())
			{
				res.emplace_back(soup::make_unique<kbRgbCorsairCue>(
					std::move(dev.asObj().at("id").asStr().value),
					std::move(dev.asObj().at("model").asStr().value)
				));
			}
		}
#endif

		return res;
	}

	void kbRgb::setAllKeys(Rgb colour)
	{
		Rgb buf[NUM_KEYS];
		for (auto& c : buf)
		{
			c = colour;
		}
		setKeys(buf);
	}

	void kbRgb::mapPosToKeys(Rgb(&keys)[NUM_KEYS], Rgb* data, uint8_t rows, uint8_t columns)
	{
		for (uint8_t row = 0; row != rows; ++row)
		{
			for (uint8_t column = 0; column != columns; ++column)
			{
				if (auto sk = mapPosToKey(row, column); sk != KEY_NONE)
				{
					keys[sk] = data[row * columns + column];
				}
			}
		}
	}

	std::pair<uint8_t, uint8_t> kbRgb::mapKeyToPos(Key key) const noexcept
	{
		const uint8_t rows = getNumRows();
		const uint8_t columns = getNumColumns();
		for (uint8_t row = 0; row != rows; ++row)
		{
			for (uint8_t column = 0; column != columns; ++column)
			{
				if (mapPosToKey(row, column) == key)
				{
					return { row, column };
				}
			}
		}
		return { 0xff, 0xff };
	}
}
#endif
