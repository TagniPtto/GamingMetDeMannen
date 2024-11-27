#pragma once
#include "utils.h"

enum class Input {
	EMPTY = 0,
	ARROW_UP = 1,
	ARROW_LEFT = 2,
	ARROW_DOWN = 4,
	ARROW_RIGHT = 8,
	ARROW_UP_LEFT = 3,
	ARROW_DOWN_LEFT = 6,
	ARROW_UP_RIGHT = 9,
	ARROW_DOWN_RIGHT = 12,
	C = 16,
	X = 32,
};
class InputBuffer
{
private:
	unsigned int m_pointerHead{ 0 };
	const unsigned int m_InputBufferLength;
	Input* m_inputBuffer;

	float m_elapsedTime{ 0 };
	float m_InputWindowDuration{ 0.1f };
	bool gotInputPrevF{ false };
public:
	unsigned int GetSize()const;
	InputBuffer(unsigned int size);
	Input operator[](int i);

	void Update(float elapsedSec);

	//const Input (& GetInput())[];
	void Print()const;

};

