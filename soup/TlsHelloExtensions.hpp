#pragma once

#include "Packet.hpp"

namespace soup
{
	SOUP_PACKET(TlsHelloExtension)
	{
		uint16_t id;
		std::string data;

		SOUP_PACKET_IO(s)
		{
			return s.u16(id)
				&& s.template str_lp<u16_t>(data)
				;
		}
	};

	SOUP_PACKET(TlsHelloExtensions)
	{
		std::vector<TlsHelloExtension> extensions{};

		SOUP_PACKET_IO(s)
		{
			if (s.isRead())
			{
				extensions.clear();
				if (s.hasMore())
				{
					uint16_t extension_bytes;
					if (!s.u16(extension_bytes))
					{
						return false;
					}
					while (extension_bytes >= 4)
					{
						TlsHelloExtension ext;
						if (!ext.io(s))
						{
							return false;
						}
						extension_bytes -= (4 + ext.data.size());
						extensions.emplace_back(std::move(ext));
					}
				}
			}
			else if (s.isWrite())
			{
				if (!extensions.empty())
				{
					std::string ext_data{};
					for (auto& ext : extensions)
					{
						ext_data.append(ext.toBinaryString());
					}
					s.template str_lp<u16_t>(ext_data);
				}
			}
			return true;
		}

		template <typename T>
		void add(uint16_t ext_id, Packet<T>& ext_packet)
		{
			return add(ext_id, ext_packet.toBinaryString());
		}

		void add(uint16_t ext_id, std::string&& ext_data) noexcept
		{
			TlsHelloExtension ext;
			ext.id = ext_id;
			ext.data = std::move(ext_data);
			extensions.emplace_back(std::move(ext));
		}
	};
}
