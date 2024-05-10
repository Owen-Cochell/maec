/**
 * @file module_param.hpp
 * @author Owen Cochell (owen@gmail.com)
 * @brief Components for module parameters
 * @version 0.1
 * @date 2023-02-03
 * 
 * @copyright Copyright (c) 2023
 * 
 * This file contains components for module parameters,
 * which allow for inter-module connections.
 */

#pragma once

#include <functional>
#include <initializer_list>
#include <utility>
#include <array>

#include "audio_module.hpp"
#include "sink_module.hpp"
#include "meta_audio.hpp"

/**
 * @brief A component that represents a parameter
 * 
 * A ModuleParameter is a component that represents a parameter to be set and accessed.
 * The value of this parameter can be used for many things,
 * and often times modules will utilize this component.
 * For example, an oscillator may use this component for the frequency.
 * 
 * The value of this parameter can differ greatly!
 * Here are some values that we support:
 * 
 * - Static values that do not change
 * - Output from other modules
 * 
 * These parameters offer a modular framework for defining sources for values.
 * Under the hood, this component is a sink,
 * meaning users can attach modules and chains to this parameter.
 * When we are asked to get parameter data, we process the backward modules
 * and return the AudioBuffer for use.
 * Developers can preform this workflow themselves,
 * or they can simply call the get() method which does all this in one call.
 * 
 * It is important to note that this component MUST be included in all state operations!
 * Otherwise, this component may preform oddly or break.
 * An easy way to do this would be to call the necessary state functions in your module.
 * Also, one can call the conf_mod() method to preform the necessary configuration steps.
 * Most of the time, it NOT necessary to respect the finished state of the parameter.
 * As long as the chain is properly stopped, it is NOT required to preform any finish or done operations,
 * but one could do so if deemed necessary.
 * 
 * We also offer some methods for simplifying configuration.
 * Using the proper methods, we can auto-configure this class for getting values
 * from an AudioModule, or this component can configured to return a constant value.
 * Since this component is a sink, one can configure it as any maec module.
 * 
 * One possible use of this component is to offer the ability for parameters to change over time.
 * For example, one could attach a SineOscillator, which would cause the this parameter to represent a sine wave.
 * This allows for components to be modulated.
 * 
 */
class ModuleParam : public SinkModule {

    private:

        /// A constant value to be returned
        long double value = 0;

        /// Pointer to ConstModule
        std::unique_ptr<ConstModule> const_mod = nullptr;

    public:

        ModuleParam() =default;

        /**
         * @brief Construct a parameter, and configures for constant values
         * 
         * Creates a ModuleParameter, and configures it for returning 
         * constant values.
         * Useful for defining an initial value to return.
         * 
         * @param val Constant value to set
         */
        ModuleParam(long double val) { this->set_constant(val); }

        /**
         * @brief Construct a parameter, and configures for constant values
         * 
         * Creates a ModuleParameter, and configures it for returning
         * values sampled from a module.
         * 
         * @param imod Module to track
         */
        ModuleParam(AudioModule* imod) { this->bind(imod); }

        /**
         * @brief Gets the current buffer
         * 
         * We meta process and backward modules,
         * and then return the AudioBuffer.
         * 
         * @return AudioBuffer Buffer of values to work with
         */
        BufferPointer get();

        /**
         * @brief Configures this parameter for constant values.
         * 
         * This sets the underlying constant value to the one provided,
         * and sets the value function to the 'get_constant()' method.
         * 
         * @param val Value to set
         */
        void set_constant(long double val);

        /**
         * @brief Configures this parameter using a given module
         * 
         * This method takes a module and uses it to self-configure this parameter.
         * Users should provide the the module that this parameter is attached to!
         * Also, it is recommended to call this method in info_sync(),
         * AFTER the necessary configuration steps are preformed!
         * This allows the parameter to configure itself in a way that makes sense.
         * 
         * This method simply attached the provided module to the parameter
         * (for reference), and configures the size, channels, and sample rate.
         * From there, it preforms an info sync on the parameter chain.
         * 
         * @param mod Module to use for configuration
         */
        void conf_mod(AudioModule* mod);
};

/**
 * @brief Manages a number of ModuleParam objects
 * 
 * This class collects and maintains a number
 * of ModuleParam objects.
 * This allows for ALL state/info operations to be preformed
 * automatically for any reasserted parameters.
 * 
 * As of now, this class does NOT preform any finish or done operations,
 * we only preform state work with info sync, start, and stop.
 * 
 * This class is recommended to be used by any component that utilizes
 * ModuleParams, but of course if the developers wishes to do so they can
 * preform all the actions here themselves.
 * 
 * The best way to populate this class is to provide pointers to
 * you ModuleParams via construction.
 * 
 */
template <int num>
class ParamModule : public AudioModule {
private:
    std::array<ModuleParam*, num> params = {};

public:
    ParamModule() = default;

    ParamModule(std::initializer_list<ModuleParam*> lst) : params(lst) {}

    /**
     * @brief Starts this module and all attached parameters
     * 
     * This method preforms all the necessary start operations
     * for this module, as well as starting each attached parameter.
     * 
     */
    void start() override {

        // Iterate over each param in array:

        for (ModuleParam* param : this->params) {

            // Start the parameter:

            param->meta_start();
        }
    }

    /**
     * @brief Stops this module and all attached parameters
     * 
     * This method preforms all the necessary stop operations
     * for this module, as well as stopping each attached parameter.
     * 
     */
    void stop() override {

        // Iterate over each param in array:

        for (ModuleParam* param : this->params) {

            // Stop the parameter:

            param->meta_stop();
        }
    }

    /**
     * @brief Preforms a meta info sync operation
     * 
     * This method differs from the default meta_info_sync operation,
     * as we first preform the child sync,
     * then preform meta syncs for all attached parameters.
     * 
     */
    void meta_info_sync() override {

        // First, preform info sync for current module:

        this->info_sync();

        // Next, preform info sync for all parameters:

        for (ModuleParam* param : this->params) {

            param->conf_mod(this);
        }

        // Finally, continue info sync for backwards modules:

        this->backward->meta_info_sync();
    }
};
