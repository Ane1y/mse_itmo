#include "typeset.hpp"

#include <cassert>
#include <type_traits>

using empty = cls03::typeset<>;
using ti = cls03::typeset<int>;
using tilc = cls03::typeset<int,long,char>;
using tcli = cls03::typeset<char,long,int>;
using tcls = cls03::typeset<char,long,short>;
using tilcs = cls03::typeset<int,long,char,short>;
using tcl = cls03::typeset<char,long>;
using tis = cls03::typeset<int,short>;

// если у вас не реализована какая-то функция, можете просто поменять 1 на 0
// чтобы не отвлекаться на ошибки компиляции

#define HAS__IS_EMPTY 1
#define HAS__SIZE 1
#define HAS__GET 1
#define HAS__BELONGS_TO 1  // эта функция нужна для тестов join/cross/subtract
#define HAS__JOIN 1
#define HAS__CROSS 1
#define HAS__SUBTRACT 1
#define HAS__VARIANT 1

////////////
// is_empty

#if HAS__IS_EMPTY
static_assert(cls03::is_empty<empty>);
static_assert(!cls03::is_empty<ti>);
static_assert(!cls03::is_empty<tilc>);
#endif

////////
// size

#if HAS__SIZE
static_assert(cls03::size<empty> == 0);
static_assert(cls03::size<ti> == 1);
static_assert(cls03::size<tilc> == 3);
#endif

///////
// get

#if HAS__GET
static_assert(std::is_same_v<cls03::get<0, ti>, int>);
static_assert(std::is_same_v<cls03::get<0, tilc>, int>);
static_assert(std::is_same_v<cls03::get<1, tilc>, long>);
static_assert(std::is_same_v<cls03::get<2, tilc>, char>);
#endif

//////////////
// belongs_to

#if HAS__BELONGS_TO
static_assert(!cls03::belongs_to<int, empty>);
static_assert(!cls03::belongs_to<char, ti>);
static_assert(cls03::belongs_to<int, ti>);
static_assert(!cls03::belongs_to<short, tilc>);
static_assert(cls03::belongs_to<int, tilc>);
static_assert(cls03::belongs_to<long, tilc>);
static_assert(cls03::belongs_to<char, tilc>);
#endif

//////////////
// comparison

#if HAS__BELONGS_TO
template<class TS1, class TS2> struct equal_sets;
template<class... Xs, class... Ys> struct equal_sets<cls03::typeset<Xs...>, cls03::typeset<Ys...>> :
    std::bool_constant<
        (cls03::belongs_to<Xs, cls03::typeset<Ys...>> && ... && true)
        &&
        (cls03::belongs_to<Ys, cls03::typeset<Xs...>> && ... && true)
    > {};
template<class TS1, class TS2> static constexpr bool equal_sets_v = equal_sets<TS1, TS2>::value;

static_assert(equal_sets_v<empty, empty>);
static_assert(equal_sets_v<tilc, tcli>);
static_assert(!equal_sets_v<tilc, tcls>);
#endif

////////
// join

#if HAS__BELONGS_TO && HAS__JOIN
static_assert(equal_sets_v<cls03::join<empty, empty>, empty>);
static_assert(equal_sets_v<cls03::join<empty, ti>, ti>);
static_assert(equal_sets_v<cls03::join<ti, empty>, ti>);

static_assert(equal_sets_v<cls03::join<tilc, ti>, tcli>);
static_assert(equal_sets_v<cls03::join<tilc, tcls>, tilcs>);
#endif

/////////
// cross

#if HAS__BELONGS_TO && HAS__CROSS
static_assert(equal_sets_v<cls03::cross<empty, empty>, empty>);
static_assert(equal_sets_v<cls03::cross<tilc, tcli>, tilc>);
static_assert(equal_sets_v<cls03::cross<tilc, tcls>, tcl>);
#endif

////////////
// subtract

#if HAS__BELONGS_TO && HAS__SUBTRACT
static_assert(equal_sets_v<cls03::subtract<empty, tilcs>, empty>);
static_assert(equal_sets_v<cls03::subtract<tilc, tcli>, empty>);
static_assert(equal_sets_v<cls03::subtract<tilcs, tcl>, tis>);
#endif

///////////
// variant

void test_variant() {
#if HAS__VARIANT
    using vilc = cls03::variant_of<tilc>;
    vilc v;
    v = 1;
    assert(std::get<int>(v) == 1);
    v = 2L;
    assert(std::get<long>(v) == 2L);
    v = '3';
    assert(std::get<char>(v) == '3');
#endif
}

int main() {
    test_variant();
}