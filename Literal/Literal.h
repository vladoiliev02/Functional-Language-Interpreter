#pragma once

#include <list>
#include <iostream>

/**
 * @brief - Enum with the types of the literals
 */
enum class LITERAL_TYPE {
    DOUBLE,
    LIST
};

class List;

/**
 * @brief - Interface for the literal types
 */
class Literal {
public:
    using list_type = std::list<Literal*>;

protected:
    virtual void print(std::ostream& os) const = 0;

public:
    virtual double           get_double()                     const =       0;
    virtual const list_type& get_list()                       const =       0;
    virtual const Literal*   head()                           const =       0;
    virtual List             tail()                           const =       0;
    virtual int              length()                         const =       0;
    virtual double           get_step()                       const =       0;
    virtual List             to_list()                        const =       0;
    virtual double           to_double()                      const =       0;
    virtual List             concat(const List& other)        const =       0;
    virtual LITERAL_TYPE     get_type()                       const =       0;
    virtual                  ~Literal()                             = default;
    virtual bool             operator==(const Literal& other) const =       0;
    virtual bool             operator!=(const Literal& other) const =       0;
    virtual operator         bool()                           const =       0;

    friend  std::ostream& operator<<(std::ostream& os, const Literal& literal);
};

/**
 * @brief - Literal of type double
 */
class Double : public Literal {
    double value;

protected:
    void print(std::ostream& os) const override;

public:
    Double(double value);

    double           get_double()                     const override;
    const list_type& get_list()                       const override;
    const Literal*   head()                           const override;
    List             tail()                           const override;
    int              length()                         const override;
    double           get_step()                       const override;
    List             to_list()                        const override;
    double           to_double()                      const override;
    List             concat(const List& other)        const override;
    LITERAL_TYPE     get_type()                       const override;
    bool             operator==(const Literal& other) const override;
    bool             operator!=(const Literal& other) const override;
    operator         bool()                           const override;
};

/**
 * @brief - Literal of type List
 */
class List : public Literal {
    list_type list{};
    double    step      =  1;

    /// Max size of -1 indicates infinite list
    int       max_size  = -1;

protected:
    void print(std::ostream& os) const override;

public:
    List();
    List(const List& other);
    List(List&& other);
    List(const list_type& list);
    List(double initial_value, double step = 1.0, int max_size = -1);
    ~List();

    double           get_double()                     const override;
    const list_type& get_list()                       const override;
    const Literal*   head()                           const override;
    List             tail()                           const override;
    int              length()                         const override;
    double           get_step()                       const override;
    List             to_list()                        const override;
    double           to_double()                      const override;
    List             concat(const List& other)        const override;
    LITERAL_TYPE     get_type()                       const override;
    bool             operator==(const Literal& other) const override;
    bool             operator!=(const Literal& other) const override;
    operator bool() const override;
};



