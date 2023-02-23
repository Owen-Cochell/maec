/**
 * @file meta_audio.hpp
 * @author Owen Cochell (owen@gmail.com)
 * @brief This file contains various meta audio modules
 * @version 0.1
 * @date 2022-09-16
 * 
 * @copyright Copyright (c) 2022
 * 
 * This file contains various 'meta modules'
 * that can be useful, particularly for debugging,
 * but are not strictly necessary for generating audio.
 * 
 */

#pragma once

#include "source_module.hpp"
#include "chrono.hpp"


/**
 * @brief Counts the number of samples this module encounters, and how many times it has been processed
 * 
 * This module counts the size of the buffer from back modules,
 * and saves it for future use.
 * We also count how many times we have been processed.
 * From there, the audio buffer can be grabbed by any forward modules.
 * 
 * This module is for testing only!
 * We don't recommend using it in production...
 * 
 */
class Counter : public AudioModule {

    private:

        /// Number of times we have been processed
        int m_processed = 0;

        /// Number of samples we have encountered
        int m_samples = 0;

    public:

        /**
         * @brief Resets this module
         * 
         * We simply set the number of processed and samples 
         * encountered to zero.
         * 
         */
        void reset() { this->m_processed = 0; this->m_samples = 0;}

        /**
         * @brief Gets the number of times we have been processed
         * 
         * @return int Number of times we have been processed
         */
        int processed() const { return this->m_processed; }

        /**
         * @brief Gets the number of samples we have encountered
         * 
         * @return int Number of samples encountered
         */
        int samples() const { return this->m_samples; }

        /**
         * @brief Counts how many samples we encounter, and how many times we have been called
         * 
         * We keep track of the info specified above.
         * 
         */
        void process() override;
};

/**
 * @brief Measures various timing statistics for determining latency.
 * 
 * Time information will be stored about each operation.
 * An operation is defined as the meta process of back modules.
 * So, if we time an operation, it will be the time it took
 * for back modules to process.
 * 
 * We measure the following values:
 * 
 * - total time elapsed since start
 * - expected time since start
 * - time of last operation
 * - latency of last operation
 * - average time of each operation
 * - average latency of each operation
 * 
 * We utilize a chain timer that allows us to determine
 * the idea time.
 * We also inherit from the Counter to aid with the process
 * of counting samples and times processed.
 * We do our timekeeping in nanoseconds.
 */
class LatencyModule : public Counter {

    private:

        /// Time we started on
        int64_t start_time = 0;

        /// Time of the last operation
        int64_t operation_time = 0;

        /// Latency of the last operation
        int64_t operation_latency = 0;

        /// Sum of all operation times
        int64_t total_operation_time = 0;

        /// Sum of all latency times
        int64_t total_operation_latency = 0;

        /// Chain timer for idea timekeeping
        ChainTimer timer;

    public:

        LatencyModule() =default;

        /**
         * @brief Resets this module
         * 
         * We simply clear all variables associated with this class,
         * as well as set the start time to now.
         * 
         */
        void reset();

        /**
         * @brief Gets the start time for this module
         * 
         * This value does not always correlate to anything!
         * We simply use it to determine time deltas down the line.
         * 
         * @return int Start time in nanoseconds
         */
        int64_t get_start_time() const { return this->start_time; }

        /**
         * @brief Gets the total elapsed time in nanoseconds
         * 
         * We utilize the start time to determine the total elapsed time.
         * This value does not take into account the processing time!
         * Any dead time spent doing nothing will be included here,
         * so it is recommended to use something more accurate,
         * such as the operation or latency time.
         * 
         * @return int Total time elapsed since start in nanoseconds
         */
        int64_t elapsed() const { return get_time() - this->start_time; }

        /**
         * @brief Gets the expected time in nanoseconds
         * 
         * We utilize the ChainTimer to get the time we expect to have elapsed
         * since the start of processing.
         * 
         * @return int Expected elapsed time in nanoseconds
         */
        int64_t expected() const { return this->timer.get_time(); }

        /**
         * @brief Gets the time of the last operation in nanoseconds
         * 
         * The operation time is the total time spend meta processing
         * all back modules.
         * 
         * @return int Elapsed operation time in nanoseconds
         */
        int64_t time() const { return this->operation_time; }

        /**
         * @brief Gets the total operation time in nanoseconds
         * 
         * We sum the time elapsed after each operation.
         * 
         * @return int Total operation time in nanoseconds
         */
        int64_t total_time() const { return this->total_operation_time; }

        /**
         * @brief Gets the latency of the last operation in nanoseconds
         * 
         * We determine the latency by subtracting the total time by the expected time.
         * Please note, a negative latency is possible!
         * This can happen if we are processing audio data way faster than the expected sample rate!
         * A negative latency value is good!
         * 
         * @return int Latency of last operation in nanoseconds
         */
        int64_t latency() const { return this->operation_latency; }

        /**
         * @brief Gets the total latency in nanoseconds
         * 
         * We sum the latency of each operation.
         * 
         * @return int Total latency in nanoseconds.
         */
        int64_t total_latency() const { return this->total_operation_latency; }

        /**
         * @brief Gets the average operation time in nanoseconds
         * 
         * We compute the average operation time 
         * by using the total operation time and the number of times processed.
         * 
         * @return int Average operation time in nanoseconds
         */
        int64_t average_time() const { return this->total_operation_time / this->processed(); }

        /**
         * @brief Gets the average operation latency in nanoseconds
         * 
         * We compute the average operation latency
         * by using the total operation latency and the number of times processed
         * 
         * @return int Average operation latency in nanoseconds
         */
        int64_t average_latency() const { return this->total_operation_latency / this->processed(); }

        /**
         * @brief Starts this latency module
         * 
         * We simply set the start time for later use.
         * 
         */
        void start() override { this->start_time = get_time(); }

        /**
         * @brief Meta process this module
         * 
         * We do some timekeeping operations
         * and save them for future use.
         * 
         */
        void meta_process() override;

        /**
         * @brief Dummy process method
         * 
         * We simply do nothing.
         * This allows any subclassed modules
         * to put custom stuff here,
         * and prevents the count module from being
         * processed twice.
         * 
         */
        void process() override {}

};

/**
 * @brief Repeats a buffer for output
 * 
 * This module will save a buffer, and repeat it forever.
 * We do this by copying an internal buffer to a new one when processed.
 * Please keep this performance issue in mind when using this module.
 * 
 * We also set the size of the chain to match the size of this buffer.
 * TODO: Is this a good idea?
 */
class BufferModule : public SourceModule {

    private:

        /// The 'good' copy to repeat
        AudioBuffer* gbuff = nullptr;

    public:

        BufferModule() =default;

        explicit BufferModule(AudioBuffer* ibuff) { this->set_rbuffer(ibuff); }

        /**
         * @brief Sets the buffer to repeat
         * 
         * @param ibuff Buffer to repeat
         */
        void set_rbuffer(AudioBuffer* ibuff) { this->gbuff = ibuff; this->get_info()->buff_size = ibuff->size(); }

        /**
         * @brief Gets the buffer being repeated
         * 
         * @return AudioBuffer* Buffer being repeated
         */
        AudioBuffer* get_rbuffer() const { return this->gbuff; }

        /**
         * @brief Copys the initial buffer into a new one
         * 
         * We create a new buffer to be returned, 
         * and copy the old contents into the new one.
         * 
         */
        void process() override;
};
