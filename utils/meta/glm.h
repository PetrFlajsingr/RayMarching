//
// Created by Petr on 18.11.2019.
//

#ifndef UTILITIES_GLM_H
#define UTILITIES_GLM_H

#include "glm/glm.hpp"

template <class T, template <class, glm::precision> class Template> struct is_vec_specialization : std::false_type {};
template <template <class, glm::precision> class Template, typename ValueType, glm::precision P>
struct is_vec_specialization<Template<ValueType, P>, Template> : std::true_type {};

template <typename T> constexpr bool is_vec_specialisation_v = is_vec_specialization<std::decay_t<T>, glm::tvec4>::value ||
                                                               is_vec_specialization<std::decay_t<T>, glm::tvec3>::value ||
                                                               is_vec_specialization<std::decay_t<T>, glm::tvec2>::value;

#endif // UTILITIES_GLM_H
