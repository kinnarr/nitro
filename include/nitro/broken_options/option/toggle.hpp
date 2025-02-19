/*
 * Copyright (c) 2015-2016, Technische Universität Dresden, Germany
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions
 *    and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to
 *    endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef INCLUDE_NITRO_BROKEN_OPTIONS_TOGGLE_HPP
#define INCLUDE_NITRO_BROKEN_OPTIONS_TOGGLE_HPP

#include <nitro/broken_options/exception.hpp>
#include <nitro/broken_options/fwd.hpp>
#include <nitro/broken_options/option/base.hpp>

#include <nitro/lang/optional.hpp>

#include <ios>
#include <memory>
#include <sstream>
#include <string>

namespace nitro
{
namespace broken_options
{
    class toggle : public crtp_base<toggle>
    {
    public:
        toggle(const std::string& name, const std::string& description)
        : crtp_base(name, description), ref_(nullptr), given_(0)
        {
        }

    public:
        toggle& ref(int& target)
        {
            ref_ = &target;

            return *this;
        }

    public:
        int given() const
        {
            return given_;
        }

    public:
        virtual void format_value(std::ostream&) const override
        {
        }

    private:
        virtual void update_value(const argument&) override
        {
            if (ref_ != nullptr)
            {
                *ref_ = true;
            }

            ++given_;
        }

        virtual void prepare() override
        {
        }

        virtual void check() override
        {
            if (has_env() && !given())
            {
                auto env_value = nitro::env::get(env());

                if (!env_value.empty())
                {
                    // Python code would look like this:
                    // if bool(env_value):
                    // FeelsBadMan
                    if (env_value == "TRUE" || env_value == "ON" || env_value == "YES" ||
                        env_value == "true" || env_value == "on" || env_value == "yes" ||
                        env_value == "1" || env_value == "Y")
                    {
                        update_value(env_value);
                    }
                    return;
                }
            }
        }

        friend class parser;

    private:
        std::string name_;
        std::string description_;
        nitro::lang::optional<std::string> short_;
        int* ref_;
        int given_;
    };
} // namespace broken_options
} // namespace nitro

#endif // INCLUDE_NITRO_BROKEN_OPTIONS_TOGGLE_HPP
