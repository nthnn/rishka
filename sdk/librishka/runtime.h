/* 
 * This file is part of the Rishka distribution (https://github.com/rishka-esp32/rishka-sdk).
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
 * @file runtime.h
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Provides utilities for managing runtime operations within Rishka applications.
 *
 * The Runtime class encapsulates functionality related to runtime management
 * and control flow within Rishka applications. It serves as a central hub for
 * coordinating runtime activities, such as task scheduling, event processing,
 * and resource management. Developers can leverage the Runtime class to
 * orchestrate the execution flow of their applications, ensuring efficient
 * utilization of system resources and optimal performance.
 */

#ifndef LIBRISHKA_RUNTIME_H
#define LIBRISHKA_RUNTIME_H

/**
 * @brief The Runtime class provides utilities for managing runtime operations.
 *
 * The Runtime class encapsulates functionality related to runtime management
 * and control flow within Rishka applications. It serves as a central hub for
 * coordinating runtime activities, such as task scheduling, event processing,
 * and resource management.
 */
class Runtime final {
public:
    /**
     * @brief Yield the execution context to other tasks or threads.
     *
     * The yield() function allows cooperative multitasking by yielding the
     * execution context to other tasks or threads within the application. By
     * invoking this function, the currently executing task relinquishes control
     * of the CPU, allowing other tasks or threads to execute. This method is
     * useful for implementing cooperative multitasking or event-driven
     * programming paradigms, where tasks voluntarily yield control to other
     * tasks or respond to external events.
     */
    static void yield();
};

#endif /* LIBRISHKA_RUNTIME_H */