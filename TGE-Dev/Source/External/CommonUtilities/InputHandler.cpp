#include "InputHandler.h"

namespace CommonUtilities
{
	InputHandler::InputHandler() : myCurrentState(), myIntermediateState(), myPreviousState()/*, myInputState()*/
	{
		myMouseDelta = { 0, 0 };
		myPendingMouseDelta = { 0, 0 };
		//myPreviousMouseScreenPosition = { 0, 0 };
		myMouseClientPosition = { 0, 0 };
		myScrollDeltaDistance = 0;
		myShouldLockcursorToClient = false;
		myToggleLockCursorToClientButton = static_cast<int>(Keys::RSHIFT);
	}

	bool InputHandler::UpdateEvents(const UINT& aMessage, const WPARAM& aWParam, const LPARAM& anLParam, const HWND& aWindowHandle)
	{
		bool returnValue = true;
		switch (aMessage)
		{
		case WM_KEYDOWN:
			myCurrentState[aWParam] = true;
			if (aWParam == myToggleLockCursorToClientButton)
			{
				myShouldLockcursorToClient = !myShouldLockcursorToClient;
			}
			break;
		case WM_KEYUP:
			myCurrentState[aWParam] = false;
			break;
			//MUS------------------------------------------------------------------
		case WM_LBUTTONDOWN:
			myCurrentState[static_cast<int>(Keys::MOUSELBUTTON)] = true;
			break;
		case WM_LBUTTONUP:
			myCurrentState[static_cast<int>(Keys::MOUSELBUTTON)] = false;
			break;
		case WM_RBUTTONDOWN:
			myCurrentState[static_cast<int>(Keys::MOUSERBUTTON)] = true;
			break;
		case WM_RBUTTONUP:
			myCurrentState[static_cast<int>(Keys::MOUSERBUTTON)] = false;
			//SetMouseClientPosition(aWindowHandle, { 0, 0 });
			break;
		case WM_MBUTTONDOWN:
			myCurrentState[static_cast<int>(Keys::MBUTTON)] = true;
			break;
		case WM_MBUTTONUP:
			myCurrentState[static_cast<int>(Keys::MBUTTON)] = false;
			break;
		case WM_MOUSEWHEEL:
		{
			myScrollDeltaDistance = GET_WHEEL_DELTA_WPARAM(aWParam);
		}
		break;
		case WM_MOUSEMOVE:
		{
			LockCursorToClient(aWindowHandle);
			POINT mousePos = POINT({ GET_X_LPARAM(anLParam), GET_Y_LPARAM(anLParam) });
			POINT mouseDelta = { mousePos.x - myMouseClientPosition.x, mousePos.y - myMouseClientPosition.y };
			myPendingMouseDelta.x += mouseDelta.x;
			myPendingMouseDelta.y += mouseDelta.y;
			myMouseClientPosition = mousePos;
		}
		break;
		default:
			returnValue = false;
			break;
		}
		return returnValue;
	}

	bool InputHandler::IsKeyDown(const int aKeyCode) const
	{
		if (myCurrentState[aKeyCode] && myPreviousState[aKeyCode] == false)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool InputHandler::IsKeyPressed(const int aKeyCode) const
	{
		if (myCurrentState[aKeyCode] && myPreviousState[aKeyCode])
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool InputHandler::IsKeyUp(const int aKeyCode) const
	{
		if (myCurrentState[aKeyCode] == false && myPreviousState[aKeyCode] == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void InputHandler::UpdatePreviousInput(/*const bool anOnlyCheckButtonUps*/)
	{
		for (int index = 0; index < 256; index++)
		{
			myPreviousState[index] = myCurrentState[index];
		}
		myScrollDeltaDistance = 0;
		myMouseDelta = myPendingMouseDelta;
		myPendingMouseDelta = { 0, 0 };
		//myPreviousMouseScreenPosition = GetMouseScreenPosition();
	}

	void InputHandler::SetMouseClientPosition(const HWND& aWindowHandle, const POINT& aPoint)
	{
		POINT clientPoint = aPoint;
		ClientToScreen(aWindowHandle, &clientPoint);
		SetCursorPos(clientPoint.x, clientPoint.y);
	}

	void InputHandler::LockCursorToClient(const HWND& aWindowHandle)
	{
		if (myShouldLockcursorToClient)
		{
			RECT rectangle;
			GetClientRect(aWindowHandle, &rectangle);
			MapWindowPoints(aWindowHandle, nullptr, (LPPOINT)(&rectangle), 2);
			ClipCursor(&rectangle);
		}
		else
		{
			ClipCursor(nullptr);
		}
	}

	POINT InputHandler::GetMouseScreenPosition() const
	{
		POINT mousePos;
		GetCursorPos(&mousePos);
		return mousePos;
	}

	POINT InputHandler::GetMouseClientPosition() const
	{
		return myMouseClientPosition;
	}

	POINT InputHandler::GetDeltaMouseMove() const
	{
		//POINT mousePos = GetMouseScreenPosition();
		return myMouseDelta;
		//return { mousePos.x - myPreviousMouseScreenPosition.x, mousePos.y - myPreviousMouseScreenPosition.y };
	}
	int InputHandler::GetDeltaMouseScroll() const
	{
		return myScrollDeltaDistance;
	}
}