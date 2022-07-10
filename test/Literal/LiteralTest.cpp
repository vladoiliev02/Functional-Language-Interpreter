#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "Literal.h"
#include <stdexcept>

TEST_CASE("Test Double")
{
    Double d = 3.14;

    REQUIRE(std::abs(d.get_double() - 3.14) < 0.001);

    REQUIRE_THROWS_AS(d.get_list(), std::invalid_argument);
    REQUIRE_THROWS_AS(d.head(), std::invalid_argument);
    REQUIRE_THROWS_AS(d.tail(), std::invalid_argument);
    REQUIRE_THROWS_AS(d.length(), std::invalid_argument);
    REQUIRE_THROWS_AS(d.get_step(), std::invalid_argument);
    REQUIRE_THROWS_AS(d.concat(List{}), std::invalid_argument);

    REQUIRE(d.get_type() == LITERAL_TYPE::DOUBLE);
}

TEST_CASE("Double operator==")
{
    const Literal& d = Double(3.14);
    const Literal& d2 = Double(3.14);

    const Literal& l1 =  List(3.14, 0, 1);
    const Literal& l2 =  List(3.14, 1, 5);

    REQUIRE(d == d2);
    REQUIRE(d == l1);
    REQUIRE_FALSE(d == l2);
}

TEST_CASE("Double to_list()")
{
    Double d = 3.14;

    const Literal& lst = d.to_list();

    REQUIRE(d == lst);
    REQUIRE(lst == List(3.14,0,1));
}

/// ------------------ List Tests -----------------------

TEST_CASE("List default ctor")
{
    List l1;

    REQUIRE_THROWS_AS(l1.get_double(), std::invalid_argument);
    REQUIRE_THROWS_AS(l1.head(), std::invalid_argument);
    REQUIRE_THROWS_AS(l1.tail(), std::invalid_argument);
    REQUIRE(l1.length() == 0);
    REQUIRE(std::abs(l1.get_step() - 0) < 0.0001);
    REQUIRE(l1 == l1.to_list());
    REQUIRE_THROWS_AS(l1.to_double(), std::invalid_argument);
    REQUIRE(List{3.14, 1, 5} == l1.concat(List{3.14, 1, 5}));
    REQUIRE(LITERAL_TYPE::LIST == l1.get_type());
}

TEST_CASE("List copy ctor")
{
    List l(1, 1, 10);
    List l1 = l;

    REQUIRE_THROWS_AS(l1.get_double(), std::invalid_argument);
    REQUIRE((*l1.head() == *l.head() && *l.head() == Double(1)));
    REQUIRE((l1.tail() == l.tail() && l1.tail() == List(2,1,9)));
    REQUIRE((l1.length() == l.length() && l.length() == 10));
    REQUIRE((std::abs(l1.get_step() - 1) < 0.0001 && std::abs(l.get_step() - 1) < 0.001));
    REQUIRE(l == l.to_list());
    REQUIRE_THROWS_AS(l.to_double(), std::invalid_argument);
    REQUIRE(l == l1);
    REQUIRE(l.concat(List{2,1,2}) == l1.concat(List{2,1,2}));
    REQUIRE(LITERAL_TYPE::LIST == l1.get_type());
}

TEST_CASE("List move ctor")
{
    List l(1, 1, 10);
    List l1 = std::move(l);

    REQUIRE_THROWS_AS(l1.get_double(), std::invalid_argument);
    REQUIRE(*l1.head() == Double(1));
    REQUIRE(l1.tail() == List(2,1,9));
    REQUIRE(l1.length() == 10);
    REQUIRE(std::move(l1.get_step() - 1 )< 0.00001);
    REQUIRE_THROWS_AS(l1.to_double(), std::invalid_argument);
    REQUIRE(l1 == List(1, 1, 10));
    REQUIRE(LITERAL_TYPE::LIST == l1.get_type());
}

TEST_CASE("List list_type ctor")
{
    List l1 = Literal::list_type({new Double(1),
                                  new Double(2),
                                  new Double(3),
                                  new Double(4),
                                  new Double(5)});

    REQUIRE_THROWS_AS(l1.get_double(), std::invalid_argument);
    REQUIRE(*l1.head() == Double(1));
    REQUIRE(l1.tail() == List(2,1,4));
    REQUIRE(l1.length() == 5);
    REQUIRE(std::abs(l1.get_step() - 1.0) < 0.001);
    REQUIRE_THROWS_AS(l1.to_double(), std::invalid_argument);
    REQUIRE(l1 == List(1, 1, 5));
    REQUIRE(LITERAL_TYPE::LIST == l1.get_type());
}

TEST_CASE("List intial_value ctor")
{
    List l1(3.14, 1, 10);

    REQUIRE_THROWS_AS(l1.get_double(), std::invalid_argument);
    REQUIRE(*l1.head() == Double(3.14));
    REQUIRE(l1.tail() == List(4.14,1,9));
    REQUIRE(l1.length() == 10);
    REQUIRE(std::abs(l1.get_step() - 1.0) < 0.001);
    REQUIRE_THROWS_AS(l1.to_double(), std::invalid_argument);
    REQUIRE(l1 == List(3.14, 1, 10));
    REQUIRE(LITERAL_TYPE::LIST == l1.get_type());
}