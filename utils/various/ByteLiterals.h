//
// Created by Petr on 18.11.2019.
//

#ifndef UTILITIES_BYTELITERALS_H
#define UTILITIES_BYTELITERALS_H

namespace ByteLiterals {
constexpr auto orderStep = 1024;
constexpr unsigned long long operator"" _B(unsigned long long value) { return value; }
constexpr unsigned long long operator"" _KB(unsigned long long value) { return value * orderStep; }
constexpr unsigned long long operator"" _MB(unsigned long long value) { return value * std::pow(orderStep, 2); }
constexpr unsigned long long operator"" _GB(unsigned long long value) { return value * std::pow(orderStep, 3); }
constexpr unsigned long long operator"" _TB(unsigned long long value) { return value * std::pow(orderStep, 4); }
constexpr unsigned long long operator"" _PB(unsigned long long value) { return value * std::pow(orderStep, 5); }

} // namespace ByteLiterals

#endif // UTILITIES_BYTELITERALS_H
