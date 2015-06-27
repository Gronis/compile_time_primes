#include <bitset>
#include <iostream>
#include <cassert>
#include <type_traits>

template<bool condition,typename T, T if_true, T if_false>
struct if_;

template<typename T, T if_true, T if_false>
struct if_<true, T, if_true, if_false>{
    static const T result = if_true;
};


template<typename T, T if_true, T if_false>
struct if_<false, T, if_true, if_false>{
    static const T result = if_false;
};

//Helper. creates completime array
template<bool... values>
struct bool_array{
    static const bool value[sizeof...(values)];
};
template<bool... values> 
bool const bool_array<values...>::value[] = {values ...};

template<typename...> 
struct iterate; 

// End, when array is empty
template<template<bool...> class array_, bool... new_values>
struct iterate<array_<new_values...>, array_<>> : bool_array<new_values...>{};

// itarate
template<template<bool...> class array_, bool old_value, bool... new_values, bool... old_values>
struct  iterate<array_<new_values...>, array_<old_value, old_values...>> : 
        iterate<array_<old_value, new_values...> , array_<old_values...>>{};

//start with empty
template<template<bool...> class array_, bool old_value, bool... old_values>
struct  iterate<array_<old_value, old_values...>> : 
        iterate<array_<old_value> , array_<old_values...>>{};

template<bool new_value, size_t interval, size_t interval_left, typename...> 
struct replace_rec; 

template<bool new_value, size_t interval, typename...> 
struct replace_; 

// End, when array is empty
template<bool new_value, size_t interval, size_t interval_left, template<bool...> class array_, bool... new_values>
struct replace_rec<new_value, interval, interval_left, array_<new_values...>, array_<>> : bool_array<new_values...>{};

// itarate
template<bool new_value, size_t interval, size_t interval_left, template<bool...> class array_, bool old_value, bool... new_values, bool... old_values>
struct  replace_rec<new_value, interval, interval_left, array_<new_values...>, array_<old_value, old_values...>> : 
        replace_rec<
            new_value,
            interval, 
            if_<interval_left == 0, size_t, 
                interval, 
            //else
                interval_left - 1>::result,
            array_<new_values..., if_<interval_left == 0, size_t, new_value, old_value>::result> , array_<old_values...>>{};

//wrapper for more easy start call
template<bool new_value, size_t interval, template<bool...> class array_, bool old_value, bool... old_values>
struct  replace_<new_value, interval, array_<old_value, old_values...>> : 
        replace_rec<new_value, interval, interval, array_<new_value> , array_<old_values...>>{};

//Generate an array of N booleans filled with value
template<int N, bool value, bool... values> struct bool_generator;
// End when N reaches 0
template<bool value, bool... values> struct bool_generator<0, value, values...> : bool_array<values...> {
    typedef bool_array<values...> type;
};
// Recursion
template<int N, bool value, bool... values> struct bool_generator : bool_generator<N - 1, value , value , values...> {};

template<int max, bool value>
bool generate_bool_array(int index){
    return iterate<typename bool_generator<max, value>::type>::value[index];
}

template<int max, bool gen_value, bool replace_value>
bool replace_bool_array(int index){
    return replace_<replace_value, 3, typename bool_generator<max, gen_value>::type>::value[index];
}

int main(){
    for (int i = 0; i < 10; ++i)
    {
        std::cout << generate_bool_array<10, true>(i) << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < 10; ++i)
    {
        std::cout << replace_bool_array<10, true, false>(i) << " ";
    }
    std::cout << std::endl;

}