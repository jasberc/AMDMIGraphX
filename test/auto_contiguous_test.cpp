#include <migraph/auto_contiguous.hpp>
#include <migraph/operators.hpp>
#include <migraph/instruction.hpp>
#include <basic_ops.hpp>
#include <test.hpp>

struct contiguous_target
{
    std::string name() const { return "contiguous"; }
    std::vector<migraph::pass> get_passes(migraph::context&) const
    {
        return {migraph::auto_contiguous{}};
    }
    migraph::context get_context() const { return {}; }
};

// TODO: Add this test case
void literal_broadcast()
{
    migraph::program p;
    p.add_literal(get_2_broadcasted());
    EXPECT(not p.get_shape().standard());
    EXPECT(p.get_shape().broadcasted());
    p.compile(contiguous_target{});
    EXPECT(p.get_shape().standard());
    EXPECT(not p.get_shape().broadcasted());
}

TEST_CASE(literal_transpose)
{
    migraph::program p;
    p.add_literal(get_2x2_transposed());
    EXPECT(not p.get_shape().standard());
    EXPECT(p.get_shape().transposed());
    p.compile(contiguous_target{});
    EXPECT(p.get_shape().standard());
    EXPECT(not p.get_shape().transposed());
}

TEST_CASE(after_literal_transpose)
{
    migraph::program p;
    auto l = p.add_literal(get_2x2());
    EXPECT(p.get_shape().standard());
    EXPECT(not p.get_shape().transposed());
    auto t = p.add_instruction(migraph::op::transpose{{1, 0}}, l);
    p.add_instruction(pass_op{}, t);
    EXPECT(not p.get_shape().standard());
    EXPECT(p.get_shape().transposed());
    p.compile(contiguous_target{});
    EXPECT(p.get_shape().standard());
    EXPECT(not p.get_shape().transposed());
}

TEST_CASE(after_literal_broadcast)
{
    migraph::program p;
    auto l1 = p.add_literal(get_2x2());
    auto l2 = p.add_literal(get_2());
    EXPECT(p.get_shape().standard());
    EXPECT(not p.get_shape().broadcasted());
    auto b = p.add_instruction(migraph::op::broadcast{0, l1->get_shape()}, l2);
    p.add_instruction(pass_op{}, b);
    EXPECT(not p.get_shape().standard());
    EXPECT(p.get_shape().broadcasted());
    p.compile(contiguous_target{});
    EXPECT(p.get_shape().standard());
    EXPECT(not p.get_shape().broadcasted());
}

TEST_CASE(after_param_transpose)
{
    migraph::program p;
    auto l = p.add_parameter("2x2", {migraph::shape::float_type, {2, 2}});
    EXPECT(p.get_shape().standard());
    EXPECT(not p.get_shape().transposed());
    auto t = p.add_instruction(migraph::op::transpose{{1, 0}}, l);
    p.add_instruction(pass_op{}, t);
    EXPECT(not p.get_shape().standard());
    EXPECT(p.get_shape().transposed());
    p.compile(contiguous_target{});
    EXPECT(p.get_shape().standard());
    EXPECT(not p.get_shape().transposed());
}

TEST_CASE(after_param_broadcast)
{
    migraph::program p;
    auto l1 = p.add_parameter("2x2", {migraph::shape::float_type, {2, 2}});
    auto l2 = p.add_parameter("2", {migraph::shape::float_type, {2}});
    EXPECT(p.get_shape().standard());
    EXPECT(not p.get_shape().broadcasted());
    auto b = p.add_instruction(migraph::op::broadcast{0, l1->get_shape()}, l2);
    p.add_instruction(pass_op{}, b);
    EXPECT(not p.get_shape().standard());
    EXPECT(p.get_shape().broadcasted());
    p.compile(contiguous_target{});
    EXPECT(p.get_shape().standard());
    EXPECT(not p.get_shape().broadcasted());
}

int main(int argc, const char* argv[]) { test::run(argc, argv); }
