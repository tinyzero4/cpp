#include "node.h"

bool Node::Evaluate(const Date &date, const string &event) const { return true; }

bool EmptyNode::Evaluate(const Date &date, const string &event) const { return true; }

DateComparisonNode::DateComparisonNode(const Comparison &comparison, const Date &date) :
        _comparison(comparison), _date(date) {};

bool DateComparisonNode::Evaluate(const Date &date, const string &event) const {
    if (_comparison == Comparison::Equal) return date == _date;
    else if (_comparison == Comparison::Greater) return date > _date;
    else if (_comparison == Comparison::GreaterOrEqual) return date >= _date;
    else if (_comparison == Comparison::Less) return date < _date;
    else if (_comparison == Comparison::LessOrEqual) return _date >= date;
    else if (_comparison == Comparison::NotEqual) return date != _date;

    return false;
}

EventComparisonNode::EventComparisonNode(const Comparison &comparison, const string &event) :
        _comparison(comparison), _event(event) {};

bool EventComparisonNode::Evaluate(const Date &date, const string &event) const {
    if (_comparison == Comparison::Equal) return event == _event;
    else if (_comparison == Comparison::Greater) return event > _event;
    else if (_comparison == Comparison::GreaterOrEqual) return event >= _event;
    else if (_comparison == Comparison::Less) return event < _event;
    else if (_comparison == Comparison::LessOrEqual) return event <= _event;
    else if (_comparison == Comparison::NotEqual) return event != _event;
    else return false;
}

LogicalOperationNode::LogicalOperationNode(
        const LogicalOperation &operation, const shared_ptr<Node> &left, const shared_ptr<Node> &right) :
        _operation(operation), _left(left), _right(right) {};


bool LogicalOperationNode::Evaluate(const Date &date, const string &event) const {
    if (_operation == LogicalOperation::And) {
        return _left->Evaluate(date, event) && _right->Evaluate(date, event);
    } else if (_operation == LogicalOperation::Or) {
        return _left->Evaluate(date, event) || _right->Evaluate(date, event);
    } else return false;
}

