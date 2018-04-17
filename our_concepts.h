//Concept definitions for Graph Explorer

//concept definition for == operator support
template<typename T>
concept bool EqualityComparable = requires(T a, T b) {
    { a == b } -> bool;
};

//concept definition for less than operator support
template<typename T>
concept bool LessThanComparable = requires(T a, T b) {
    { a < b } -> bool;
} && EqualityComparable<T>;