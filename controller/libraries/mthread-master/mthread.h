// -*- mode: c++ -*-

/// \file mthread.h
/// \author Jonathan Lamothe

// Arduino Multi-Threading Library (mthread)

// Copyright (C) 2010-2012 Jonathan Lamothe <jonathan@jlamothe.net>

// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation, either version 3 of
// the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this program.  If not, see:
// http://www.gnu.org/licenses/

#ifndef MTHREAD_H
#define MTHREAD_H

//
// INCLUDES
//

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "../newdel/newdel.h"
#endif

/// \brief Default switch debounce time.
#define DEFAULT_DEBOUNCE 50

class ThreadList;
void loop(void);

//
// CLASSES
//

/// \brief Provides thread functionality.
class Thread
{
public:

    /// \brief Various running modes for a Thread.
    enum Mode
    {

        /// \brief Thread is running.
        run_mode,

        /// \brief Thread is paused.
        pause_mode,

        /// \brief Thread is sleeping (for seconds).
        sleep_mode,

        /// \brief Thread is sleeping (for milliseconds).
        sleep_milli_mode,

        /// \brief Thread is sleeping (for microseconds).
        sleep_micro_mode,

        /// \brief Thread is to be killed on next call.
        kill_mode

    };

    /// \brief Constructor.
    Thread();

    /// \brief Destructor.
    virtual ~Thread();

    /// \brief Returns the running mode for the Thread.
    /// \return The running mode.
    Mode get_mode() const;

    /// \brief Kills a Thread.
    /// \param force If true, the Thread will be killed immediately on
    /// the next call without running any more loops, if false, the
    /// Thread will have to opportunity to terminate cleanly but will
    /// be resumed if sleeping or paused.
    /// \note If the force parameter is set to false, the Thread could
    /// possibly ignore or cancel the request, however this is still
    /// the preferred way of calling the kill() function.
    /// \return true on success, false on failure.
    bool kill(bool force = false);

    /// \brief Pauses a Thread.  This function will cause a Thread to
    /// pause until its resume() function is called.  This function
    /// will cancel any sleep timer currently in effect.
    /// \return true on success, false on failure.
    bool pause();

    /// \brief Resumes a paused or sleeping Thread.
    /// \return true on success, false on failure.
    bool resume();

    /// \brief Puts the Thread to sleep for a certain number of
    /// seconds.  If already running, the thread's loop() function
    /// will be allowed to finish, but will not be called again until
    /// the timeout has expired, or the resume() or kill() function
    /// has been called.
    /// \param t The number of seconds the Thread is to sleep for.
    /// \return true on success, false on failure.
    bool sleep(unsigned long t);

    /// \brief Puts the Thread to sleep for a certain number of
    /// microseconds.  If already running, the thread's loop()
    /// function will be allowed to finish, but will not be called
    /// again until the timeout has expired, or the resume() or kill()
    /// function has been called.
    /// \param t The number of microseconds the Thread is to sleep for
    /// \return true on success, false on failure.
    bool sleep_micro(unsigned long t);

    /// \brief Puts the Thread to sleep for a certain number of
    /// milliseconds.  If already running, the thread's loop()
    /// function will be allowed to finish, but will not be called
    /// again until the timeout has expired, or the resume() or kill()
    /// function has been called.
    /// \param t The number of milliseconds the Thread is to sleep
    /// for.
    /// \return true on success, false on failure.
    bool sleep_milli(unsigned long t);

protected:

    /// \brief The Thread's main loop.  This function is to be
    /// overridden.  It takes the place of the loop function found in
    /// most Arduino programs.  A single loop() should run as quickly
    /// as possible, as it will hold up other Thread objects while it
    /// is executing.
    /// \note At the beginning of each loop, the function should check
    /// the kill_flag.
    /// \return true if the loop needs to be called again, false if
    /// the Thread has completed executing (at which point it will be
    /// destroyed).
    virtual bool loop();

    /// \brief Kill flag.  This variable should be checked at the
    /// beginning of every loop() function.  If it is true, the Thread
    /// has been requested to be killed, and the loop() function
    /// should behave accordingly.  The request can be denied by
    /// resetting it to false.
    bool kill_flag;

private:

    /// \brief Determines if the function is active and runs through a
    /// loop if appropriate.
    /// This function is called automatically by a ThreadList.
    /// \return true if the Thread needs to be called again, false if
    /// the Thread has completed execution.
    /// \note It is important to note that once a Thread has completed
    /// its execution it will automatically destroy itself and MUST
    /// NOT be used again.  A new instance must first be created.
    bool call();

    /// \brief The time the thread was stopped at.
    unsigned long stop_time;

    /// \brief The amount of the the thread is to wait for.
    unsigned long wait_time;

    /// \brief The thread's running mode (can be read through the
    /// get_mode() function).
    Mode mode;

    friend class ThreadList;
    friend void loop(void);
};

/// \brief An object for running several Thread objects
/// simultaneously.  A ThreadList object is a Thread in and of itself.
/// This allows the creation of tiered ThreadList objects by placing a
/// lower-priority ThreadList inside of a higher-priority ThreadList.
/// \note DO NOT place a Thread in more than one ThreadList or more
/// than once in a single ThreadList. DO NOT place a ThreadList inside
/// of itself or one of its children.  Also, DO NOT place the @link
/// mthread_globals::main_thread_list main_thread_list @endlink in
/// another ThreadList.  These WILL cause memory corruption (and are
/// silly things to do in the first place).
class ThreadList : public Thread
{
public:

    /// \brief Constructor.

    /// \param keep If true, the ThreadList will continue to run even
    /// after it's empty, otherwise it will automatically destroy
    /// itself once all of its Thread objects have finished.
    ThreadList(bool keep = false);

    /// \brief Destructor.
    ~ThreadList();

    /// \brief Adds a Thread to the ThreadList.
    /// \param t A pointer to the Thread to be added.
    /// \return true on success, false on failure.
    bool add_thread(Thread *t);

protected:

    /// \brief The main loop.
    /// \see Thread::loop().
    bool loop();

private:

    /// \brief An array of pointers to the Thread objects in the list.
    Thread **thread;

    /// \brief The number of Thread objects in the list.
    unsigned thread_count;

    /// \brief The index number of the active thread.
    unsigned thread_index;

    /// \brief If true, the ThreadList will not destroy itself when it
    /// becomes empty.
    bool keep_flag;

};

/// \brief A pointer to the main ThreadList.
/// This object will be run in place of the loop function expected in
/// most Arduino programs.
extern ThreadList *main_thread_list;

#endif  // MTHREAD_H

// jl
