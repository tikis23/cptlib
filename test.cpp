#include <iostream>
#include <source_location>
#include <thread>

#include "cptlib.h"
using namespace std::chrono_literals;

#define assert_with_operator(expr1, expr2, op)                                                                                           \
    do {                                                                                                                                 \
        if ((expr1)op(expr2)) {                                                                                                          \
            std::cerr << std::source_location::current().function_name() << "::" << std::source_location::current().line() << std::endl; \
            std::cerr << "    Assertion failed: " << #expr1 << " " << #op << " " << #expr2 << std::endl;                                 \
            std::cerr << "        " << (expr1) << " " << #op << " " << (expr2) << std::endl;                                             \
            std::exit(1);                                                                                                                \
        }                                                                                                                                \
    } while (0)

#define assert_equal(expr1, expr2)         assert_with_operator(expr1, expr2, !=)
#define assert_greater(expr1, expr2)       assert_with_operator(expr1, expr2, <=)
#define assert_greater_equal(expr1, expr2) assert_with_operator(expr1, expr2, <)
#define assert_less(expr1, expr2)          assert_with_operator(expr1, expr2, >=)

#define MILLI_BIAS 20

void TimeDurationTest_Constructors() {
    // default
    {
        cpt::time_duration duration;
        assert_equal(duration.iSec(), 0);
    }
    // chrono duration
    {
        cpt::time_duration duration_s(1s);
        assert_equal(duration_s.iSec(), 1);
        cpt::time_duration duration_ms(5ms);
        assert_equal(duration_ms.iMilli(), 5);
        cpt::time_duration duration_s2 = 2s;
        cpt::time_duration duration_ms2 = 2000ms;
        assert_equal(duration_s2.fSec(), duration_ms2.fSec());

        cpt::time_duration duration_f(1.5s);
        assert_equal(duration_f.iSec(), 1);
    }
    // copy
    {
        cpt::time_duration duration(15min);
        cpt::time_duration duration2 = duration;
        assert_equal(duration.fSec(), duration2.fSec());

        cpt::time_duration duration3(duration);
        assert_equal(duration.fSec(), duration3.fSec());
    }
    // move
    {
        cpt::time_duration duration(45s);
        cpt::time_duration duration2 = std::move(duration);
        assert_equal(duration2.iSec(), 45);

        cpt::time_duration duration3(45s);
        cpt::time_duration duration4(std::move(duration3));
        assert_equal(duration4.iSec(), 45);
    }
}
void TimeDurationTest_Methods() {
    // floats not tested because meh
    {
        cpt::time_duration duration = 1s;
        cpt::time_duration duration2 = 1000ms;
        assert_equal(duration.iMilli(), duration2.iSec() * 1000);
    }
    {
        cpt::time_duration durationNano = 2h;
        cpt::time_duration durationMicro = 2h;
        cpt::time_duration durationMilli = 2h;
        cpt::time_duration durationSec = 2h;
        cpt::time_duration durationMin = 2h;
        cpt::time_duration durationHour = 2h;
        assert_equal(durationNano.iNano(), durationMicro.iMicro() * 1000);
        assert_equal(durationMicro.iMicro(), durationMilli.iMilli() * 1000);
        assert_equal(durationMilli.iMilli(), durationSec.iSec() * 1000);
        assert_equal(durationSec.iSec(), durationMin.iMin() * 60);
        assert_equal(durationMin.iMin(), durationHour.iHour() * 60);
    }
    // chrono
    {
        cpt::time_duration duration = 5s;
        std::chrono::nanoseconds chronoDuration = duration.chrono();
        assert_equal(chronoDuration, 5s);
    }
}
void TimeDurationTest_Operators() {
    // chrono cast
    {
        cpt::time_duration duration = 5s;
        std::chrono::seconds chronoDuration = duration;
        assert_equal(chronoDuration, 5000ms);

        std::chrono::duration<double, std::chrono::seconds::period> chronoDuration2 = duration;
        assert_equal(chronoDuration2, 5s);
    }
    // unary
    {
        cpt::time_duration duration = 1s;
        cpt::time_duration duration2 = -duration;
        assert_equal(duration2.iSec(), -1);
        cpt::time_duration duration3 = +duration;
        assert_equal(duration3.iSec(), 1);

        duration = 1.5s;
        duration2 = -duration;
        assert_equal(duration.iNano(), -duration2.iNano());
        duration3 = +duration;
        assert_equal(duration.iNano(), duration3.iNano());
    }
    // binary
    {
        cpt::time_duration duration = 1s;
        cpt::time_duration duration2 = 1s;
        cpt::time_duration duration3 = duration + duration2;
        assert_equal(duration3.iSec(), 2);

        duration = 5min;
        duration2 = duration + 30s;
        assert_equal(duration2.iNano(), cpt::time_duration(5min + 30s).iNano());

        duration = 5min;
        duration2 = 30s + duration;
        assert_equal(duration2.iNano(), cpt::time_duration(5min + 30s).iNano());

        duration = 5min;
        duration = duration + 0.5s;
        duration2 = 5min + 0.5s;
        assert_equal(duration.iNano(), duration2.iNano());
    }
    {
        cpt::time_duration duration = 1s;
        cpt::time_duration duration2 = 1s;
        cpt::time_duration duration3 = duration - duration2;
        assert_equal(duration3.iSec(), 0);

        duration = 5min;
        duration2 = duration - 30s;
        assert_equal(duration2.iNano(), cpt::time_duration(5min - 30s).iNano());

        duration = 5min;
        duration2 = 30s - duration;
        assert_equal(duration2.iNano(), cpt::time_duration(30s - 5min).iNano());

        duration = 5min;
        duration = duration - 0.5s;
        duration2 = 5min - 0.5s;
        assert_equal(duration.iNano(), duration2.iNano());
    }
    {
        cpt::time_duration duration = 5min;
        cpt::time_duration duration2 = duration * 2;
        assert_equal(duration2.iNano(), cpt::time_duration(5min * 2).iNano());

        duration = 5min;
        duration2 = 2 * duration;
        assert_equal(duration2.iNano(), cpt::time_duration(5min * 2).iNano());

        duration = 5min;
        duration2 = duration * 0.5;
        assert_equal(duration2.iNano(), cpt::time_duration(5min * 0.5).iNano());

        duration = 5min;
        duration2 = 0.5 * duration;
        assert_equal(duration2.iNano(), cpt::time_duration(5min * 0.5).iNano());

        duration = 5min;
        duration = duration * 0.5;
        duration2 = 5min * 0.5;
        assert_equal(duration.iNano(), duration2.iNano());
    }
    {
        cpt::time_duration duration = 5min;
        cpt::time_duration duration2 = 8min;
        int ansInt = duration / duration2;
        double ansDouble = duration / duration2;
        assert_equal(ansInt, 5min / 8min);
        // assert_equal(ansDouble, 5.0min / 8min);

        duration = 5min;
        duration = duration / 2;
        duration2 = 5.0min / 2;
        assert_equal(duration.iNano(), duration2.iNano());

        duration = 5min;
        duration2 = duration / 0.5;
        assert_equal(duration2.iNano(), cpt::time_duration(5.0min / 0.5).iNano());

        duration = 5min;
        ansDouble = duration / 2.5min;
        assert_equal(ansDouble, 5.0min / 2.5min);

        duration = 5min;
        ansDouble = 2.5min / duration;
        assert_equal(ansDouble, 2.5min / 5.0min);
    }
    {
        cpt::time_duration duration = 3s;
        cpt::time_duration duration2 = 5s;
        cpt::time_duration duration3 = duration % duration2;
        assert_equal(duration3.iNano(), cpt::time_duration(3s % 5s).iNano());

        duration = 3min;
        duration2 = 5s;
        duration3 = duration % duration2;
        assert_equal(duration3.iNano(), cpt::time_duration(3min % 5s).iNano());

        duration = 5min;
        duration2 = duration % 2min;
        assert_equal(duration2.iNano(), cpt::time_duration(5min % 2min).iNano());

        duration = 5min;
        duration2 = 2min % duration;
        assert_equal(duration2.iNano(), cpt::time_duration(2min % 5min).iNano());
    }

    // assignment
    {
        cpt::time_duration duration = 5min;
        cpt::time_duration duration2 = 3min;
        duration += duration2;
        assert_equal(duration.iNano(), cpt::time_duration(5min + 3min).iNano());
        duration = 5min;
        duration += 3min;
        assert_equal(duration.iNano(), cpt::time_duration(5min + 3min).iNano());
        duration = 5min;
        duration += 3.5s;
        assert_equal(duration.iNano(), cpt::time_duration(5min + 3.5s).iNano());

        duration = 5min;
        duration2 = 3min;
        duration -= duration2;
        assert_equal(duration.iNano(), cpt::time_duration(5min - 3min).iNano());
        duration = 5min;
        duration -= 3min;
        assert_equal(duration.iNano(), cpt::time_duration(5min - 3min).iNano());
        duration = 5min;
        duration -= 3.5s;
        assert_equal(duration.iNano(), cpt::time_duration(5min - 3.5s).iNano());

        duration = 5min;
        duration *= 2;
        assert_equal(duration.iNano(), cpt::time_duration(5min * 2).iNano());
        duration = 5min;
        duration *= 1.5;
        assert_equal(duration.iNano(), cpt::time_duration(5min * 1.5).iNano());

        duration = 5min;
        duration /= 2;
        assert_equal(duration.iNano(), cpt::time_duration(5.0min / 2).iNano());
        duration = 5min;
        duration /= 1.5;
        assert_equal(duration.iNano(), cpt::time_duration(5min / 1.5).iNano());

        duration = 5min;
        duration2 = 2min;
        duration %= duration2;
        assert_equal(duration.iNano(), cpt::time_duration(5min % 2min).iNano());
        duration = 5min;
        duration %= 2min;
        assert_equal(duration.iNano(), cpt::time_duration(5min % 2min).iNano());
    }
    // comparison
    {
        cpt::time_duration duration = 5min;
        assert_equal(duration == 5min, true);
        assert_equal(duration == 6min, false);
        assert_equal(duration != 5min, false);
        assert_equal(duration != 6min, true);
        assert_equal(duration < 6min, true);
        assert_equal(duration < 5min, false);
        assert_equal(duration > 4min, true);
        assert_equal(duration > 5min, false);
        assert_equal(duration <= 5min, true);
        assert_equal(duration <= 6min, true);
        assert_equal(duration <= 4min, false);
        assert_equal(duration >= 5min, true);
        assert_equal(duration >= 4min, true);
        assert_equal(duration >= 6min, false);

        assert_equal(5min == duration, true);
        assert_equal(6min != duration, true);
        assert_equal(6min > duration, true);
        assert_equal(5min < duration, false);
        assert_equal(4min <= duration, true);
        assert_equal(5min >= duration, true);

        assert_equal(duration == 5.0min, true);

        cpt::time_duration duration2 = 500ms;
        assert_equal(duration == duration2, false);
        assert_equal(duration != duration2, true);
        assert_equal(duration < duration2, false);
        assert_equal(duration > duration2, true);
        assert_equal(duration <= duration2, false);
        assert_equal(duration >= duration2, true);
    }
}
void TimePointTest_Methods() {
    {
        cpt::time_point point;
        std::this_thread::sleep_for(200ms);
        cpt::time_duration elapsed = point.elapsed();
        assert_greater_equal(elapsed.iMilli(), 200 - MILLI_BIAS);
    }
}
void TimePointTest_Operators() {
    // binary
    {
        cpt::time_point point1 = 5min;
        cpt::time_duration duration = 3min;
        cpt::time_point point2 = point1 + duration;
        assert_equal(point2 == 8min, true);
        point2 = duration + point1;
        assert_equal(point2 == 8min, true);

        point2 = point1 + 3min;
        assert_equal(point2 == 8min, true);
        point2 = 3min + point1;
        assert_equal(point2 == 8min, true);

        point2 = point1 + 3.5min;
        assert_equal(point2 == 8.5min, true);
        point2 = 3.5min + point1;
        assert_equal(point2 == 8.5min, true);
    }
    {
        cpt::time_point point1 = 5min;
        cpt::time_duration duration = 3min;
        cpt::time_point point2 = point1 - duration;
        assert_equal(point2 == 2min, true);

        point2 = point1 - 3min;
        assert_equal(point2 == 2min, true);

        point2 = point1 - 3.5min;
        assert_equal(point2 == 1.5min, true);

        point1 = 5min;
        point2 = 3min;
        duration = point1 - point2;
        assert_equal(duration == 2min, true);
        duration = point2 - point1;
        assert_equal(duration == -2min, true);
    }
    // assignment
    {
        cpt::time_point point1 = 5min;
        cpt::time_duration duration = 3min;
        point1 += duration;
        assert_equal(point1 == 8min, true);
        point1 = 5min;
        point1 += 3min;
        assert_equal(point1 == 8min, true);
        point1 = 5min;
        point1 += 3.5min;
        assert_equal(point1 == 8.5min, true);
    }
    {
        cpt::time_point point1 = 5min;
        cpt::time_duration duration = 3min;
        point1 -= duration;
        assert_equal(point1 == 2min, true);
        point1 = 5min;
        point1 -= 3min;
        assert_equal(point1 == 2min, true);
        point1 = 5min;
        point1 -= 3.5min;
        assert_equal(point1 == 1.5min, true);
    }
    // comparison
    {
        cpt::time_point point1 = 5min;
        cpt::time_point point2 = 5min;
        assert_equal(point1 == point2, true);
        assert_equal(point1 != point2, false);
        assert_equal(point1 < point2, false);
        assert_equal(point1 > point2, false);
        assert_equal(point1 <= point2, true);
        assert_equal(point1 >= point2, true);

        point2 = 6min;
        assert_equal(point1 == point2, false);
        assert_equal(point1 != point2, true);
        assert_equal(point1 < point2, true);
        assert_equal(point1 > point2, false);
        assert_equal(point1 <= point2, true);
        assert_equal(point1 >= point2, false);

        point2 = 4min;
        assert_equal(point1 == point2, false);
        assert_equal(point1 != point2, true);
        assert_equal(point1 < point2, false);
        assert_equal(point1 > point2, true);
        assert_equal(point1 <= point2, false);
        assert_equal(point1 >= point2, true);
    }
}
void PauseableClockTest() {
    {
        cpt::time_point<cpt::clocks::pauseable_clock_st<>> point;
        std::this_thread::sleep_for(200ms);
        cpt::time_duration elapsed = point.elapsed();
        assert_greater_equal(elapsed.iMilli(), 200 - MILLI_BIAS);
    }
    {
        cpt::clocks::pauseable_clock_st<>::pause();
        cpt::time_point<cpt::clocks::pauseable_clock_st<>> point;
        std::this_thread::sleep_for(200ms);
        cpt::time_duration elapsed = point.elapsed();
        assert_equal(elapsed.iMilli(), 0);

        cpt::clocks::pauseable_clock_st<>::resume();
        std::this_thread::sleep_for(200ms);
        elapsed = point.elapsed();
        assert_greater_equal(elapsed.iMilli(), 200 - MILLI_BIAS);
        assert_less(elapsed.iMilli(), 400 + MILLI_BIAS);

        cpt::clocks::pauseable_clock_st<>::pause();
        std::this_thread::sleep_for(200ms);
        elapsed = point.elapsed();
        assert_greater_equal(elapsed.iMilli(), 200 - MILLI_BIAS);
        assert_less(elapsed.iMilli(), 400 + MILLI_BIAS);

        cpt::clocks::pauseable_clock_st<>::resume();
        std::this_thread::sleep_for(200ms);
        elapsed = point.elapsed();
        assert_greater_equal(elapsed.iMilli(), 400 - MILLI_BIAS);
        assert_less(elapsed.iMilli(), 600 + MILLI_BIAS);
    }
}

int main() {
    TimeDurationTest_Constructors();
    TimeDurationTest_Methods();
    TimeDurationTest_Operators();

    TimePointTest_Methods();
    TimePointTest_Operators();

    PauseableClockTest();

    std::cout << "All tests passed." << std::endl;
    return 0;
}
