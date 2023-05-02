#pragma once

#include <cmath>
#include "pocketpy.h"

namespace pkpy{

// https://easings.net/

const double PI = 3.1415926545;

inline static double easeInSine( double x ) {
	return 1.0 - std::cos( x * PI / 2 );
}

inline static double easeOutSine( double x ) {
	return std::sin( x * PI / 2 );
}

inline static double easeInOutSine( double x ) {
	return -( std::cos( PI * x ) - 1 ) / 2;
}

inline static double easeInQuad( double x ) {
    return x * x;
}

inline static double easeOutQuad( double x ) {
    return 1 - std::pow( 1 - x, 2 );
}

inline static double easeInOutQuad( double x ) {
    if( x < 0.5 ) {
        return 2 * x * x;
    } else {
        return 1 - std::pow( -2 * x + 2, 2 ) / 2;
    }
}

inline static double easeInCubic( double x ) {
    return x * x * x;
}

inline static double easeOutCubic( double x ) {
    return 1 - std::pow( 1 - x, 3 );
}

inline static double easeInOutCubic( double x ) {
    if( x < 0.5 ) {
        return 4 * x * x * x;
    } else {
        return 1 - std::pow( -2 * x + 2, 3 ) / 2;
    }
}

inline static double easeInQuart( double x ) {
    return std::pow( x, 4 );
}

inline static double easeOutQuart( double x ) {
    return 1 - std::pow( 1 - x, 4 );
}

inline static double easeInOutQuart( double x ) {
    if( x < 0.5 ) {
        return 8 * std::pow( x, 4 );
    } else {
        return 1 - std::pow( -2 * x + 2, 4 ) / 2;
    }
}

inline static double easeInQuint( double x ) {
    return std::pow( x, 5 );
}

inline static double easeOutQuint( double x ) {
    return 1 - std::pow( 1 - x, 5 );
}

inline static double easeInOutQuint( double x ) {
    if( x < 0.5 ) {
        return 16 * std::pow( x, 5 );
    } else {
        return 1 - std::pow( -2 * x + 2, 5 ) / 2;
    }
}

inline static double easeInExpo( double x ) {
    return x == 0 ? 0 : std::pow( 2, 10 * x - 10 );
}

inline static double easeOutExpo( double x ) {
    return x == 1 ? 1 : 1 - std::pow( 2, -10 * x );
}

inline double easeInOutExpo( double x ) {
    if( x == 0 ) {
        return 0;
    } else if( x == 1 ) {
        return 1;
    } else if( x < 0.5 ) {
        return std::pow( 2, 20 * x - 10 ) / 2;
    } else {
        return (2 - std::pow( 2, -20 * x + 10 )) / 2;
    }
}

inline static double easeInCirc( double x ) {
    return 1 - std::sqrt( 1 - std::pow( x, 2 ) );
}

inline static double easeOutCirc( double x ) {
    return std::sqrt( 1 - std::pow( x - 1, 2 ) );
}

inline static double easeInOutCirc( double x ) {
    if( x < 0.5 ) {
        return (1 - std::sqrt( 1 - std::pow( 2 * x, 2 ) )) / 2;
    } else {
        return (std::sqrt( 1 - std::pow( -2 * x + 2, 2 ) ) + 1) / 2;
    }
}

inline static double easeInBack( double x ) {
    const double c1 = 1.70158;
    const double c3 = c1 + 1;
    return c3 * x * x * x - c1 * x * x;
}

inline static double easeOutBack( double x ) {
    const double c1 = 1.70158;
    const double c3 = c1 + 1;
    return 1 + c3 * std::pow( x - 1, 3 ) + c1 * std::pow( x - 1, 2 );
}

inline static double easeInOutBack( double x ) {
    const double c1 = 1.70158;
    const double c2 = c1 * 1.525;
    if( x < 0.5 ) {
        return (std::pow( 2 * x, 2 ) * ((c2 + 1) * 2 * x - c2)) / 2;
    } else {
        return (std::pow( 2 * x - 2, 2 ) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
    }
}

double easeInElastic( double t ) {
    double t2 = t * t;
    return t2 * t2 * sin( t * PI * 4.5 );
}

double easeOutElastic( double t ) {
    double t2 = (t - 1) * (t - 1);
    return 1 - t2 * t2 * cos( t * PI * 4.5 );
}

double easeInOutElastic( double t ) {
    double t2;
    if( t < 0.45 ) {
        t2 = t * t;
        return 8 * t2 * t2 * sin( t * PI * 9 );
    } else if( t < 0.55 ) {
        return 0.5 + 0.75 * sin( t * PI * 4 );
    } else {
        t2 = (t - 1) * (t - 1);
        return 1 - 8 * t2 * t2 * sin( t * PI * 9 );
    }
}

double easeInBounce( double t ) {
    return pow( 2, 6 * (t - 1) ) * abs( sin( t * PI * 3.5 ) );
}

double easeOutBounce( double t ) {
    return 1 - pow( 2, -6 * t ) * abs( cos( t * PI * 3.5 ) );
}

double easeInOutBounce( double t ) {
    if( t < 0.5 ) {
        return 8 * pow( 2, 8 * (t - 1) ) * abs( sin( t * PI * 7 ) );
    } else {
        return 1 - 8 * pow( 2, -8 * t ) * abs( sin( t * PI * 7 ) );
    }
}

inline void add_module_easing(VM* vm){
    PyObject* mod = vm->new_module("easing");

#define EASE(name)  \
    vm->bind_func<1>(mod, "Ease"#name, [](VM* vm, ArgsView args){  \
        f64 t = CAST(f64, args[0]); \
        return VAR(ease##name(t));   \
    });

    EASE(InSine)
    EASE(OutSine)
    EASE(InOutSine)
    EASE(InQuad)
    EASE(OutQuad)
    EASE(InOutQuad)
    EASE(InCubic)
    EASE(OutCubic)
    EASE(InOutCubic)
    EASE(InQuart)
    EASE(OutQuart)
    EASE(InOutQuart)
    EASE(InQuint)
    EASE(OutQuint)
    EASE(InOutQuint)
    EASE(InExpo)
    EASE(OutExpo)
    EASE(InOutExpo)
    EASE(InCirc)
    EASE(OutCirc)
    EASE(InOutCirc)
    EASE(InBack)
    EASE(OutBack)
    EASE(InOutBack)
    EASE(InElastic)
    EASE(OutElastic)
    EASE(InOutElastic)
    EASE(InBounce)
    EASE(OutBounce)
    EASE(InOutBounce)

#undef EASE
}


} // namespace pkpy
