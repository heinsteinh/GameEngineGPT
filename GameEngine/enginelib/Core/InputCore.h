#ifndef INPUTCORE_H
#define INPUTCORE_H

#pragma once

#include <Core/Base.h>
#include <Core/KeyCodes.h>
#include <Core/MouseCodes.h>
#include <glm/glm.hpp>

namespace fts
{
    // forward declaration
    class Application;

    /****************************************************************************/ /*!
       @brief     Interface of the Engine's Input system that will be used by
                  by the client.
      */
                                                                                   /*****************************************************************************/
    class InputCore
    {
    public:
        /*-----------------------------------------------------------------------------*/
        /* Interface Functions                                                         */
        /*-----------------------------------------------------------------------------*/
        /****************************************************************************/ /*!
           @brief     Determine if a key by the given keycode is currently held down
           @note      This function will return true even when keypressed is true too.
           @return    Whether the key is currently down.
          */
                                                                                       /*****************************************************************************/
        static bool IsKeyDown(KeyCode keycode);
        /****************************************************************************/ /*!
           @brief     Determine if a key by the given keycode has been triggered within
                      this frame.
           @return    Whether the key is triggered this frame .
          */
                                                                                       /*****************************************************************************/
        static bool IsKeyPressed(KeyCode keycode);
        /****************************************************************************/ /*!
           @brief     Determine if a key by the given keycode has been triggered within
                      this frame.

            @note      KeyRelease returns true once when a key that was pressed is released.
                       To check if a key is not pressed all the time
                       use !(IsKeyDown(keycode)) instead.
            @return    Whether the key is Released this frame.
           */
        /*****************************************************************************/
        static bool IsKeyReleased(KeyCode keycode);

        static bool IsAnyKeyDown();
        static bool IsAnyKeyPressed();
        static bool IsAnyKeyReleased();

        static std::vector<KeyCode> GetKeysDown();
        static std::vector<KeyCode> GetKeysPressed();
        static std::vector<KeyCode> GetKeysReleased();

        /****************************************************************************/ /*!
           @brief     Determine if a mouse button by the given mouse code is
                      currently held down
           @note      This function will return true even when mousepressed is true too.
           @return    Whether the mouse button is currently down.
          */
                                                                                       /*****************************************************************************/
        static bool IsMouseButtonDown(MouseCode button);
        /****************************************************************************/ /*!
           @brief     Determine if a mouse button by the given mouse code is
                      has been triggered this frame.
           @return    Whether the mouse button is being clicked this frame.
          */
                                                                                       /*****************************************************************************/
        static bool IsMouseButtonPressed(MouseCode button);
        /****************************************************************************/ /*!
           @brief     Determine if a mouse button by the given mouse code is
                      has been released this frame.
           @return    Whether the mouse button is being released this frame.
          */
                                                                                       /*****************************************************************************/
        static bool IsMouseButtonReleased(MouseCode button);

        static bool IsAnyMouseButtonDown();
        static bool IsAnyMouseButtonPressed();
        static bool IsAnyMouseButtonReleased();

        static std::vector<MouseCode> GetMouseButtonsDown();
        static std::vector<MouseCode> GetMouseButtonsPressed();
        static std::vector<MouseCode> GetMouseButtonsReleased();

        /****************************************************************************/ /*!
           @brief     Retrieve the current mouse position in screen coordinates
           @return    a pair containing the x and y position of the mouse
          */
                                                                                       /*****************************************************************************/
        static std::pair<int, int> GetMousePosition();
        /****************************************************************************/ /*!
           @brief     Retrieve the current X-axis mouse position in screen coordinates
           @return    X-axis position of where the mouse current is at
          */
                                                                                       /*****************************************************************************/
        static int GetMouseX();
        /****************************************************************************/ /*!
           @brief     Retrieve the current Y-axis mouse position in screen coordinates
           @return    Y-axis position of where the mouse current is at
          */
                                                                                       /*****************************************************************************/
        static int GetMouseY();

    private:
        InputCore()  = delete;
        ~InputCore() = delete;

        static void Init();
        static void Update();
        static void ShutDown();

        friend class Application;
    };
} // namespace fts

#endif // INPUTCORE_H
