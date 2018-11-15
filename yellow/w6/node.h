#pragma once

#include <string>
#include <memory>
#include "date.h"

enum Comparison {
    Less,
    LessOrEqual,
    Greater,
    GreaterOrEqual,
    Equal,
    NotEqual
};

enum LogicalOperation {
    Or,
    And
};

class Node {
public:
    virtual bool Evaluate(const Date &date, const string &event) const = 0;
};

class EmptyNode : public Node {
public:
    bool Evaluate(const Date &date, const string &event) const override;
};

class DateComparisonNode : public Node {
public:
    DateComparisonNode(const Comparison &comparison, const Date &date);

    bool Evaluate(const Date &date, const string &event) const override;

private:
    const Comparison _comparison;
    const Date _date;
};

class EventComparisonNode : public Node {
public:
    EventComparisonNode(const Comparison &comparison, const string &event);

    bool Evaluate(const Date &date, const string &event) const override;

private:
    const Comparison _comparison;
    const string _event;
};

class LogicalOperationNode : public Node {
public:
    LogicalOperationNode(const LogicalOperation &operation, const shared_ptr<Node> &left,
                         const shared_ptr<Node> &right);

    bool Evaluate(const Date &date, const string &event) const override;

private:
    const LogicalOperation _operation;
    const shared_ptr<Node> _left;
    const shared_ptr<Node> _right;
};

