#pragma once

#include "bigint.hpp"

namespace soup
{
	struct rsa
	{
		struct key
		{
			bigint n;
			bigint e;

			[[nodiscard]] bigint modPow(const bigint& x) const;

			[[nodiscard]] bigint encryptUnpadded(const std::string& msg) const;
			[[nodiscard]] std::string decryptUnpadded(const bigint& enc) const;

			[[nodiscard]] bigint sign(const bigint& hash) const;
			[[nodiscard]] bool verify(const bigint& hash, const bigint& sig) const;
		};

		struct keypair
		{
			bigint n;
			bigint e;
			bigint d;

			void random(int bits);
			void fromPrimes(const bigint& p, const bigint& q);

			[[nodiscard]] key getPublic() const;
			[[nodiscard]] key getPrivate() const;
		};
	};
}
