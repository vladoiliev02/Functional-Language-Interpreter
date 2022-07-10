#include "Literal.h"

#include <stdexcept>
#include <cassert>

///------------------DOUBLE--------------------------


Double::Double(double value)
        : value(value)
{}

double Double::get_double() const
{
    return value;
}

const Literal::list_type& Double::get_list() const
{
    throw std::invalid_argument("Literal :: get_list() -> Incorrect type, actual type is Double.");
}

const Literal* Double::head() const
{
    throw std::invalid_argument("Literal :: head() -> Incorrect type, actual type is Double.");
}

List Double::tail() const
{
    throw std::invalid_argument("Literal :: tail() -> Incorrect type, actual type is Double.");
}

int Double::length() const
{
    throw std::invalid_argument("Literal :: length() -> Incorrect type, actual type is Double.");
}

double Double::get_step() const
{
    throw std::invalid_argument("Literal ::  get_step() -> Incorrect type, actual type is Double.");
}

void Double::print(std::ostream& os) const
{
    os << value;
}

List Double::to_list() const
{
    return {value, 0, 1};
}

List Double::concat(const List& other) const
{
    throw std::invalid_argument("Literal :: concat() -> Incorrect type, actual type is Double.");
}

double Double::to_double() const
{
    return value;
}

LITERAL_TYPE Double::get_type() const
{
    return LITERAL_TYPE::DOUBLE;
}

bool Double::operator==(const Literal& other) const
{
    if (other.get_type() == LITERAL_TYPE::DOUBLE)
        return std::abs(value - other.get_double()) < 0.00001;
    else
        return other.length() == 1 && value == other.head()->get_double();
}

bool Double::operator!=(const Literal& other) const
{
    return !(*this == other);
}

Double::operator bool() const
{
    return std::abs(value) >= 0.00001;
}

///------------------LIST--------------------------

List::List(const List& other)
        : list{},
          step(other.step),
          max_size(other.max_size)
{
    for (Literal* el : other.list) {
        switch (el->get_type()) {
            case LITERAL_TYPE::DOUBLE :{
                list.push_back(new Double(el->get_double()));
                break;
            }
            case LITERAL_TYPE::LIST :{
                list.push_back(new List(el->get_list()));
                break;
            }
        }
    }
}


List::List(const list_type& list)
        : list{},
          max_size(list.size())
{
    for (Literal* el : list) {
        switch (el->get_type()) {
            case LITERAL_TYPE::DOUBLE :{
                this->list.push_back(new Double(el->get_double()));
                break;
            }
            case LITERAL_TYPE::LIST :{
                this->list.push_back(new List(el->get_list()));
                break;
            }
        }
    }
}

List::List(double initial_value, double step, int max_size)
    : list(),
      step(step),
      max_size(max_size)
{
    int max = max_size == -1 ? 10 : max_size;
    if (max_size != 0)
        list.push_back(new Double(initial_value));

    for (int i = 1; i < max; ++i)
        list.push_back(new Double(list.back()->get_double() + step));
}

double List::get_double() const
{
    throw std::invalid_argument("Literal :: get_double() -> Incorrect type, actual type is List.");
}

const Literal::list_type& List::get_list() const
{
    return list;
}

const Literal* List::head() const
{
    if (list.empty())
        throw std::invalid_argument("List :: head() on empty list.");

    return list.front();
}

List List::tail() const
{
    if (list.empty())
        throw std::invalid_argument("List :: tail() on empty list.");

    List res = *this;
    res.list.pop_front();

    if (max_size == -1)
        res.list.push_back(new Double(list.back()->get_double() + step));

    return res;
}

int List::length() const
{
    return max_size == -1 ? -1 : (int)list.size();
}

double List::get_step() const
{
    return step;
}

void List::print(std::ostream& os) const
{
    if (list.empty()) {
        os << "[]";
    } else {
        auto end = list.end();

        os << '[' << *list.front();
        for (auto it = ++list.begin(); it != end; ++it)
            os << ' ' << *(*it);
        os << (max_size == -1 ? " ...]" : "]");
    }
}

List List::to_list() const
{
    return *this;
}

List::List()
    : step(0),
      max_size(0)
{}

List List::concat(const List& other) const
{
    if (max_size == -1)
        throw std::invalid_argument("List :: cannot concatenate to an endless list");

    list_type result = list;

    for (Literal* el : other.list)
        result.push_back(el);

    List new_list(result);

    if (other.max_size == -1) {
        new_list.max_size = -1;
        new_list.step = other.step;
    }

    return new_list;
}

double List::to_double() const
{
    if (list.size() != 1)
        throw std::invalid_argument("List :: Only lists with one member can be converted to double");
    return list.front()->get_double();
}

LITERAL_TYPE List::get_type() const
{
    return LITERAL_TYPE::LIST;
}

List::List(List&& other)
    : list(std::move(other.list)),
      step(other.step),
      max_size(other.max_size)
{}

List::~List()
{
    for (Literal* el : list)
        delete el;
}

bool List::operator==(const Literal& other) const
{
    if (other.get_type() == LITERAL_TYPE::DOUBLE)
        /// Causes compiler warning,
        /// but actually this calls the (==) method of Double.
        return other == *this;

    if (length() != other.length())
        return false;

    auto aend = list.end();
    for (auto ait = list.begin(), bit = ((const List&)other).list.begin();
         ait != aend;
         ++ait, ++bit)
    {
        if (*(*ait) != *(*bit))
            return false;
    }

    return true;
}

bool List::operator!=(const Literal& other) const
{
    return !(*this == other);
}

List::operator bool() const
{
    return !list.empty();
}

std::ostream& operator<<(std::ostream& os, const Literal& literal)
{
    literal.print(os);
    return os;
}
