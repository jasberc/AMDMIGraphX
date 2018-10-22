#ifndef MIGRAPH_GUARD_RTGLIB_CONVOLUTION_HPP
#define MIGRAPH_GUARD_RTGLIB_CONVOLUTION_HPP

#include <migraph/gpu/lowering.hpp>
#include <migraph/manage_ptr.hpp>
#include <migraph/instruction.hpp>
#include <migraph/operators.hpp>
#include <migraph/generate.hpp>
#include <migraph/shape_for_each.hpp>
#include <migraph/gpu/miopen.hpp>
#include <migraph/gpu/hip.hpp>
#include <migraph/dfor.hpp>
#include <migraph/gpu/device/contiguous.hpp>
#include <migraph/gpu/device/add.hpp>
#include <migraph/iterator_for.hpp>
#include <migraph/gpu/rocblas.hpp>
#include <migraph/gpu/context.hpp>
#include <utility>

namespace migraph {
namespace gpu {

struct miopen_convolution
{
    op::convolution op;
    shared<convolution_descriptor> cd;
    miopenConvFwdAlgorithm_t algo{};

    template <class Self, class F>
    static auto reflect(Self& self, F f)
    {
        // TODO: Add algo
        return op::convolution::reflect(self.op, f);
    }

    std::string name() const { return "gpu::convolution"; }
    shape compute_shape(const std::vector<shape>& inputs) const;
    argument
    compute(context& ctx, const shape& output_shape, const std::vector<argument>& args) const;
    shape compile(context& ctx, const shape& output_shape, std::vector<instruction_ref> inputs);
};

} // namespace gpu

} // namespace migraph

#endif
