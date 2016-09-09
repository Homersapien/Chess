#pragma once

struct SquarePosition
{
	// Initialization
	SquarePosition() : row(0), col(0) {};
	SquarePosition(int row, int col) : row(row), col(col) {};

	// Fields
	int row;
	int col;

	// Operators
	SquarePosition& operator+=(const SquarePosition& right)
	{
		row += right.row;
		col += right.col;
		return *this;
	}

	const SquarePosition operator+(const SquarePosition& other) const
	{
		return SquarePosition(*this) += other;
	}
};

inline bool operator==(const SquarePosition& left, const SquarePosition& right)
{
	return (left.row == right.row && left.col == right.col);
}

inline bool operator!=(const SquarePosition& left, const SquarePosition& right)
{
	return !(left == right);
}