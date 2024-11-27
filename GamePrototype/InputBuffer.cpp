#include "pch.h"
#include "InputBuffer.h"
#include "iostream"

unsigned int InputBuffer::GetSize() const
{
	return m_InputBufferLength;
}

InputBuffer::InputBuffer(unsigned int size) :
	m_InputBufferLength(size), m_inputBuffer(new Input[size]{})//m_inputBuffer(((Input*)malloc(m_InputBufferLength * sizeof(Input))) )
{
}

void InputBuffer::Update(float elapsedSec)
{


	const Uint8* keyboard_state_array = SDL_GetKeyboardState(NULL);


	bool gettingInputThisF{ false };
	int x{ 0 };
	int y{ 0 };
	if (keyboard_state_array[SDL_SCANCODE_UP] or keyboard_state_array[SDL_SCANCODE_W]) {
		y = (int)Input::ARROW_UP;
		gettingInputThisF = true;
	}
	if (keyboard_state_array[SDL_SCANCODE_LEFT] or keyboard_state_array[SDL_SCANCODE_A]) {
		x = (int)Input::ARROW_LEFT;
		gettingInputThisF = true;
	}
	if (keyboard_state_array[SDL_SCANCODE_DOWN] or keyboard_state_array[SDL_SCANCODE_S]) {
		y = (int)Input::ARROW_DOWN;
		gettingInputThisF = true;
	}
	if (keyboard_state_array[SDL_SCANCODE_RIGHT] or keyboard_state_array[SDL_SCANCODE_D]) {
		x = (int)Input::ARROW_RIGHT;
		gettingInputThisF = true;
	}



	//If input changed add it
	if (x + y != 0) {
		if (m_inputBuffer[m_pointerHead] != (Input)(x + y)) {
			m_pointerHead = (m_pointerHead + 1) % m_InputBufferLength;
			m_inputBuffer[m_pointerHead] = (Input)(x + y);
		}
	}

	//m_pointerTail = (m_pointerTail + 1) % m_BufferWindowLength;
	int z{ 0 };
	if (keyboard_state_array[SDL_SCANCODE_X]) {
		z = (int)Input::X;
		gettingInputThisF = true;
	}
	if (keyboard_state_array[SDL_SCANCODE_C]) {
		z = (int)Input::C;
		gettingInputThisF = true;
	}
	if (z != 0) {
		if (m_inputBuffer[m_pointerHead] != (Input)(z)) {
			m_pointerHead = (m_pointerHead + 1) % m_InputBufferLength;
			m_inputBuffer[m_pointerHead] = (Input)(z);
		}
	}




	if (!gettingInputThisF) {
		m_elapsedTime += elapsedSec;
		if (gotInputPrevF) {
			m_pointerHead = (m_pointerHead + 1) % m_InputBufferLength;
			m_inputBuffer[m_pointerHead] = (Input)(0);
		}
		else
			if (m_elapsedTime > m_InputWindowDuration) {
				m_elapsedTime -= m_InputWindowDuration;
				m_pointerHead = (m_pointerHead + 1) % m_InputBufferLength;
				m_inputBuffer[m_pointerHead] = (Input)(0);
			}
	}
	else {
		m_elapsedTime = 0;
	}
	gotInputPrevF = gettingInputThisF;

	//std::cout << " head Pointer" << m_pointerHead << std::endl;
}

void InputBuffer::Print() const
{
	//std::cout << " input " << (int)m_inputBuffer[m_pointerHead] << std::endl;
	for (int i{}; i < m_InputBufferLength; ++i) {
		switch (m_inputBuffer[(m_pointerHead + 1 + i) % m_InputBufferLength]) {

		case(Input::EMPTY):
			std::cout << " EMPTY ,";
			break;
		case(Input::ARROW_UP):
			std::cout << " ARROW_UP ,";
			break;
		case(Input::ARROW_DOWN):
			std::cout << " ARROW_DOWN ,";
			break;
		case(Input::ARROW_LEFT):
			std::cout << " ARROW_LEFT ,";
			break;
		case(Input::ARROW_RIGHT):
			std::cout << " ARROW_RIGHT ,";
			break;

		case(Input::ARROW_DOWN_LEFT):
			std::cout << " ARROW_DOWN_LEFT ,";
			break;
		case(Input::ARROW_DOWN_RIGHT):
			std::cout << " ARROW_DOWN_RIGHT ,";
			break;
		case(Input::ARROW_UP_LEFT):
			std::cout << " ARROW_UP_LEFT ,";
			break;
		case(Input::ARROW_UP_RIGHT):
			std::cout << " ARROW_UP_RIGHT ,";
			break;
		case(Input::C):
			std::cout << " C ,";
			break;
		case(Input::X):
			std::cout << " X ,";
			break;
		}
		std::cout << std::endl;
	}
}

Input InputBuffer::operator[](int i)
{
	return m_inputBuffer[(m_pointerHead + i) % m_InputBufferLength];
}
