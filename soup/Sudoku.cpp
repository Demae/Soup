#include "Sudoku.hpp"

#include "bitutil.hpp"
#include "RasterFont.hpp"
#include "RenderTarget.hpp"
#include "Rgb.hpp"

namespace soup
{
	using index_t = Sudoku::index_t;
	using value_t = Sudoku::value_t;
	using mask_t = Sudoku::mask_t;
	using count_t = Sudoku::count_t;

	value_t Sudoku::Field::getValue() const noexcept
	{
		return bitutil::getLeastSignificantSetBit(value_bf) + 1;
	}

	count_t Sudoku::Field::getNumCandidates() const noexcept
	{
		return bitutil::getNumSetBits(candidates_bf);
	}

	void Sudoku::Field::draw(RenderTarget& rt, int x, int y) const
	{
		rt.drawRect(x, y, 15, 15, Rgb::BLACK);
		if (value_bf != 0)
		{
			rt.drawText(x + 3, y, std::u32string(1, '0' + getValue()), RasterFont::simple5(), value_was_given ? Rgb::WHITE : Rgb::BLUE, 3);
		}
		else
		{
			if (candidates_bf & (1 << 0))
			{
				rt.drawText(x + 1, y + 0, U"1", RasterFont::simple5(), Rgb::WHITE, 1);
			}
			if (candidates_bf & (1 << 1))
			{
				rt.drawText(x + 6, y + 0, U"2", RasterFont::simple5(), Rgb::WHITE, 1);
			}
			if (candidates_bf & (1 << 2))
			{
				rt.drawText(x + 11, y + 0, U"3", RasterFont::simple5(), Rgb::WHITE, 1);
			}
			if (candidates_bf & (1 << 3))
			{
				rt.drawText(x + 1, y + 5, U"4", RasterFont::simple5(), Rgb::WHITE, 1);
			}
			if (candidates_bf & (1 << 4))
			{
				rt.drawText(x + 6, y + 5, U"5", RasterFont::simple5(), Rgb::WHITE, 1);
			}
			if (candidates_bf & (1 << 5))
			{
				rt.drawText(x + 11, y + 5, U"6", RasterFont::simple5(), Rgb::WHITE, 1);
			}
			if (candidates_bf & (1 << 6))
			{
				rt.drawText(x + 1, y + 10, U"7", RasterFont::simple5(), Rgb::WHITE, 1);
			}
			if (candidates_bf & (1 << 7))
			{
				rt.drawText(x + 6, y + 10, U"8", RasterFont::simple5(), Rgb::WHITE, 1);
			}
			if (candidates_bf & (1 << 8))
			{
				rt.drawText(x + 11, y + 10, U"9", RasterFont::simple5(), Rgb::WHITE, 1);
			}
		}
	}

	mask_t Sudoku::getValuesInBox(index_t i) const noexcept
	{
		index_t bx = i % 3;
		index_t by = i / 3;
		return getValuesInBox(bx, by);
	}

	mask_t Sudoku::getValuesInBox(index_t bx, index_t by) const noexcept
	{
		return getField((bx * 3) + 0, (by * 3) + 0).value_bf
			|  getField((bx * 3) + 1, (by * 3) + 0).value_bf
			|  getField((bx * 3) + 2, (by * 3) + 0).value_bf
			|  getField((bx * 3) + 0, (by * 3) + 1).value_bf
			|  getField((bx * 3) + 1, (by * 3) + 1).value_bf
			|  getField((bx * 3) + 2, (by * 3) + 1).value_bf
			|  getField((bx * 3) + 0, (by * 3) + 2).value_bf
			|  getField((bx * 3) + 1, (by * 3) + 2).value_bf
			|  getField((bx * 3) + 2, (by * 3) + 2).value_bf
			;
	}

	mask_t Sudoku::getValuesInRow(index_t y) const noexcept
	{
		mask_t value_bf = 0;
		for (uint8_t x = 0; x != 9; ++x)
		{
			value_bf |= getField(x, y).value_bf;
		}
		return value_bf;
	}

	mask_t Sudoku::getValuesInColumn(index_t x) const noexcept
	{
		mask_t value_bf = 0;
		for (uint8_t y = 0; y != 9; ++y)
		{
			value_bf |= getField(x, y).value_bf;
		}
		return value_bf;
	}

	mask_t Sudoku::getCandidatesInBox(mask_t value_bf, index_t bx, index_t by) const noexcept
	{
		return ((1 << 0) * getField((bx * 3) + 0, (by * 3) + 0).isCandidateMask(value_bf))
			|  ((1 << 1) * getField((bx * 3) + 1, (by * 3) + 0).isCandidateMask(value_bf))
			|  ((1 << 2) * getField((bx * 3) + 2, (by * 3) + 0).isCandidateMask(value_bf))
			|  ((1 << 3) * getField((bx * 3) + 0, (by * 3) + 1).isCandidateMask(value_bf))
			|  ((1 << 4) * getField((bx * 3) + 1, (by * 3) + 1).isCandidateMask(value_bf))
			|  ((1 << 5) * getField((bx * 3) + 2, (by * 3) + 1).isCandidateMask(value_bf))
			|  ((1 << 6) * getField((bx * 3) + 0, (by * 3) + 2).isCandidateMask(value_bf))
			|  ((1 << 7) * getField((bx * 3) + 1, (by * 3) + 2).isCandidateMask(value_bf))
			|  ((1 << 8) * getField((bx * 3) + 2, (by * 3) + 2).isCandidateMask(value_bf))
			;
	}

	mask_t Sudoku::getCandidatesInRow(mask_t value_bf, index_t y) const noexcept
	{
		mask_t candidates_bf = 0;
		for (uint8_t x = 0; x != 9; ++x)
		{
			if (getField(x, y).isCandidateMask(value_bf))
			{
				candidates_bf |= (1 << x);
			}
		}
		return candidates_bf;
	}

	mask_t Sudoku::getCandidatesInColumn(mask_t value_bf, index_t x) const noexcept
	{
		mask_t candidates_bf = 0;
		for (uint8_t y = 0; y != 9; ++y)
		{
			if (getField(x, y).isCandidateMask(value_bf))
			{
				candidates_bf |= (1 << y);
			}
		}
		return candidates_bf;
	}

	count_t Sudoku::getNumCandidatesInBox(mask_t value_bf, index_t bx, index_t by) const noexcept
	{
		return getField((bx * 3) + 0, (by * 3) + 0).isCandidateMask(value_bf)
			+  getField((bx * 3) + 1, (by * 3) + 0).isCandidateMask(value_bf)
			+  getField((bx * 3) + 2, (by * 3) + 0).isCandidateMask(value_bf)
			+  getField((bx * 3) + 0, (by * 3) + 1).isCandidateMask(value_bf)
			+  getField((bx * 3) + 1, (by * 3) + 1).isCandidateMask(value_bf)
			+  getField((bx * 3) + 2, (by * 3) + 1).isCandidateMask(value_bf)
			+  getField((bx * 3) + 0, (by * 3) + 2).isCandidateMask(value_bf)
			+  getField((bx * 3) + 1, (by * 3) + 2).isCandidateMask(value_bf)
			+  getField((bx * 3) + 2, (by * 3) + 2).isCandidateMask(value_bf)
			;
	}

	count_t Sudoku::getNumCandidatesInRow(mask_t value_bf, index_t y) const noexcept
	{
		count_t res = 0;
		for (index_t x = 0; x != 9; ++x)
		{
			res += getField(x, y).isCandidateMask(value_bf);
		}
		return res;
	}

	count_t Sudoku::getNumCandidatesInColumn(mask_t value_bf, index_t x) const noexcept
	{
		count_t res = 0;
		for (index_t y = 0; y != 9; ++y)
		{
			res += getField(x, y).isCandidateMask(value_bf);
		}
		return res;
	}

	void Sudoku::eliminateImpossibleCandiates() noexcept
	{
		for (index_t y = 0; y != 9; ++y)
		{
			for (index_t x = 0; x != 9; ++x)
			{
				if (getField(x, y).value_bf != 0)
				{
					getField(x, y).candidates_bf = 0;
				}
				else
				{
					index_t bx = x / 3;
					index_t by = y / 3;

					getField(x, y).candidates_bf &= ~getValuesInBox(bx, by);
					getField(x, y).candidates_bf &= ~getValuesInRow(y);
					getField(x, y).candidates_bf &= ~getValuesInColumn(x);
				}
			}
		}
	}

	bool Sudoku::eliminateCandidate(mask_t value_bf, index_t x, index_t y) noexcept
	{
		if (getField(x, y).candidates_bf & value_bf)
		{
			getField(x, y).candidates_bf &= ~value_bf;
			return true;
		}
		return false;
	}

	bool Sudoku::narrowCandidatesInRowToBox(mask_t value_bf, index_t y, index_t pin_bx) noexcept
	{
		bool changed = false;
		for (index_t x = 0; x != 9; ++x)
		{
			index_t bx = x / 3;

			if (bx != pin_bx)
			{
				changed |= eliminateCandidate(value_bf, x, y);
			}
		}
		return changed;
	}

	bool Sudoku::narrowCandidatesInColumnToBox(mask_t value_bf, index_t x, index_t pin_by) noexcept
	{
		bool changed = false;
		for (index_t y = 0; y != 9; ++y)
		{
			index_t by = y / 3;

			if (by != pin_by)
			{
				changed |= eliminateCandidate(value_bf, x, y);
			}
		}
		return changed;
	}

	bool Sudoku::narrowCandidatesInBoxToRow(mask_t value_bf, index_t bx, index_t by, index_t pin_y) noexcept
	{
		bool changed = false;
		if ((by * 3) + 0 != pin_y)
		{
			changed |= eliminateCandidate(value_bf, (bx * 3) + 0, (by * 3) + 0);
			changed |= eliminateCandidate(value_bf, (bx * 3) + 1, (by * 3) + 0);
			changed |= eliminateCandidate(value_bf, (bx * 3) + 2, (by * 3) + 0);
		}
		if ((by * 3) + 1 != pin_y)
		{
			changed |= eliminateCandidate(value_bf, (bx * 3) + 0, (by * 3) + 1);
			changed |= eliminateCandidate(value_bf, (bx * 3) + 1, (by * 3) + 1);
			changed |= eliminateCandidate(value_bf, (bx * 3) + 2, (by * 3) + 1);
		}
		if ((by * 3) + 2 != pin_y)
		{
			changed |= eliminateCandidate(value_bf, (bx * 3) + 0, (by * 3) + 2);
			changed |= eliminateCandidate(value_bf, (bx * 3) + 1, (by * 3) + 2);
			changed |= eliminateCandidate(value_bf, (bx * 3) + 2, (by * 3) + 2);
		}
		return changed;
	}

	bool Sudoku::narrowCandidatesInBoxToColumn(mask_t value_bf, index_t bx, index_t by, index_t pin_x) noexcept
	{
		bool changed = false;
		if ((bx * 3) + 0 != pin_x)
		{
			changed |= eliminateCandidate(value_bf, (bx * 3) + 0, (by * 3) + 0);
			changed |= eliminateCandidate(value_bf, (bx * 3) + 0, (by * 3) + 1);
			changed |= eliminateCandidate(value_bf, (bx * 3) + 0, (by * 3) + 2);
		}
		if ((bx * 3) + 1 != pin_x)
		{
			changed |= eliminateCandidate(value_bf, (bx * 3) + 1, (by * 3) + 0);
			changed |= eliminateCandidate(value_bf, (bx * 3) + 1, (by * 3) + 1);
			changed |= eliminateCandidate(value_bf, (bx * 3) + 1, (by * 3) + 2);
		}
		if ((bx * 3) + 2 != pin_x)
		{
			changed |= eliminateCandidate(value_bf, (bx * 3) + 2, (by * 3) + 0);
			changed |= eliminateCandidate(value_bf, (bx * 3) + 2, (by * 3) + 1);
			changed |= eliminateCandidate(value_bf, (bx * 3) + 2, (by * 3) + 2);
		}
		return changed;
	}

	bool Sudoku::eliminateCandidatesInRow(mask_t value_bf, index_t y, index_t exclude_x1, index_t exclude_x2) noexcept
	{
		bool changed = false;
		for (index_t x = 0; x != 9; ++x)
		{
			if (x != exclude_x1 && x != exclude_x2)
			{
				changed |= eliminateCandidate(value_bf, x, y);
			}
		}
		return changed;
	}

	bool Sudoku::eliminateCandidatesInColumn(mask_t value_bf, index_t x, index_t exclude_y1, index_t exclude_y2) noexcept
	{
		bool changed = false;
		for (index_t y = 0; y != 9; ++y)
		{
			if (y != exclude_y1 && y != exclude_y2)
			{
				changed |= eliminateCandidate(value_bf, x, y);
			}
		}
		return changed;
	}

	bool Sudoku::stepAny() noexcept
	{
		return stepNakedSingle()
			|| stepHiddenSingle()
			|| stepLockedCandidates()
			|| stepHiddenPair()
			|| stepXWing()
			;
	}

	bool Sudoku::stepNakedSingle() noexcept
	{
		for (index_t i = 0; i != 9 * 9; ++i)
		{
			if (fields[i].getNumCandidates() == 1)
			{
				fields[i].value_bf = fields[i].candidates_bf;
				return true;
			}
		}
		return false;
	}

	bool Sudoku::stepHiddenSingle() noexcept
	{
		for (index_t y = 0; y != 9; ++y)
		{
			for (index_t x = 0; x != 9; ++x)
			{
				index_t bx = x / 3;
				index_t by = y / 3;

				auto candidates_bf = getField(x, y).candidates_bf;
				while (candidates_bf)
				{
					value_t value = bitutil::getLeastSignificantSetBit(candidates_bf) + 1;
					const auto value_bf = valueToMask(value);

					if (getNumCandidatesInBox(value_bf, bx, by) == 1
						|| getNumCandidatesInRow(value_bf, y) == 1
						|| getNumCandidatesInColumn(value_bf, x) == 1
						)
					{
						getField(x, y).value_bf = value_bf;
						return true;
					}

					bitutil::unsetLeastSignificantSetBit(candidates_bf);
				}
			}
		}
		return false;
	}

	bool Sudoku::stepLockedCandidates() noexcept
	{
		for (value_t value = 1; value != 10; ++value)
		{
			const auto value_bf = valueToMask(value);

			// Type 1
			for (index_t by = 0; by != 3; ++by)
			{
				for (index_t bx = 0; bx != 3; ++bx)
				{
					const auto candidates = getCandidatesInBox(value_bf, bx, by);
					if (((candidates & 0b000'000'111) != 0)
						&& ((candidates & 0b111'111'000) == 0)
						)
					{
						if (narrowCandidatesInRowToBox(value_bf, (by * 3) + 0, bx))
						{
							return true;
						}
					}
					else if (((candidates & 0b000'111'000) != 0)
						&& ((candidates & 0b111'000'111) == 0)
						)
					{
						if (narrowCandidatesInRowToBox(value_bf, (by * 3) + 1, bx))
						{
							return true;
						}
					}
					else if (((candidates & 0b111'000'000) != 0)
						&& ((candidates & 0b000'111'111) == 0)
						)
					{
						if (narrowCandidatesInRowToBox(value_bf, (by * 3) + 2, bx))
						{
							return true;
						}
					}
					else if (((candidates & 0b001'001'001) != 0)
						&& ((candidates & 0b110'110'110) == 0)
						)
					{
						if (narrowCandidatesInColumnToBox(value_bf, (bx * 3) + 0, by))
						{
							return true;
						}
					}
					else if (((candidates & 0b010'010'010) != 0)
						&& ((candidates & 0b101'101'101) == 0)
						)
					{
						if (narrowCandidatesInColumnToBox(value_bf, (bx * 3) + 1, by))
						{
							return true;
						}
					}
					else if (((candidates & 0b100'100'100) != 0)
						&& ((candidates & 0b011'011'011) == 0)
						)
					{
						if (narrowCandidatesInColumnToBox(value_bf, (bx * 3) + 2, by))
						{
							return true;
						}
					}
				}
			}

			// Type 2
			for (index_t y = 0; y != 9; ++y)
			{
				index_t by = y / 3;

				auto candidates = getCandidatesInRow(value_bf, y);
				if (((candidates & 0b000'000'111) != 0)
					&& ((candidates & 0b111'111'000) == 0)
					)
				{
					if (narrowCandidatesInBoxToRow(value_bf, 0, by, y))
					{
						return true;
					}
				}
				else if (((candidates & 0b000'111'000) != 0)
					&& ((candidates & 0b111'000'111) == 0)
					)
				{
					if (narrowCandidatesInBoxToRow(value_bf, 1, by, y))
					{
						return true;
					}
				}
				else if (((candidates & 0b111'000'000) != 0)
					&& ((candidates & 0b000'111'111) == 0)
					)
				{
					if (narrowCandidatesInBoxToRow(value_bf, 2, by, y))
					{
						return true;
					}
				}
			}

			for (index_t x = 0; x != 9; ++x)
			{
				index_t bx = x / 3;

				auto candidates = getCandidatesInColumn(value_bf, x);
				if (((candidates & 0b000'000'111) != 0)
					&& ((candidates & 0b111'111'000) == 0)
					)
				{
					if (narrowCandidatesInBoxToColumn(value_bf, bx, 0, x))
					{
						return true;
					}
				}
				else if (((candidates & 0b000'111'000) != 0)
					&& ((candidates & 0b111'000'111) == 0)
					)
				{
					if (narrowCandidatesInBoxToColumn(value_bf, bx, 1, x))
					{
						return true;
					}
				}
				else if (((candidates & 0b111'000'000) != 0)
					&& ((candidates & 0b000'111'111) == 0)
					)
				{
					if (narrowCandidatesInBoxToColumn(value_bf, bx, 2, x))
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	bool Sudoku::stepHiddenPair() noexcept
	{
		for (value_t value1 = 1; value1 != 10; ++value1)
		{
			for (value_t value2 = 1; value2 != 10; ++value2)
			{
				if (value1 != value2)
				{
					const mask_t value1_bf = valueToMask(value1);
					const mask_t value2_bf = valueToMask(value2);

					// Row
					for (index_t y = 0; y != 9; ++y)
					{
						auto candidates = getCandidatesInRow(value1_bf, y);
						if (candidates == getCandidatesInRow(value2_bf, y)
							&& bitutil::getNumSetBits(candidates) == 2
							)
						{
							//std::cout << "Found hidden pair on " << (int)value1 << " and " << (int)value2 << " in row " << (y + 1) << "\n";
							bool changed = false;
							do
							{
								index_t x = bitutil::getLeastSignificantSetBit(candidates);
								changed |= eliminateCandidate(~(value1_bf | value2_bf), x, y);
							} while (bitutil::unsetLeastSignificantSetBit(candidates), candidates);
							if (changed)
							{
								return true;
							}
						}
					}

					// Column
					for (index_t x = 0; x != 9; ++x)
					{
						auto candidates = getCandidatesInColumn(value1_bf, x);
						if (candidates == getCandidatesInColumn(value2_bf, x)
							&& bitutil::getNumSetBits(candidates) == 2
							)
						{
							//std::cout << "Found hidden pair on " << (int)value1 << " and " << (int)value2 << " in column " << (x + 1) << "\n";
							bool changed = false;
							do
							{
								index_t y = bitutil::getLeastSignificantSetBit(candidates);
								changed |= eliminateCandidate(~(value1_bf | value2_bf), x, y);
							} while (bitutil::unsetLeastSignificantSetBit(candidates), candidates);
							if (changed)
							{
								return true;
							}
						}
					}

					// Could technically also do this for boxes although I think row & column is enough to pick everything up.
				}
			}
		}
		return false;
	}

	bool Sudoku::stepXWing() noexcept
	{
		for (value_t value = 1; value != 10; ++value)
		{
			const auto value_bf = valueToMask(value);

			// Check rows to eliminate candidates in respective columns
			for (index_t r1y = 0; r1y != 9; ++r1y)
			{
				auto candidates = getCandidatesInRow(value_bf, r1y);
				if (bitutil::getNumSetBits(candidates) == 2)
				{
					for (index_t r2y = 0; r2y != 9; ++r2y)
					{
						if (r1y != r2y
							&& candidates == getCandidatesInColumn(value_bf, r2y)
							)
						{
							bool changed = false;
							do
							{
								index_t x = bitutil::getLeastSignificantSetBit(candidates);
								changed |= eliminateCandidatesInColumn(value_bf, x, r1y, r2y);
							} while (bitutil::unsetLeastSignificantSetBit(candidates), candidates);
							if (changed)
							{
								return true;
							}
						}
					}
				}
			}

			// Check columns to eliminate candidates in respective rows
			for (index_t c1x = 0; c1x != 9; ++c1x)
			{
				auto candidates = getCandidatesInColumn(value_bf, c1x);
				if (bitutil::getNumSetBits(candidates) == 2)
				{
					for (index_t c2x = 0; c2x != 9; ++c2x)
					{
						if (c1x != c2x
							&& candidates == getCandidatesInColumn(value_bf, c2x)
							)
						{
							bool changed = false;
							do 
							{
								index_t y = bitutil::getLeastSignificantSetBit(candidates);
								changed |= eliminateCandidatesInRow(value_bf, y, c1x, c2x);
							} while (bitutil::unsetLeastSignificantSetBit(candidates), candidates);
							if (changed)
							{
								return true;
							}
						}
					}
				}
			}
		}
		return false;
	}

	void Sudoku::draw(RenderTarget& rt) const
	{
		for (index_t y = 0; y != 9; ++y)
		{
			for (index_t x = 0; x != 9; ++x)
			{
				getField(x, y).draw(rt, x * 15, y * 15);
			}
		}
	}
}
