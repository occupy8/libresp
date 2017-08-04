

enum class state {
    st_start = 0,

    st_string = 1,
    st_string_lf = 2,

    st_error_string = 3,
    st_error_lf = 4,

    st_integer = 5,
    st_integer_lf = 6,

    st_bulk_size = 7,
    st_bulk_size_lf = 8,
    st_bulk = 9,
    st_bulk_cr = 10,
    st_bulk_lf = 11,

    st_array_size = 12,
    st_array_size_lf = 13,
 };
