#pragma once

#include <string>

namespace Chess
{

	enum class Type : char { NONE, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };
	enum class Color : char { NONE, WHITE, BLACK };

	class Piece
	{
	public:
		Piece(Color color, Type type);
		Color getColor();
		Type getType();
	protected:
		Color color;
		Type type;
	};

	class Board
	{
	public:
		Board();
		bool move(int srcX, int srcY, int dstX, int dstY);
		Piece* getPieceAtPos(int x, int y);
	private:
		void forceMove(int srcX, int srcY, int dstX, int dstY);

		Piece* pieces[8][8] =
		{
			{new Piece(Color::BLACK, Type::ROOK), new Piece(Color::BLACK, Type::KNIGHT), new Piece(Color::BLACK, Type::BISHOP), new Piece(Color::BLACK, Type::QUEEN), new Piece(Color::BLACK, Type::KING), new Piece(Color::BLACK, Type::BISHOP), new Piece(Color::BLACK, Type::KNIGHT), new Piece(Color::BLACK, Type::ROOK)},
			{new Piece(Color::BLACK, Type::PAWN), new Piece(Color::BLACK, Type::PAWN), new Piece(Color::BLACK, Type::PAWN), new Piece(Color::BLACK, Type::PAWN), new Piece(Color::BLACK, Type::PAWN), new Piece(Color::BLACK, Type::PAWN), new Piece(Color::BLACK, Type::PAWN), new Piece(Color::BLACK, Type::PAWN)},
			{new Piece(Color::NONE, Type::NONE), new Piece(Color::NONE, Type::NONE), new Piece(Color::NONE, Type::NONE), new Piece(Color::NONE, Type::NONE), new Piece(Color::NONE, Type::NONE), new Piece(Color::NONE, Type::NONE), new Piece(Color::NONE, Type::NONE), new Piece(Color::NONE, Type::NONE)},
			{new Piece(Color::NONE, Type::NONE), new Piece(Color::NONE, Type::NONE), new Piece(Color::NONE, Type::NONE), new Piece(Color::NONE, Type::NONE), new Piece(Color::NONE, Type::NONE), new Piece(Color::NONE, Type::NONE), new Piece(Color::NONE, Type::NONE), new Piece(Color::NONE, Type::NONE)},
			{new Piece(Color::NONE, Type::NONE), new Piece(Color::NONE, Type::NONE), new Piece(Color::NONE, Type::NONE), new Piece(Color::NONE, Type::NONE), new Piece(Color::NONE, Type::NONE), new Piece(Color::NONE, Type::NONE), new Piece(Color::NONE, Type::NONE), new Piece(Color::NONE, Type::NONE)},
			{new Piece(Color::NONE, Type::NONE), new Piece(Color::NONE, Type::NONE), new Piece(Color::NONE, Type::NONE), new Piece(Color::NONE, Type::NONE), new Piece(Color::NONE, Type::NONE), new Piece(Color::NONE, Type::NONE), new Piece(Color::NONE, Type::NONE), new Piece(Color::NONE, Type::NONE)},
			{new Piece(Color::WHITE, Type::PAWN), new Piece(Color::WHITE, Type::PAWN), new Piece(Color::WHITE, Type::PAWN), new Piece(Color::WHITE, Type::PAWN), new Piece(Color::WHITE, Type::PAWN), new Piece(Color::WHITE, Type::PAWN), new Piece(Color::WHITE, Type::PAWN), new Piece(Color::WHITE, Type::PAWN)},
			{new Piece(Color::WHITE, Type::ROOK), new Piece(Color::WHITE, Type::KNIGHT), new Piece(Color::WHITE, Type::BISHOP), new Piece(Color::WHITE, Type::QUEEN), new Piece(Color::WHITE, Type::KING), new Piece(Color::WHITE, Type::BISHOP), new Piece(Color::WHITE, Type::KNIGHT), new Piece(Color::WHITE, Type::ROOK)}
		};
	};
}