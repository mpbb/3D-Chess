#include "chess.h"
#include <iostream>

using namespace Chess;

Piece::Piece(Color color, Type type)
{
	this->color = color;
	this->type = type;
}
Color Piece::getColor()
{
	return this->color;
}
Type Piece::getType()
{
	return this->type;
}

Board::Board()
{

}

bool Board::move(int srcX, int srcY, int dstX, int dstY)
{

	if (pieces[srcY][srcX]->getType() == Type::NONE || pieces[dstY][dstX]->getType() != Type::NONE)
	{
		return false;
	}
	else
	{
		Board::forceMove(srcX, srcY, dstX, dstY);
		return true;
	}
}

void Board::forceMove(int srcX, int srcY, int dstX, int dstY)
{
	delete pieces[dstY][dstX];
	pieces[dstY][dstX] = pieces[srcY][srcX];
	pieces[srcY][srcX] = new Piece(Color::NONE, Type::NONE);
}

Piece* Board::getPieceAtPos(int x, int y)
{
	return this->pieces[y][x];
}