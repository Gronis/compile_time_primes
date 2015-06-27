#include <iostream>

/////////////////////////////////////////
///
/// if_
///         check condition and returns
///         different values accordingly.
///         
/////////////////////////////////////////
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
/////////////////////////////////////////
///
/// bool_array_
///             creates completime array
///             with runtime accessable
///             elements.
///
/////////////////////////////////////////
template<bool... values>
struct bool_array{
    static const bool value[sizeof...(values)];
};
template<bool... values> 
bool const bool_array<values...>::value[] = {values ...};

/////////////////////////////////////////
///
/// get_index_
///             access array elements 
///             compile-time.
///
/////////////////////////////////////////
template<size_t index, size_t start, typename...>
struct get_index;

template<size_t index, template<bool...> class array_, bool value, bool... values>
struct get_index<index, index, array_<value, values...>>{
    static const bool result = value;
};

template<size_t index, size_t start, template<bool...> class array_, bool value, bool... values>
struct get_index<index, start, array_<value, values...>> : get_index<index, start + 1, array_<values...>>{};

/////////////////////////////////////////////////////////////////
///
/// replace_
///             Replace elements with value after each interval.
///             First replacement starts after interval * 2.
///
///             Used for setting all values that are divideable
///             with a prime number as a non prime number
///
/////////////////////////////////////////////////////////////////
template<bool new_value, size_t interval, size_t interval_left, typename...> 
struct replace_rec; 

template<bool new_value, size_t interval, typename...> 
struct replace_; 

// End, when array is empty
template<bool new_value, size_t interval, size_t interval_left, template<bool...> class array_, bool... new_values>
struct replace_rec<new_value, interval, interval_left, array_<new_values...>, array_<>> : bool_array<new_values...>{
    typedef bool_array<new_values...> type;
};

// Recursive call to replace value.
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

/////////////////////////////////////////////////////////////////
///
/// bool_generator
///             Create a compile-time boolean array filled with
///             values.
///
/////////////////////////////////////////////////////////////////
template<int N, bool value, bool... values> struct bool_generator;
// End when N reaches 0
template<bool value, bool... values> struct bool_generator<0, value, values...> : bool_array<values...> {
    typedef bool_array<values...> type;
};
// Recursive inheritance to generate all values
template<int N, bool value, bool... values> struct bool_generator : bool_generator<N - 1, value , value , values...> {};

///////////////////////////////////////////////////////////////////
///
/// calc_prime_array
///             generates a compiletime bool array with each index
///             set to whenever that number is a prime or not.
///
///////////////////////////////////////////////////////////////////
template<size_t N, size_t current, typename...> struct calc_prime_array;

// Recursive inheritance to generate prime array
template<size_t N, size_t current, template<bool...> class array_, bool... values> 
struct calc_prime_array<N, current, array_<values...>> : calc_prime_array<N, current + 1, typename replace_<false, 
    if_<get_index<current, 0, bool_array<values...>>::result, size_t, current, N>::result, array_<values...>>::type > {};

//When current == N, we have reached the end
template<size_t N, template<bool...> class array_, bool... values> 
struct calc_prime_array<N, N, array_<values...>> : bool_array<values...>{};


///////////////////////////////////////////////////////////////////
///
/// is_prime
///             generates a compiletime bool array with the size of
///             max, and evalute witch are primes or not
///             (compile-time).
///
/// USAGE:      is_prime(i) //i should be less than 200
///             is_prime<max>(i) // i should be less than max
///
///////////////////////////////////////////////////////////////////
template<size_t max = 200>
bool is_prime(size_t number){
    return calc_prime_array<max, 2, typename bool_generator<max, true>::type>::value[number];
}

int main(){
    const int max = 50;
    for (int i = 0; i < max; ++i)
    {
        if(is_prime(i)) std::cout << i << std::endl;
    }
    std::cout << std::endl;
}