#ifndef CLASS_MEMBER_FUNCTIONS_HPP
#define CLASS_MEMBER_FUNCTIONS_HPP

#define ALL_FUNC(a)

#define NO_DC_FUNCs(className)                                          \
    ~className () = default;                                             \
                                                                        \
    className (const className & other) noexcept = default;              \
    className ( className && other) noexcept = default;                   \
                                                                        \
    className& operator=( const className & other) noexcept = default;    \
    className& operator=( className && other) noexcept = default;

#define NO_DC_VD_FUNCs(className)                                       \
    virtual ~className () = default;                                     \
                                                                        \
    className (const className & other) noexcept = default;              \
    className ( className && other) noexcept = default;                   \
                                                                        \
    className& operator=(const className & other) noexcept = default;    \
    className& operator=( className && other) noexcept = default;

#define NOC__NOD_FUNCs(className)                                       \
    className (const className & other) noexcept = default;              \
    className ( className && other) noexcept = default;                   \
                                                                        \
    className& operator=(const className & other) noexcept = default;    \
    className& operator=( className && other) noexcept = default;

#endif // !CLASS_MEMBER_FUNCTIONS_HPP
