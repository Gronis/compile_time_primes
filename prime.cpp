#include <iostream>
#include <cassert>
#include <string>

/////////////////////////////////////////
///
/// if_
///         check condition and returns
///         different values accordingly.
///         
/////////////////////////////////////////
template<bool condition, unsigned if_true, unsigned if_false>
struct if_;

template<unsigned if_true, unsigned if_false>
struct if_<true, if_true, if_false>{
    enum{ result = if_true };
};

template<unsigned if_true, unsigned if_false>
struct if_<false, if_true, if_false>{
    enum{ result = if_false };
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
template<   bool new_value, 
            size_t interval, 
            size_t interval_left, 
            typename...> 
struct replace_rec; 

template<   bool new_value, 
            size_t interval, 
            typename...> 
struct replace_; 

// End, when array is empty
template<   bool new_value, 
            size_t interval, 
            size_t interval_left, 
            template<bool...> class array_, 
            bool... new_values>
struct  replace_rec<new_value, interval, interval_left, 
            array_<new_values...>, 
            array_<>>{
    typedef bool_array<new_values...> type;
};

// Recursive call to replace value.
template<   bool new_value, 
            size_t interval, 
            size_t interval_left, 
            template<bool...> class array_, 
            bool old_value, 
            bool... new_values, 
            bool... old_values>
struct  replace_rec<new_value, interval, interval_left, 
            array_<new_values...>, array_<old_value, old_values...>> : 
        replace_rec<new_value, interval, 
            //if current value should be replaced, replace it
            if_<interval_left == 1, 
                interval, 
            //else
                interval_left - 1>::result,
            //endif
            array_<new_values..., 
                if_<interval_left == 1, 
                    new_value, 
                //else
                    old_value>::result> ,
                //endif
                array_<old_values...>
            >{};

//wrapper for more easy start call
template<   bool new_value, 
            size_t interval, 
            template<bool...> class array_, 
            bool old_value, 
            bool... old_values>
struct  replace_<new_value, interval, 
            array_<old_value, old_values...>> : 
        replace_rec<new_value, interval, interval - 1, 
            array_<old_value> , array_<old_values...>>{};

//wrapper without elements
template<   bool new_value, 
            size_t interval, 
            template<bool...> class array_>
struct  replace_<new_value, interval, array_<>> {
    typedef bool_array<> type;
};

/////////////////////////////////////////////////////////////////
///
/// bool_generator
///             Create a compile-time boolean array filled with
///             values.
///
/////////////////////////////////////////////////////////////////
template<   int N, 
            bool value, 
            bool... values> 
struct bool_generator;

// End when N reaches 0
template<   bool value, 
            bool... values> 
struct bool_generator<0, value, values...> {
    typedef bool_array<values...> type;
};
// Recursive inheritance to generate all values
template<   int N, 
            bool value, 
            bool... values> 
struct  bool_generator : 
        bool_generator<N - 1, value , values..., value> {};

///////////////////////////////////////////////////////////////////
///
/// calc_prime_array
///             generates a compiletime bool array with each index
///             set to whenever that number is a prime or not.
///
///////////////////////////////////////////////////////////////////
template<   size_t N, 
            size_t current, 
            typename...> 
struct calc_prime_array;

// Recursive inheritance to generate prime array
template<   size_t N, 
            size_t current, 
            template<bool...> class array_, 
            bool input_value, 
            bool... input_values, 
            bool... output_values> 
struct  calc_prime_array<N, current, 
            array_<input_value, input_values...>, 
            array_<output_values...>> : 
        calc_prime_array<N, current + 1, typename replace_<false, 
        //if input value is prime, set all values divisable with prime as a non-prime
        if_<input_value, 
            current, 
        //else
            N>::result, 
        //endif
        array_<input_values...>>::type,             //replace every dividable input with current_prime
        array_<output_values..., input_value> > {}; //place input value with the finished array

//When current == N, we have reached the end, stop
template<   size_t N, 
            template<bool...> class array_, 
            bool... output_values> 
struct calc_prime_array<N, N, array_<>, array_<output_values...>> {
    typedef bool_array<output_values...> type;
};

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
    assert(number < max);
    return calc_prime_array<max, 2, typename 
        bool_generator<max - 2, true>::type, bool_array<false, false>>
        ::type::value[number];
}

int main(){
    const int max = 200;
    /*
    for (int i = 0; i < max; ++i)
    {
        std::cout << replace_<false, 2, typename bool_generator<max, true>::type>::type::value[i] << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < max; ++i)
    {
        if(is_prime<max>(i)) std::cout << i << std::endl;
    }
    std::cout << std::endl;
    */
    std::string input = "";
    while(input != "q") {
        std::cout << "Check if number is prime (max " << max << "): ";
        std::cin >> input;
        int value = atoi(input.c_str());
        std::cout 
            << (input == "q"? "goodbye" : is_prime<max>(value) == false? "no" : "yes") 
            << std::endl;
    }
}