#include "node_test.h"

class AlwaysFalseNode : public Node {
    bool Evaluate(const Date&, const std::string& event) const override {
        return false;
    }
};

void TestDateComparisonNode() {
    {
        DateComparisonNode dcn(Comparison::Equal, {2017, 11, 18});
        Assert(dcn.Evaluate(Date{2017, 11, 18}, ""), "DateComparison 1");
        Assert(!dcn.Evaluate(Date{2017, 11, 19}, ""), "DateComparison 2");
    }
    {
        DateComparisonNode dcn(Comparison::NotEqual, {2017, 11, 18});
        Assert(dcn.Evaluate(Date{2017, 11, 19}, ""), "DateComparison 3");
        Assert(!dcn.Evaluate(Date{2017, 11, 18}, ""), "DateComparison 4");
    }
    {
        DateComparisonNode dcn(Comparison::Less, {2017, 11, 18});
        Assert(dcn.Evaluate(Date{2017, 11, 17}, ""), "DateComparison 5");
        Assert(!dcn.Evaluate(Date{2017, 11, 18}, ""), "DateComparison 6");
        Assert(!dcn.Evaluate(Date{2017, 11, 19}, ""), "DateComparison 7");
    }
    {
        DateComparisonNode dcn(Comparison::Greater, {2017, 11, 18});
        Assert(dcn.Evaluate(Date{2017, 11, 19}, ""), "DateComparison 8");
        Assert(!dcn.Evaluate(Date{2017, 11, 18}, ""), "DateComparison 9");
        Assert(!dcn.Evaluate(Date{2017, 11, 17}, ""), "DateComparison 10");
    }
    {
        DateComparisonNode dcn(Comparison::LessOrEqual, {2017, 11, 18});
        Assert(dcn.Evaluate(Date{2017, 11, 17}, ""), "DateComparison 11");
        Assert(dcn.Evaluate(Date{2017, 11, 18}, ""), "DateComparison 12");
        Assert(!dcn.Evaluate(Date{2017, 11, 19}, ""), "DateComparison 13");
    }
    {
        DateComparisonNode dcn(Comparison::GreaterOrEqual, {2017, 11, 18});
        Assert(dcn.Evaluate(Date{2017, 11, 19}, ""), "DateComparison 14");
        Assert(dcn.Evaluate(Date{2017, 11, 18}, ""), "DateComparison 15");
        Assert(!dcn.Evaluate(Date{2017, 11, 17}, ""), "DateComparison 16");
    }
}

void TestEventComparisonNode() {
    {
        EventComparisonNode ecn(Comparison::Equal, "abc");
        Assert(ecn.Evaluate(Date{0, 1, 1}, "abc"), "EventComparison 1");
        Assert(!ecn.Evaluate(Date{0, 1, 1}, "cbe"), "EventComparison 2");
    }
    {
        EventComparisonNode ecn(Comparison::NotEqual, "abc");
        Assert(ecn.Evaluate(Date{0, 1, 1}, "cde"), "EventComparison 3");
        Assert(!ecn.Evaluate(Date{0, 1, 1}, "abc"), "EventComparison 4");
    }
    {
        EventComparisonNode ecn(Comparison::Less, "abc");
        Assert(ecn.Evaluate(Date{0, 1, 1}, "abb"), "EventComparison 5");
        Assert(!ecn.Evaluate(Date{0, 1, 1}, "abc"), "EventComparison 6");
        Assert(!ecn.Evaluate(Date{0, 1, 1}, "abd"), "EventComparison 7");
    }
    {
        EventComparisonNode ecn(Comparison::Greater, "abc");
        Assert(ecn.Evaluate(Date{0, 1, 1}, "abd"), "EventComparison 8");
        Assert(!ecn.Evaluate(Date{0, 1, 1}, "abc"), "EventComparison 9");
        Assert(!ecn.Evaluate(Date{0, 1, 1}, "abb"), "EventComparison 10");
    }
    {
        EventComparisonNode ecn(Comparison::LessOrEqual, "abc");
        Assert(ecn.Evaluate(Date{0, 1, 1}, "abb"), "EventComparison 11");
        Assert(ecn.Evaluate(Date{0, 1, 1}, "abc"), "EventComparison 12");
        Assert(!ecn.Evaluate(Date{0, 1, 1}, "abd"), "EventComparison 13");
    }
    {
        EventComparisonNode ecn(Comparison::GreaterOrEqual, "abc");
        Assert(ecn.Evaluate(Date{0, 1, 1}, "abd"), "EventComparison 14");
        Assert(ecn.Evaluate(Date{0, 1, 1}, "abc"), "EventComparison 15");
        Assert(!ecn.Evaluate(Date{0, 1, 1}, "abb"), "EventComparison 16");
    }
}

void TestLogicalOperationNode() {
    {
        LogicalOperationNode lon(LogicalOperation::And, make_shared<EmptyNode>(), make_shared<EmptyNode>());
        Assert(lon.Evaluate(Date{0, 1, 1}, "abc"), "LogicalOperationNode 1");
    }
    {
        LogicalOperationNode lon(LogicalOperation::And, make_shared<AlwaysFalseNode>(), make_shared<EmptyNode>());
        Assert(!lon.Evaluate(Date{0, 1, 1}, "abc"), "LogicalOperationNode 2");
    }
    {
        LogicalOperationNode lon(LogicalOperation::And, make_shared<EmptyNode>(), make_shared<AlwaysFalseNode>());
        Assert(!lon.Evaluate(Date{0, 1, 1}, "abc"), "LogicalOperationNode 3");
    }
    {
        LogicalOperationNode lon(LogicalOperation::And, make_shared<AlwaysFalseNode>(), make_shared<AlwaysFalseNode>());
        Assert(!lon.Evaluate(Date{0, 1, 1}, "abc"), "LogicalOperationNode 4");
    }
    {
        LogicalOperationNode lon(LogicalOperation::Or, make_shared<EmptyNode>(), make_shared<EmptyNode>());
        Assert(lon.Evaluate(Date{0, 1, 1}, "abc"), "LogicalOperationNode 5");
    }
    {
        LogicalOperationNode lon(LogicalOperation::Or, make_shared<AlwaysFalseNode>(), make_shared<EmptyNode>());
        Assert(lon.Evaluate(Date{0, 1, 1}, "abc"), "LogicalOperationNode 6");
    }
    {
        LogicalOperationNode lon(LogicalOperation::Or, make_shared<EmptyNode>(), make_shared<AlwaysFalseNode>());
        Assert(lon.Evaluate(Date{0, 1, 1}, "abc"), "LogicalOperationNode 7");
    }
    {
        LogicalOperationNode lon(LogicalOperation::Or, make_shared<AlwaysFalseNode>(), make_shared<AlwaysFalseNode>());
        Assert(!lon.Evaluate(Date{0, 1, 1}, "abc"), "LogicalOperationNode 8");
    }
}

void TestEmptyNode() {
    {
        EmptyNode en;
        Assert(en.Evaluate(Date{0, 1, 1}, "abc"), "EmptyNode 1");
        Assert(en.Evaluate(Date{2017, 11, 18}, "def"), "EmptyNode 2");
        Assert(en.Evaluate(Date{9999, 12, 31}, "ghi"), "EmptyNode 3");
    }
}

void TestNode() {
    TestDateComparisonNode();
    TestEventComparisonNode();
    TestLogicalOperationNode();
    TestEmptyNode();
}
