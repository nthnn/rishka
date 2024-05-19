/* 
 * This file is part of the Rishka distribution (https://github.com/nthnn/rishka).
 * Copyright (c) 2024 Nathanne Isip.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file librishka/drivers/display.h
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Provides an interface for interacting with
 *        display hardware in the Rishka SDK.
 *
 * The `Display` class offers a set of static methods
 * to retrieve various properties of the display
 * hardware. This includes methods to get the dimensions
 * of the screen and viewport, as well as the number of
 * supported colors. By using this class, developers can
 * easily obtain necessary information about the display
 * to tailor their applications accordingly.
 */

#ifndef LIBRISHKA_DRIVERS_DISPLAY_H
#define LIBRISHKA_DRIVERS_DISPLAY_H

#include <librishka/types.h>

/**
 * @class Display
 * @brief Final class to interact with
 *        display hardware.
 *
 * The `Display` class contains only static
 * methods and cannot be instantiated. It
 * provides methods to get the screen and
 * viewport dimensions as well as the number
 * of supported colors. This class serves as
 * a utility for querying display properties,
 * allowing developers to adapt their applications 
 * based on the display characteristics.
 */
class Display final {
public:
    /**
     * @brief Get the height of the screen.
     * @return The height of the screen in pixels.
     *
     * This method retrieves the height of the
     * entire screen. The value returned is in
     * pixels and represents the total vertical
     * resolution of the display.
     */
    static i32 screen_height();

    /**
     * @brief Get the width of the screen.
     * @return The width of the screen in pixels.
     *
     * This method retrieves the width of the
     * entire screen. The value returned is
     * in pixels and represents the total
     * horizontal resolution of the display.
     */
    static i32 screen_width();

    /**
     * @brief Get the height of the viewport.
     * @return The height of the viewport in pixels.
     *
     * This method retrieves the height of the viewport,
     * which is the area of the screen currently visible
     * to the user. The value returned is in pixels and
     * represents the vertical resolution of the viewport.
     */
    static i32 viewport_height();

    /**
     * @brief Get the width of the viewport.
     * @return The width of the viewport in pixels.
     *
     * This method retrieves the width of the viewport,
     * which is the area of the screen currently visible
     * to the user. The value returned is in pixels and
     * represents the horizontal resolution of the viewport.
     */
    static i32 viewport_width();

    /**
     * @brief Get the number of supported colors.
     * @return The number of colors supported by
     *         the display.
     *
     * This method retrieves the number of colors
     * that the display hardware is capable of
     * showing. This is an important  characteristic
     * for applications that need to adapt to differentdisplay capabilities and provide appropriate 
     * visual elements.
     */
    static i32 supported_colors();
};

#endif