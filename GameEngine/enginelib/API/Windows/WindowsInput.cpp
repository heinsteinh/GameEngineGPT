/************************************************************************************/ /*!
  \file           Input.cpp
  \project        INSERT PROJECT NAME
  \author         Chua Teck Lee, c.tecklee, 390008420
  \par            email: c.tecklee\@digipen.edu
  \date           May 15, 2021
  \brief          Describes a Windows(Platform) specific input that ements
                  the generic Input interface.
                  Currently using SDL as the backend abstraction.
  Copyright (C) 2021 DigiPen Institute of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
  */
                                                                                       /*************************************************************************************/

#include "Core/Application.h"
#include "Core/InputCore.h"
#include "ftspch.h"

#include <SDL.h>

namespace fts
{
    // Windows Platform currently using SDL
    // SDL Specific Input Requirements
    int m_keyLength;
    const Uint8* m_keyboardState;
    Uint8* m_prevKeyboardState;

    Uint32 m_prevMouseState;
    Uint32 m_mouseState;

    int m_mouseXPos;
    int m_mouseYPos;

    void InputCore::Init()
    {
        m_prevMouseState = m_mouseState = m_mouseXPos = m_mouseYPos = 0;

        m_keyboardState     = SDL_GetKeyboardState(&m_keyLength);
        m_prevKeyboardState = new Uint8[m_keyLength];
        memcpy(m_prevKeyboardState, m_keyboardState, m_keyLength);
    }

    void InputCore::Update()
    {
        memcpy(m_prevKeyboardState, m_keyboardState, m_keyLength);
        m_prevMouseState = m_mouseState;

        m_mouseState = SDL_GetMouseState(&m_mouseXPos, &m_mouseYPos);
    }

    void InputCore::ShutDown()
    {
        delete[] m_prevKeyboardState;
        m_prevKeyboardState = nullptr;
    }

    bool InputCore::IsKeyDown(const KeyCode keycode)
    {
        return m_keyboardState[static_cast<int>(keycode)];
    }

    bool InputCore::IsKeyPressed(const KeyCode keycode)
    {
        return !m_prevKeyboardState[static_cast<int>(keycode)] && m_keyboardState[static_cast<int>(keycode)];
    }

    bool InputCore::IsKeyReleased(const KeyCode keycode)
    {
        return m_prevKeyboardState[static_cast<int>(keycode)] && !m_keyboardState[static_cast<int>(keycode)];
    }

    bool InputCore::IsAnyKeyDown()
    {
        for(KeyCode keycode{0}; keycode < m_keyLength; ++keycode)
        {
            if(IsKeyDown(keycode))
                return true;
        }

        return false;
    }

    bool InputCore::IsAnyKeyPressed()
    {
        for(KeyCode keycode{0}; keycode < m_keyLength; ++keycode)
        {
            if(IsKeyPressed(keycode))
                return true;
        }

        return false;
    }

    bool InputCore::IsAnyKeyReleased()
    {
        for(KeyCode keycode{0}; keycode < m_keyLength; ++keycode)
        {
            if(IsKeyReleased(keycode))
                return true;
        }

        return false;
    }

    std::vector<KeyCode> InputCore::GetKeysDown()
    {
        std::vector<KeyCode> keys;

        for(KeyCode keycode{0}; keycode < m_keyLength; ++keycode)
        {
            if(IsKeyDown(keycode))
                keys.emplace_back(keycode);
        }

        return keys;
    }

    std::vector<KeyCode> InputCore::GetKeysPressed()
    {
        std::vector<KeyCode> keys;

        for(KeyCode keycode{0}; keycode < m_keyLength; ++keycode)
        {
            if(IsKeyPressed(keycode))
                keys.emplace_back(keycode);
        }

        return keys;
    }

    std::vector<KeyCode> InputCore::GetKeysReleased()
    {
        std::vector<KeyCode> keys;

        for(KeyCode keycode{0}; keycode < m_keyLength; ++keycode)
        {
            if(IsKeyReleased(keycode))
                keys.emplace_back(keycode);
        }

        return keys;
    }

    bool InputCore::IsMouseButtonDown(const MouseCode button)
    {
        Uint32 mask = 0;

        switch(button)
        {
        case Mouse::BUTTON_LEFT:
            mask = SDL_BUTTON_LMASK;
            break;

        case Mouse::BUTTON_RIGHT:
            mask = SDL_BUTTON_RMASK;
            break;

        case Mouse::BUTTON_MIDDLE:
            mask = SDL_BUTTON_MMASK;
            break;

        case Mouse::BUTTON_X1:
            mask = SDL_BUTTON_X1MASK;
            break;

        case Mouse::BUTTON_X2:
            mask = SDL_BUTTON_X2MASK;
            break;
        }

        return (m_mouseState & mask);
    }

    bool InputCore::IsMouseButtonPressed(const MouseCode button)
    {
        Uint32 mask = 0;

        switch(button)
        {
        case Mouse::BUTTON_LEFT:
            mask = SDL_BUTTON_LMASK;
            break;

        case Mouse::BUTTON_RIGHT:
            mask = SDL_BUTTON_RMASK;
            break;

        case Mouse::BUTTON_MIDDLE:
            mask = SDL_BUTTON_MMASK;
            break;

        case Mouse::BUTTON_X1:
            mask = SDL_BUTTON_X1MASK;
            break;

        case Mouse::BUTTON_X2:
            mask = SDL_BUTTON_X2MASK;
            break;
        }

        return !(m_prevMouseState & mask) && (m_mouseState & mask);
    }

    bool InputCore::IsMouseButtonReleased(const MouseCode button)
    {
        Uint32 mask = 0;

        switch(button)
        {
        case Mouse::BUTTON_LEFT:
            mask = SDL_BUTTON_LMASK;
            break;

        case Mouse::BUTTON_RIGHT:
            mask = SDL_BUTTON_RMASK;
            break;

        case Mouse::BUTTON_MIDDLE:
            mask = SDL_BUTTON_MMASK;
            break;

        case Mouse::BUTTON_X1:
            mask = SDL_BUTTON_X1MASK;
            break;

        case Mouse::BUTTON_X2:
            mask = SDL_BUTTON_X2MASK;
            break;
        }

        return (m_prevMouseState & mask) && !(m_mouseState & mask);
    }

    bool InputCore::IsAnyMouseButtonDown()
    {
        for(MouseCode mousecode{0}; mousecode <= Mouse::BUTTON_LAST; ++mousecode)
        {
            if(IsMouseButtonDown(mousecode))
                return true;
        }

        return false;
    }

    bool InputCore::IsAnyMouseButtonPressed()
    {

        for(MouseCode mousecode{0}; mousecode <= Mouse::BUTTON_LAST; ++mousecode)
        {
            if(IsMouseButtonPressed(mousecode))
                return true;
        }

        return false;
    }

    bool InputCore::IsAnyMouseButtonReleased()
    {

        for(MouseCode mousecode{0}; mousecode <= Mouse::BUTTON_LAST; ++mousecode)
        {
            if(IsMouseButtonReleased(mousecode))
                return true;
        }

        return false;
    }

    std::vector<MouseCode> InputCore::GetMouseButtonsDown()
    {
        std::vector<MouseCode> mouseButtons;

        for(MouseCode mousecode{0}; mousecode <= Mouse::BUTTON_LAST; ++mousecode)
        {
            if(IsMouseButtonDown(mousecode))
                mouseButtons.emplace_back(mousecode);
        }

        return mouseButtons;
    }

    std::vector<MouseCode> InputCore::GetMouseButtonsPressed()
    {
        std::vector<MouseCode> mouseButtons;

        for(MouseCode mousecode{0}; mousecode <= Mouse::BUTTON_LAST; ++mousecode)
        {
            if(IsMouseButtonPressed(mousecode))
                mouseButtons.emplace_back(mousecode);
        }

        return mouseButtons;
    }

    std::vector<MouseCode> InputCore::GetMouseButtonsReleased()
    {
        std::vector<MouseCode> mouseButtons;

        for(MouseCode mousecode{0}; mousecode <= Mouse::BUTTON_LAST; ++mousecode)
        {
            if(IsMouseButtonReleased(mousecode))
                mouseButtons.emplace_back(mousecode);
        }

        return mouseButtons;
    }

    std::pair<int, int> InputCore::GetMousePosition()
    {
        int x, y;
        SDL_GetMouseState(&x, &y);

        return {x, y};
    }

    int InputCore::GetMouseX()
    {
        return GetMousePosition().first;
    }

    int InputCore::GetMouseY()
    {
        return GetMousePosition().second;
    }
} // namespace fts
