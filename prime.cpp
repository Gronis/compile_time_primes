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

template<bool new_value, typename...> 
struct replace_; 

// End, when array is empty
template<bool new_value, template<bool...> class array_, bool... new_values>
struct replace_<new_value, array_<new_values...>, array_<>> : bool_array<new_values...>{};

// itarate
template<bool new_value, template<bool...> class array_, bool old_value, bool... new_values, bool... old_values>
struct  replace_<new_value, array_<new_values...>, array_<old_value, old_values...>> : 
        replace_<new_value, array_<new_value, new_values...> , array_<old_values...>>{};

//start with empty
template<bool new_value, template<bool...> class array_, bool old_value, bool... old_values>
struct  replace_<new_value, array_<old_value, old_values...>> : 
        replace_<new_value, array_<new_value> , array_<old_values...>>{};

//Generate an array of N booleans filled with value
template<int N, bool value, bool... values> struct bool_generator;
// End when N reaches 0
template<bool value, bool... values> struct bool_generator<0, value, values...> : bool_array<values...> {
    typedef bool_array<values...> type;
};
// Recursion
template<int N, bool value, bool... values> struct bool_generator : bool_generator<N - 1, value , value , values...> {};

/*/fill each n-th element with value
template<bool new_value, 
        size_t steps, 
        size_t current_step, 
        template<bool...> class new_array, 
        bool... values> struct fill; 

template<bool new_value, size_t steps, size_t current_step, template<bool...> class new_array, bool... new_values, bool... old_values>
struct fill<new_value, steps, current_step, new_array<new_values...>, old_values...> : 
    bool_array<new_values...>{}; 

template<bool new_value, size_t steps, size_t current_step, template<bool...> class new_array, bool old_value, bool... new_values, bool... old_values>
struct fill<new_value, steps, current_step, template<new_values...> class new_array, old_value, old_values...> : 
    fill<new_value, steps, if_<current_step == 0, size_t, steps, current_step - 1>::result, 
    new_array<if_<current_step == 0, bool, new_value, old_value>::result, new_values...>, old_values...>{
};*/
//template<bool new_value, size_t steps, size_t current_step, size_t N, bool old_value, bool...values>
//const bool fill<new_value, steps, current_step, N, old_value, values...>::array[N] = 
//{result, fill<new_value, steps, if_<current_step == 0, size_t, steps, current_step - 1>::result, N, values...>::result };

/*
template<size_t N, size_t i, bool_array<N> primes>
struct calc_prime;

template<size_t N, bool_array<N> primes>
struct calc_prime<N, 2, primes>{
    static const bool[N] result = ;
};

template<size_t N, size_t i, bool_array<N> primes>
struct calc_prime
{
    const static bool[N] result = 
};


bool is_prime(size_t num){
    static prime_list primes = calc_prime<prime_list>::result;
    assert(num < max_num);
    return primes[num];
}
*/
template<int max, bool value>
bool generate_bool_array(int index){
    return iterate<typename bool_generator<max, value>::type>::value[index];
}

template<int max, bool gen_value, bool replace_value>
bool replace_bool_array(int index){
    return replace_<replace_value, typename bool_generator<max, gen_value>::type>::value[index];
}

int main(){
    for (int i = 0; i < 100; ++i)
    {
        std::cout << generate_bool_array<100, true>(i) << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < 100; ++i)
    {
        std::cout << replace_bool_array<100, true, true>(i) << " ";
    }
    std::cout << std::endl;

}