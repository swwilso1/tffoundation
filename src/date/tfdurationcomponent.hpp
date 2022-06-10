/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2022 to 2022 Tectiform Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


******************************************************************************/

#ifndef TFDURATIONCOMPONENT_HPP
#define TFDURATIONCOMPONENT_HPP

#define NEEDS_CHRONO
#define NEEDS_CSTDINT
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfdurationtypes.hpp"

namespace TF::Foundation
{

    class DurationComponents : public AllocatorInterface
    {
    public:
        using size_type = uint64_t;

        DurationComponents() = default;

        DurationComponents(int32_t millenia, int32_t centuries, int32_t decades, int32_t years, int32_t months,
                           int32_t weeks, int32_t days, int32_t hours, int32_t minutes, int32_t seconds,
                           int32_t milliseconds, int32_t microseconds) :
            m_millenia{millenia},
            m_centuries{centuries}, m_decades{decades}, m_years{years}, m_months{months}, m_weeks{weeks}, m_days{days},
            m_hours{hours}, m_minutes{minutes}, m_seconds{seconds}, m_milliseconds{milliseconds}, m_microseconds{
                                                                                                      microseconds}
        {}

        template<typename Rep, typename Period>
        explicit DurationComponents(const std::chrono::duration<Rep, Period> & d)
        {
            deconstruct_duration_into_components(d);
        }

        DurationComponents & operator=(const DurationComponents & c) = default;

        DurationComponents & operator=(DurationComponents && c) noexcept = default;

        bool operator==(const DurationComponents & c) const;

        bool operator!=(const DurationComponents & c) const;

        [[nodiscard]] int32_t get_microseconds() const;

        void set_microseconds(int32_t f);

        [[nodiscard]] int32_t get_milliseconds() const;

        void set_milliseconds(int32_t f);

        [[nodiscard]] int32_t get_seconds() const;

        void set_seconds(int32_t s);

        [[nodiscard]] int32_t get_minutes() const;

        void set_minutes(int32_t m);

        [[nodiscard]] int32_t get_hours() const;

        void set_hours(int32_t h);

        [[nodiscard]] int32_t get_days() const;

        void set_days(int32_t d);

        [[nodiscard]] int32_t get_weeks() const;

        void set_weeks(int32_t w);

        [[nodiscard]] int32_t get_months() const;

        void set_months(int32_t m);

        [[nodiscard]] int32_t get_years() const;

        void set_years(int32_t y);

        [[nodiscard]] int32_t get_decades() const;

        void set_decades(int32_t d);

        [[nodiscard]] int32_t get_centuries() const;

        void set_centuries(int32_t c);

        [[nodiscard]] int32_t get_millenia() const;

        void set_millenia(int32_t m);

        template<typename Rep, typename Period>
        std::chrono::duration<Rep, Period> get_as_duration() const
        {
            return reconstruct_duration_from_components<Rep, Period>();
        }

    private:
        int32_t m_millenia{};
        int32_t m_centuries{};
        int32_t m_decades{};
        int32_t m_years{};
        int32_t m_months{};
        int32_t m_weeks{};
        int32_t m_days{};
        int32_t m_hours{};
        int32_t m_minutes{};
        int32_t m_seconds{};
        int32_t m_milliseconds{};
        int32_t m_microseconds{};

        template<typename Duration, typename T>
        struct DeconstructorHelper
        {
            Duration time_left{};
            T result{};

            DeconstructorHelper() {}

            DeconstructorHelper(const Duration & d, T t) : time_left{d}, result{t} {}
        };

        template<typename PrimaryDuration, typename ConversionDuration, typename T>
        DeconstructorHelper<PrimaryDuration, T> deconstruct(const PrimaryDuration & remaining)
        {
            DeconstructorHelper<PrimaryDuration, T> helper_result{};

            auto dur = std::chrono::duration_cast<ConversionDuration>(remaining);
            helper_result.result = static_cast<T>(dur.count());

            auto back_cast = std::chrono::duration_cast<PrimaryDuration>(dur);
            helper_result.time_left = remaining - back_cast;
            return helper_result;
        }

        template<typename Rep, typename Period>
        void deconstruct_duration_into_components(const std::chrono::duration<Rep, Period> & d)
        {
            auto m_helper = deconstruct<std::chrono::duration<Rep, Period>, millenia, decltype(m_millenia)>(d);
            m_millenia = m_helper.result;

            auto c_helper =
                deconstruct<std::chrono::duration<Rep, Period>, centuries, decltype(m_centuries)>(m_helper.time_left);
            m_centuries = c_helper.result;

            auto d_helper =
                deconstruct<std::chrono::duration<Rep, Period>, decades, decltype(m_decades)>(c_helper.time_left);
            m_decades = d_helper.result;

            auto y_helper = deconstruct<std::chrono::duration<Rep, Period>, std::chrono::years, decltype(m_years)>(
                d_helper.time_left);
            m_years = y_helper.result;

            auto mo_helper = deconstruct<std::chrono::duration<Rep, Period>, std::chrono::months, decltype(m_months)>(
                y_helper.time_left);
            m_months = mo_helper.result;

            auto we_helper = deconstruct<std::chrono::duration<Rep, Period>, std::chrono::weeks, decltype(m_weeks)>(
                mo_helper.time_left);
            m_weeks = we_helper.result;

            auto da_helper = deconstruct<std::chrono::duration<Rep, Period>, std::chrono::days, decltype(m_days)>(
                we_helper.time_left);
            m_days = da_helper.result;

            auto hr_helper = deconstruct<std::chrono::duration<Rep, Period>, std::chrono::hours, decltype(m_hours)>(
                da_helper.time_left);
            m_hours = hr_helper.result;

            auto min_helper =
                deconstruct<std::chrono::duration<Rep, Period>, std::chrono::minutes, decltype(m_minutes)>(
                    hr_helper.time_left);
            m_minutes = min_helper.result;

            auto sec_helper =
                deconstruct<std::chrono::duration<Rep, Period>, std::chrono::seconds, decltype(m_seconds)>(
                    min_helper.time_left);
            m_seconds = sec_helper.result;

            auto mi_helper =
                deconstruct<std::chrono::duration<Rep, Period>, std::chrono::milliseconds, decltype(m_milliseconds)>(
                    sec_helper.time_left);
            m_milliseconds = mi_helper.result;

            auto frac_helper =
                deconstruct<std::chrono::duration<Rep, Period>, std::chrono::microseconds, decltype(m_microseconds)>(
                    mi_helper.time_left);
            m_microseconds = frac_helper.result;
        }

        template<typename Rep, typename Period>
        std::chrono::duration<Rep, Period> reconstruct_duration_from_components() const
        {
            std::chrono::duration<Rep, Period> new_duration{0};

            auto millenia_dur = millenia{m_millenia};
            new_duration += std::chrono::duration_cast<decltype(new_duration)>(millenia_dur);

            auto centuries_dur = centuries(m_centuries);
            new_duration += std::chrono::duration_cast<decltype(new_duration)>(centuries_dur);

            auto decades_dur = decades(m_decades);
            new_duration += std::chrono::duration_cast<decltype(new_duration)>(decades_dur);

            auto years_dur = std::chrono::years(m_years);
            new_duration += std::chrono::duration_cast<decltype(new_duration)>(years_dur);

            auto months_dur = std::chrono::years(m_months);
            new_duration += std::chrono::duration_cast<decltype(new_duration)>(months_dur);

            auto weeks_dur = std::chrono::weeks(m_weeks);
            new_duration += std::chrono::duration_cast<decltype(new_duration)>(weeks_dur);

            auto days_dur = std::chrono::days(m_days);
            new_duration += std::chrono::duration_cast<decltype(new_duration)>(days_dur);

            auto hours_dur = std::chrono::hours(m_hours);
            new_duration += std::chrono::duration_cast<decltype(new_duration)>(hours_dur);

            auto minutes_dur = std::chrono::minutes(m_minutes);
            new_duration += std::chrono::duration_cast<decltype(new_duration)>(minutes_dur);

            auto seconds_dur = std::chrono::seconds(m_seconds);
            new_duration += std::chrono::duration_cast<decltype(new_duration)>(seconds_dur);

            auto milliseconds_dur = std::chrono::milliseconds(m_milliseconds);
            new_duration += std::chrono::duration_cast<decltype(new_duration)>(milliseconds_dur);

            auto fractions_dur = std::chrono::microseconds(m_microseconds);
            new_duration += std::chrono::duration_cast<decltype(new_duration)>(fractions_dur);

            return new_duration;
        }
    };

} // namespace TF::Foundation

#endif // TFDURATIONCOMPONENT_HPP
