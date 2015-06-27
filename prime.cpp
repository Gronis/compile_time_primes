#include <iostream>

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

//access array elements compile-time
template<size_t index, size_t start, typename...>
struct get_index;

template<size_t index, template<bool...> class array_, bool value, bool... values>
struct get_index<index, index, array_<value, values...>>{
    static const bool result = value;
};

//special case when empty
//template<size_t index, template<bool...> class array_>
//struct get_index<index, index, array_<>>{
//    static const bool result = false;
//};

template<size_t index, size_t start, template<bool...> class array_, bool value, bool... values>
struct get_index<index, start, array_<value, values...>> : get_index<index, start + 1, array_<values...>>{};

template<typename...> 
struct iterate; 

// End, when array is empty
template<template<bool...> class array_, bool... new_values>
struct iterate<array_<new_values...>, array_<>> : bool_array<new_values...>{
    typedef bool_array<new_values...> type;
};

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
struct replace_rec<new_value, interval, interval_left, array_<new_values...>, array_<>> : bool_array<new_values...>{
    typedef bool_array<new_values...> type;
};

// itarate
template<bool new_value, size_t interval, size_t interval_left, template<bool...> class array_, bool old_value, bool... new_values, bool... old_values>
struct  replace_rec<new_value, interval, interval_left, array_<new_values...>, array_<old_value, old_values...>> : 
        replace_rec<
            new_value,
            interval, 
            if_<interval_left == 1, size_t, 
                interval, 
            //else
                interval_left - 1>::result,
            array_<new_values..., if_<interval_left == 1, size_t, new_value, old_value>::result> , array_<old_values...>>{};

//wrapper for more easy start call
template<bool new_value, size_t interval, template<bool...> class array_, bool old_value, bool... old_values>
struct  replace_<new_value, interval, array_<old_value, old_values...>> : 
        replace_rec<new_value, interval, interval * 2, array_<new_value> , array_<old_values...>>{};

//Generate an array of N booleans filled with value
template<int N, bool value, bool... values> struct bool_generator;
// End when N reaches 0
template<bool value, bool... values> struct bool_generator<0, value, values...> : bool_array<values...> {
    typedef bool_array<values...> type;
};
// Recursion
template<int N, bool value, bool... values> struct bool_generator : bool_generator<N - 1, value , value , values...> {};

//calc pi
template<size_t N, size_t current, typename...> struct calc_pi_array;

//Recursion 
template<size_t N, size_t current, template<bool...> class array_, bool... values> 
struct calc_pi_array<N, current, array_<values...>> : calc_pi_array<N, current + 1, typename replace_<false, 
    if_<get_index<current, 0, bool_array<values...>>::result, size_t, current, N>::result, array_<values...>>::type > {};

//end
template<size_t N, template<bool...> class array_, bool... values> 
struct calc_pi_array<N, N, array_<values...>> : bool_array<values...>{};

template<int max, bool value>
bool generate_bool_array(int index){
    return iterate<typename bool_generator<max, value>::type>::value[index];
}

template<int max, bool gen_value, bool replace_value>
bool replace_bool_array(int index){
    return replace_<replace_value, 3, typename bool_generator<max, gen_value>::type>::value[index];
}

template<size_t max>
bool is_prime(size_t number){
    return calc_pi_array<max, 2, typename bool_generator<max, true>::type>::value[number];
}

int main(){
    for (int i = 0; i < 10; ++i)
    {
        std::cout << "[" << i << "] = " << generate_bool_array<10, true>(i) << ", ";
    }
    std::cout << std::endl;
    for (int i = 0; i < 20; ++i)
    {
        std::cout << replace_bool_array<20, true, false>(i) << " ";
    }
    std::cout << std::endl;
    const int max = 50;
    for (int i = 0; i < max; ++i)
    {
        if(is_prime<max>(i)) std::cout << i << std::endl;
    }
    std::cout << std::endl;
}