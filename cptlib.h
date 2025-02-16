#pragma once

#include <chrono>
#include <stdint.h>

namespace cpt {
class time_duration {
public:
    using floatRep = double;
    using intRep = int64_t;
    using thisRep = intRep; // TODO: Make this a template parameter (maybe)
    using thisPeriod = std::nano;

    constexpr time_duration() noexcept : m_duration(0) {}
    template <class Rep, class Period>
        requires(std::is_convertible_v<std::chrono::duration<Rep, Period>, std::chrono::duration<thisRep, thisPeriod>>)
    constexpr time_duration(const std::chrono::duration<Rep, Period>& duration) noexcept(std::is_arithmetic_v<Rep>)
        : m_duration(duration) {}
    template <class Rep, class Period>
        requires(!std::is_convertible_v<std::chrono::duration<Rep, Period>, std::chrono::duration<thisRep, thisPeriod>>)
    constexpr time_duration(const std::chrono::duration<Rep, Period>& duration) noexcept(std::is_arithmetic_v<Rep>)
        : m_duration(std::chrono::duration_cast<decltype(m_duration)>(duration)) {}

    constexpr double fNano() const noexcept {
        return std::chrono::duration_cast<std::chrono::duration<floatRep, std::chrono::nanoseconds::period>>(m_duration).count();
    }
    constexpr double fMicro() const noexcept {
        return std::chrono::duration_cast<std::chrono::duration<floatRep, std::chrono::microseconds::period>>(m_duration).count();
    }
    constexpr double fMilli() const noexcept {
        return std::chrono::duration_cast<std::chrono::duration<floatRep, std::chrono::milliseconds::period>>(m_duration).count();
    }
    constexpr double fSec() const noexcept {
        return std::chrono::duration_cast<std::chrono::duration<floatRep, std::chrono::seconds::period>>(m_duration).count();
    }
    constexpr double fMin() const noexcept {
        return std::chrono::duration_cast<std::chrono::duration<floatRep, std::chrono::minutes::period>>(m_duration).count();
    }
    constexpr double fHour() const noexcept {
        return std::chrono::duration_cast<std::chrono::duration<floatRep, std::chrono::hours::period>>(m_duration).count();
    }

    constexpr int64_t iNano() const noexcept {
        return std::chrono::duration_cast<std::chrono::duration<intRep, std::chrono::nanoseconds::period>>(m_duration).count();
    }
    constexpr int64_t iMicro() const noexcept {
        return std::chrono::duration_cast<std::chrono::duration<intRep, std::chrono::microseconds::period>>(m_duration).count();
    }
    constexpr int64_t iMilli() const noexcept {
        return std::chrono::duration_cast<std::chrono::duration<intRep, std::chrono::milliseconds::period>>(m_duration).count();
    }
    constexpr int64_t iSec() const noexcept {
        return std::chrono::duration_cast<std::chrono::duration<intRep, std::chrono::seconds::period>>(m_duration).count();
    }
    constexpr int64_t iMin() const noexcept {
        return std::chrono::duration_cast<std::chrono::duration<intRep, std::chrono::minutes::period>>(m_duration).count();
    }
    constexpr int64_t iHour() const noexcept {
        return std::chrono::duration_cast<std::chrono::duration<intRep, std::chrono::hours::period>>(m_duration).count();
    }

    constexpr auto chrono() const noexcept { return m_duration; }

    template <class Rep, class Period>
    constexpr operator std::chrono::duration<Rep, Period>() const noexcept(std::is_arithmetic_v<Rep>) {
        return std::chrono::duration_cast<std::chrono::duration<Rep, Period>>(m_duration);
    }

    constexpr time_duration operator+() const noexcept { return +m_duration; }
    constexpr time_duration operator-() const noexcept { return -m_duration; }

    constexpr time_duration operator+(const time_duration& other) const noexcept { return m_duration + other.m_duration; }
    template <class Rep, class Period>
    constexpr time_duration operator+(const std::chrono::duration<Rep, Period>& other) const noexcept(std::is_arithmetic_v<Rep>) {
        return m_duration + other;
    }
    template <class Rep, class Period>
    friend constexpr time_duration operator+(const std::chrono::duration<Rep, Period>& lhs,
                                             const time_duration& rhs) noexcept(std::is_arithmetic_v<Rep>) {
        return lhs + rhs.m_duration;
    }

    constexpr time_duration operator-(const time_duration& other) const noexcept { return m_duration - other.m_duration; }
    template <class Rep, class Period>
    constexpr time_duration operator-(const std::chrono::duration<Rep, Period>& other) const noexcept(std::is_arithmetic_v<Rep>) {
        return m_duration - other;
    }
    template <class Rep, class Period>
    friend constexpr time_duration operator-(const std::chrono::duration<Rep, Period>& lhs,
                                             const time_duration& rhs) noexcept(std::is_arithmetic_v<Rep>) {
        return lhs - rhs.m_duration;
    }

    template <class T>
        requires(std::is_arithmetic_v<T>)
    constexpr time_duration operator*(const T& other) const noexcept(std::is_arithmetic_v<T>) {
        return m_duration * other;
    }
    template <class T>
        requires(std::is_arithmetic_v<T>)
    friend constexpr time_duration operator*(const T& lhs, const time_duration& rhs) noexcept(std::is_arithmetic_v<T>) {
        return lhs * rhs.m_duration;
    }

    // If you need floating point division, cast to chrono::duration<double> first
    constexpr thisRep operator/(const time_duration& other) const noexcept { return m_duration / other.m_duration; }
    template <class T>
        requires(std::is_arithmetic_v<T>)
    constexpr time_duration operator/(const T& other) const noexcept(std::is_arithmetic_v<T>) {
        return m_duration / other;
    }
    template <class Rep, class Period>
    constexpr Rep operator/(const std::chrono::duration<Rep, Period>& other) const noexcept(std::is_arithmetic_v<Rep>) {
        return m_duration / other;
    }
    template <class Rep, class Period>
    friend constexpr Rep operator/(const std::chrono::duration<Rep, Period>& lhs,
                                   const time_duration& rhs) noexcept(std::is_arithmetic_v<Rep>) {
        return lhs / rhs.m_duration;
    }

    constexpr time_duration operator%(const time_duration& other) const noexcept { return m_duration % other.m_duration; }
    template <class Rep, class Period>
    constexpr time_duration operator%(const std::chrono::duration<Rep, Period>& other) const noexcept(std::is_arithmetic_v<Rep>) {
        return m_duration % other;
    }
    template <class Rep, class Period>
    friend constexpr time_duration operator%(const std::chrono::duration<Rep, Period>& lhs,
                                             const time_duration& rhs) noexcept(std::is_arithmetic_v<Rep>) {
        return lhs % rhs.m_duration;
    }

    constexpr time_duration& operator+=(const time_duration& other) noexcept {
        m_duration += other.m_duration;
        return *this;
    }
    template <class Rep, class Period>
        requires(std::is_same_v<thisRep, Rep>)
    constexpr time_duration& operator+=(const std::chrono::duration<Rep, Period>& other) noexcept(std::is_arithmetic_v<Rep>) {
        m_duration += other;
        return *this;
    }
    template <class Rep, class Period>
        requires(!std::is_same_v<thisRep, Rep>)
    constexpr time_duration& operator+=(const std::chrono::duration<Rep, Period>& other) noexcept(std::is_arithmetic_v<Rep>) {
        m_duration = std::chrono::duration_cast<decltype(m_duration)>(m_duration + other);
        return *this;
    }

    constexpr time_duration& operator-=(const time_duration& other) noexcept {
        m_duration -= other.m_duration;
        return *this;
    }
    template <class Rep, class Period>
        requires(std::is_same_v<thisRep, Rep>)
    constexpr time_duration& operator-=(const std::chrono::duration<Rep, Period>& other) noexcept(std::is_arithmetic_v<Rep>) {
        m_duration -= other;
        return *this;
    }
    template <class Rep, class Period>
        requires(!std::is_same_v<thisRep, Rep>)
    constexpr time_duration& operator-=(const std::chrono::duration<Rep, Period>& other) noexcept(std::is_arithmetic_v<Rep>) {
        m_duration = std::chrono::duration_cast<decltype(m_duration)>(m_duration - other);
        return *this;
    }

    template <class T>
        requires((std::is_integral_v<T> && std::is_integral_v<thisRep>) ||
                 (std::is_floating_point_v<T> && std::is_floating_point_v<thisRep>))
    constexpr time_duration& operator*=(const T& other) noexcept(std::is_arithmetic_v<T>) {
        m_duration *= other;
        return *this;
    }
    template <class T>
        requires(!((std::is_integral_v<T> && std::is_integral_v<thisRep>) ||
                   (std::is_floating_point_v<T> && std::is_floating_point_v<thisRep>)))
    constexpr time_duration& operator*=(const T& other) noexcept(std::is_arithmetic_v<T>) {
        m_duration = std::chrono::duration_cast<decltype(m_duration)>(m_duration * other);
        return *this;
    }

    template <class T>
        requires((std::is_integral_v<T> && std::is_integral_v<thisRep>) ||
                 (std::is_floating_point_v<T> && std::is_floating_point_v<thisRep>))
    constexpr time_duration& operator/=(const T& other) noexcept(std::is_arithmetic_v<T>) {
        m_duration /= other;
        return *this;
    }
    template <class T>
        requires(!((std::is_integral_v<T> && std::is_integral_v<thisRep>) ||
                   (std::is_floating_point_v<T> && std::is_floating_point_v<thisRep>)))
    constexpr time_duration& operator/=(const T& other) noexcept(std::is_arithmetic_v<T>) {
        m_duration = std::chrono::duration_cast<decltype(m_duration)>(m_duration / other);
        return *this;
    }

    constexpr time_duration& operator%=(const time_duration& other) noexcept {
        m_duration %= other.m_duration;
        return *this;
    }
    template <class Rep, class Period>
    constexpr time_duration& operator%=(const std::chrono::duration<Rep, Period>& other) noexcept(std::is_arithmetic_v<Rep>) {
        m_duration %= other;
        return *this;
    }

    constexpr bool operator==(const time_duration& other) const noexcept { return m_duration == other.m_duration; }
    template <class Rep, class Period>
    constexpr bool operator==(const std::chrono::duration<Rep, Period>& other) const noexcept(std::is_arithmetic_v<Rep>) {
        return m_duration == other;
    }
    template <class Rep, class Period>
    friend constexpr bool operator==(const std::chrono::duration<Rep, Period>& lhs,
                                     const time_duration& rhs) noexcept(std::is_arithmetic_v<Rep>) {
        return lhs == rhs.m_duration;
    }

    constexpr bool operator!=(const time_duration& other) const noexcept { return m_duration != other.m_duration; }
    template <class Rep, class Period>
    constexpr bool operator!=(const std::chrono::duration<Rep, Period>& other) const noexcept(std::is_arithmetic_v<Rep>) {
        return m_duration != other;
    }
    template <class Rep, class Period>
    friend constexpr bool operator!=(const std::chrono::duration<Rep, Period>& lhs,
                                     const time_duration& rhs) noexcept(std::is_arithmetic_v<Rep>) {
        return lhs != rhs.m_duration;
    }

    constexpr bool operator<(const time_duration& other) const noexcept { return m_duration < other.m_duration; }
    template <class Rep, class Period>
    constexpr bool operator<(const std::chrono::duration<Rep, Period>& other) const noexcept(std::is_arithmetic_v<Rep>) {
        return m_duration < other;
    }
    template <class Rep, class Period>
    friend constexpr bool operator<(const std::chrono::duration<Rep, Period>& lhs,
                                    const time_duration& rhs) noexcept(std::is_arithmetic_v<Rep>) {
        return lhs < rhs.m_duration;
    }

    constexpr bool operator>(const time_duration& other) const noexcept { return m_duration > other.m_duration; }
    template <class Rep, class Period>
    constexpr bool operator>(const std::chrono::duration<Rep, Period>& other) const noexcept(std::is_arithmetic_v<Rep>) {
        return m_duration > other;
    }
    template <class Rep, class Period>
    friend constexpr bool operator>(const std::chrono::duration<Rep, Period>& lhs,
                                    const time_duration& rhs) noexcept(std::is_arithmetic_v<Rep>) {
        return lhs > rhs.m_duration;
    }

    constexpr bool operator<=(const time_duration& other) const noexcept { return m_duration <= other.m_duration; }
    template <class Rep, class Period>
    constexpr bool operator<=(const std::chrono::duration<Rep, Period>& other) const noexcept(std::is_arithmetic_v<Rep>) {
        return m_duration <= other;
    }
    template <class Rep, class Period>
    friend constexpr bool operator<=(const std::chrono::duration<Rep, Period>& lhs,
                                     const time_duration& rhs) noexcept(std::is_arithmetic_v<Rep>) {
        return lhs <= rhs.m_duration;
    }

    constexpr bool operator>=(const time_duration& other) const noexcept { return m_duration >= other.m_duration; }
    template <class Rep, class Period>
    constexpr bool operator>=(const std::chrono::duration<Rep, Period>& other) const noexcept(std::is_arithmetic_v<Rep>) {
        return m_duration >= other;
    }
    template <class Rep, class Period>
    friend constexpr bool operator>=(const std::chrono::duration<Rep, Period>& lhs,
                                     const time_duration& rhs) noexcept(std::is_arithmetic_v<Rep>) {
        return lhs >= rhs.m_duration;
    }

private:
    std::chrono::duration<thisRep, thisPeriod> m_duration;
};

template <class Clock = std::chrono::steady_clock>
    requires(std::chrono::is_clock_v<Clock>)
class time_point {
public:
    constexpr time_point() noexcept : m_timePoint(Clock::now()) {}
    template <class Duration>
        requires(std::is_convertible_v<Duration, typename Clock::duration>)
    constexpr time_point(const std::chrono::time_point<Clock, Duration>& time_point) noexcept : m_timePoint(time_point) {}
    template <class Duration>
        requires(!std::is_convertible_v<Duration, typename Clock::duration>)
    constexpr time_point(const std::chrono::time_point<Clock, Duration>& time_point) noexcept
        : m_timePoint(std::chrono::time_point_cast<typename Clock::duration>(time_point)) {}
    template <class Rep, class Period>
        requires(std::is_convertible_v<std::chrono::duration<Rep, Period>, typename Clock::duration>)
    constexpr time_point(const std::chrono::duration<Rep, Period>& duration) noexcept(std::is_arithmetic_v<Rep>) : m_timePoint(duration) {}
    template <class Rep, class Period>
        requires(!std::is_convertible_v<std::chrono::duration<Rep, Period>, typename Clock::duration>)
    constexpr time_point(const std::chrono::duration<Rep, Period>& duration) noexcept(std::is_arithmetic_v<Rep>)
        : m_timePoint(std::chrono::duration_cast<typename Clock::duration>(duration)) {}

    constexpr time_duration elapsed() const noexcept { return Clock::now() - m_timePoint; }

    constexpr auto chrono() const noexcept { return m_timePoint; }

    constexpr operator std::chrono::time_point<Clock>() const noexcept { return m_timePoint; }

    constexpr time_point operator+(const time_duration& duration) const noexcept { return m_timePoint + duration.chrono(); }
    friend constexpr time_point operator+(const time_duration& duration, const time_point& point) noexcept {
        return duration.chrono() + point.m_timePoint;
    }
    template <class Rep, class Period>
    constexpr time_point operator+(const std::chrono::duration<Rep, Period>& duration) const noexcept(std::is_arithmetic_v<Rep>) {
        return m_timePoint + duration;
    }

    constexpr time_point operator-(const time_duration& duration) const noexcept { return m_timePoint - duration.chrono(); }
    template <class Rep, class Period>
    constexpr time_point operator-(const std::chrono::duration<Rep, Period>& duration) const noexcept(std::is_arithmetic_v<Rep>) {
        return m_timePoint - duration;
    }
    constexpr time_duration operator-(const time_point& other) const noexcept { return m_timePoint - other.m_timePoint; }

    constexpr time_point& operator+=(const time_duration& duration) noexcept {
        m_timePoint += duration.chrono();
        return *this;
    }
    constexpr time_point& operator-=(const time_duration& duration) noexcept {
        m_timePoint -= duration.chrono();
        return *this;
    }

    constexpr bool operator==(const time_point& other) const noexcept { return m_timePoint == other.m_timePoint; }
    constexpr bool operator!=(const time_point& other) const noexcept { return m_timePoint != other.m_timePoint; }
    constexpr bool operator<(const time_point& other) const noexcept { return m_timePoint < other.m_timePoint; }
    constexpr bool operator>(const time_point& other) const noexcept { return m_timePoint > other.m_timePoint; }
    constexpr bool operator<=(const time_point& other) const noexcept { return m_timePoint <= other.m_timePoint; }
    constexpr bool operator>=(const time_point& other) const noexcept { return m_timePoint >= other.m_timePoint; }

private:
    std::chrono::time_point<Clock> m_timePoint;
};
} // namespace cpt

namespace cpt::clocks {
// If you want to have different clocks that can be paused, use 'UniqueIdentifier' to differentiate them.
// Currently, there is no way to create new pauseable_clocks at runtime.
// TODO: provide multi-threaded variant
template <typename UniqueIdentifier = void, class BaseClock = std::chrono::steady_clock>
    requires(std::chrono::is_clock_v<BaseClock>)
struct pauseable_clock_st {
    using rep = BaseClock::rep;
    using period = BaseClock::period;
    using duration = BaseClock::duration;
    using time_point = std::chrono::time_point<pauseable_clock_st>;
    static constexpr bool is_steady = false; // can pause

    static constexpr time_point now() noexcept {
        if (m_paused) {
            return time_point(m_pauseStartPoint - m_totalPauseDuration);
        } else {
            return time_point(BaseClock::now().time_since_epoch() - m_totalPauseDuration);
        }
    }

    static constexpr void pause() noexcept {
        if (m_paused)
            return;
        m_pauseStartPoint = BaseClock::now().time_since_epoch();
        m_paused = true;
    }
    static constexpr void resume() noexcept {
        if (!m_paused)
            return;
        m_totalPauseDuration += BaseClock::now().time_since_epoch() - m_pauseStartPoint;
        m_paused = false;
    }
    static constexpr bool is_paused() noexcept { return m_paused; }

private:
    static inline bool m_paused{false};
    static inline duration m_pauseStartPoint{0};
    static inline duration m_totalPauseDuration{0};
};
} // namespace cpt::clocks