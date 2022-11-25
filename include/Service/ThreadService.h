/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "ThreadStatus.h"
#include "Service/SignalService.h"
#include "Service/TimeService.h"
#include "Service/SignalServiceTs.h"
#include "Input/InputSignals.h"

// C++ Standard Library includes
#include <functional>
#include <vector>
#include <map>
#include <thread>
#include <mutex>

namespace astu {

    /**
     * This signal represents progress reports from threads.
     *
     * @ingroup srv_group
     */
    class ThreadProgressSignal {
    public:

        /**
         * Constructor.
         *
         * @param threadId  the numerical identifier of the thread
         */
        ThreadProgressSignal(int threadId, double progress) : threadId(threadId), progress(progress) {}

        /** The numerical identifier of the thread. */
        int threadId;

        /** The progress in percent. */
        double progress;
    };


    /**
     * Type definition for signal services that transmit thread progress report signals.
     *
     * @ingroup input_group
     */
    using ThreadProgressSignalService = SignalServiceTs<ThreadProgressSignal>;


    /**
     * Type definition for signal listeners which receive thread progress report signals.
     *
     * @ingroup srv_group
     */
    using IThreadProgressListener = ISignalListener<ThreadProgressSignal>;


    /**
     * This signal represents status updates from threads.
     *
     * Note: When this signal arrives, it is not guaranteed that the retrieval of the information about the thread
     * will be successful. By the time the signal arrives, the information about the thread may have already been
     * cleared because the thread was terminated and the end of the sweeping cycle was completed at the same time.
     *
     * @ingroup srv_group
     */
    class ThreadStatusSignal {
    public:

        /**
         * Constructor.
         *
         * @param threadId  the numerical identifier of the thread
         */
        ThreadStatusSignal(int threadId, ThreadStatus status) : threadId(threadId), status(status) {}

        /** The numerical identifier of the thread. */
        int threadId;

        /** The status of the thread. */
        ThreadStatus status;
    };

    /**
     * Type definition for signal services that transmit thread status signals.
     *
     * @ingroup input_group
     */
    using ThreadStatusSignalService = SignalServiceTs<ThreadStatusSignal>;


    /**
     * Type definition for signal listeners which receive thread status signals.
     *
     * @ingroup srv_group
     */
    using IThreadStatusListener = ISignalListener<ThreadStatusSignal>;


    /**
     * Services can derive from this class to process thread status button signals.
     *
     * @ingroup srv_group
     */
    class ThreadStatusListener
        : virtual public Service
        , private IThreadStatusListener
    {
    public:

        /**
         * Constructor.
         */
        ThreadStatusListener() {
            AddStartupHook([this](){
                ASTU_SERVICE(ThreadStatusSignalService).AddListener(*this);
            });

            AddShutdownHook([this](){
                ASTU_SERVICE(ThreadStatusSignalService).RemoveListener(*this);
            });
        }

    protected:

        /**
         * Called by this base class when a thread status update has arrived.
         *
         * @param threadId  the ID of the thread
         * @param inStatus  the thread status
         * @return `true` if this signal has been consumed
         */
        virtual bool OnThreadStatusUpdate(int threadId, ThreadStatus inStatus) {
            return false;
        }

    private:

        // Inherited via MouseButtonListener
        virtual bool OnSignal(const ThreadStatusSignal & signal) {
            return OnThreadStatusUpdate(signal.threadId, signal.status);
        }
    };

    /**
     * Manages execution of concurrent threads.
     * 
     * @ingroup srv_group
     */
    class ThreadService
        : virtual public Service
        , public Updatable
        , public TimeClient
        , public SignalEmitterTs<ThreadStatusSignal>
    {
    public:

        /**
         * Constructor.
         *
         * @param updatePriority  the update priority
         */
        ThreadService(int updatePriority = Priority::VeryLow);

        /**
         * Type alias for thread functions.
         */
        using Delegate = std::function<void (int)>;

        /**
         * Virtual destructor.
         */
        virtual ~ThreadService() {}

        /**
         * Starts a new thread executing the specified delegate.
         *
         * @param delegate the delegate to execute
         * @return the numerical identifier of the newly started thread
         */
        int StartThread(Delegate delegate);

        bool HasThreadTs(int id) const;
        ThreadStatus GetStatusTs(int id) const;
        const std::string& GetMessageTs(int id) const;

    private:

        struct ThreadInfo {
            int infoId;
            std::thread::id threadId;
            Delegate delegate;
            ThreadStatus status;
            std::string messages;

            ThreadInfo(int infoId, std::thread::id threadId, Delegate delegate)
                    : infoId(infoId), threadId(threadId), delegate(delegate), status(ThreadStatus::Running) {}

            ThreadInfo() : infoId(0) {}
        };

        /** Used to generate unique thread identifiers. */
        int cntThreads;

        /** Contains status and additional information about threads. */
        std::map<int, ThreadInfo> infos;

        /** The threads. */
        std::vector<std::thread> threads;

        /** Used to synchronize access to thread infos map. */
        mutable std::mutex infoMutex;

        /** Defines the number of update cycles, before stale thread infos get removed. */
        int sweepInterval;

        /** The number of cycles of the current sweep interval. */
        int cntCycles;

        void ExecuteThread(int infoId);

        Delegate& GetDelegateTs(int infoId);
        void SetStatusTs(int infoId, ThreadStatus status);
        void SetStatusTs(int infoId, ThreadStatus status, const std::string& message);
        void VerifyInfoId(int infoId) const;
        void RemoveStaleInfos();

        // Inherited via Service
        void OnStartup() override;
        void OnShutdown() override;

        // Inherited via Updatable
        void OnUpdate() override;
    };

} // end of namespace