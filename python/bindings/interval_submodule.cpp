/***************************************************************************
 *            interval_submodule.cpp
 *
 *  Copyright  2008-26  Pieter Collins
 *
 ****************************************************************************/

#include "pybind11.hpp"
#include "interval-utilities.hpp"

using namespace Ariadne;
using namespace PyBind11;

template<class X>
X from_python_object_or_literal_allowing_default_precision(pybind11::handle h) {
    if constexpr (Same<X,Decimal>) {
        try {
            return X(pybind11::cast<double>(h));
        } catch(pybind11::cast_error&) { }
    }
    if constexpr (Constructible<X,String>) {
        try {
            return X(pybind11::cast<String>(h));
        } catch (pybind11::cast_error&) { }
    }
    if constexpr (ConstructibleGivenDefaultPrecision<X,String>) {
        try {
            PrecisionType<X> pr;
            return X(pybind11::cast<String>(h),pr);
        } catch (pybind11::cast_error&) { }
    }
    if constexpr (ConstructibleGivenDefaultPrecision<X,Dyadic>) {
        PrecisionType<X> pr;
        try {
            return X(pybind11::cast<Dyadic>(h),pr);
        } catch (pybind11::cast_error&) { }
        try {
            return X(Dyadic(pybind11::cast<String>(h)),pr);
        } catch (pybind11::cast_error&) { }
    }
    return pybind11::cast<X>(h);
}

template<class IVL>
IVL interval_from_pair(pybind11::handle lh, pybind11::handle uh) {
    using LB=typename IVL::LowerBoundType;
    using UB=typename IVL::UpperBoundType;
    return IVL(
        from_python_object_or_literal_allowing_default_precision<LB>(lh),
        from_python_object_or_literal_allowing_default_precision<UB>(uh));
}

template<class IVL>
IVL interval_from_dict(pybind11::dict dct) {
    assert(dct.size()==1);
    pybind11::detail::dict_iterator::reference item=*dct.begin();
    return interval_from_pair<IVL>(item.first,item.second);
}

template<class IVL>
IVL interval_from_tuple(pybind11::tuple tup) {
    assert(tup.size()==2);
    return interval_from_pair<IVL>(tup[0],tup[1]);
}

template<class IVL>
Void export_interval_arithmetic(pybind11::module&, pybind11::class_<IVL>&) {
}

template<>
Void export_interval_arithmetic(
    pybind11::module& module,
    pybind11::class_<UpperIntervalType>& interval_class)
{
    define_arithmetic(module,interval_class);
    define_transcendental(module,interval_class);
    define_mixed_arithmetic(module,interval_class,Tag<ValidatedNumber>());
}

template<class IVL>
Void export_interval(pybind11::module& module, std::string name=python_class_name<IVL>()) {
    using IntervalType=IVL;
    using LowerBoundType=typename IntervalType::LowerBoundType;
    using UpperBoundType=typename IntervalType::UpperBoundType;
    using MidpointType=typename IntervalType::MidpointType;

    using ContainsType=decltype(contains(declval<IntervalType>(),declval<MidpointType>()));
    using DisjointType=decltype(disjoint(declval<IntervalType>(),declval<IntervalType>()));
    using SubsetType=decltype(subset(
        declval<Interval<UpperBoundType>>(),
        declval<Interval<LowerBoundType>>()));

    pybind11::class_<IntervalType> interval_class(module,name.c_str());
    interval_class.def(pybind11::init<IntervalType>());
    interval_class.def(pybind11::init<MidpointType>());
    interval_class.def(pybind11::init<LowerBoundType,UpperBoundType>());
    interval_class.def(pybind11::init([](pybind11::dict dct){
        return interval_from_dict<IntervalType>(dct); }));
    interval_class.def(pybind11::init([](pybind11::tuple tup){
        return interval_from_tuple<IntervalType>(tup); }));
    pybind11::implicitly_convertible<pybind11::dict,IntervalType>();
    pybind11::implicitly_convertible<pybind11::tuple,IntervalType>();

    if constexpr (Constructible<UpperBoundType,String>) {
        interval_class.def(pybind11::init([](String lb, String ub){
            return IntervalType(LowerBoundType(lb),UpperBoundType(ub)); }));
    } else if constexpr (ConstructibleGivenDefaultPrecision<UpperBoundType,String>) {
        interval_class.def(pybind11::init([](String lb, String ub){
            PrecisionType<UpperBoundType> pr;
            return IntervalType(LowerBoundType(lb,pr),UpperBoundType(ub,pr)); }));
    } else if constexpr (ConstructibleGivenDefaultPrecision<UpperBoundType,Decimal>) {
        interval_class.def(pybind11::init([](String lb, String ub){
            PrecisionType<UpperBoundType> pr;
            return IntervalType(LowerBoundType(Decimal(lb),pr),UpperBoundType(Decimal(ub),pr)); }));
    } else if constexpr (ConstructibleGivenDefaultPrecision<UpperBoundType,Dyadic>) {
        interval_class.def(pybind11::init([](String lb, String ub){
            PrecisionType<UpperBoundType> pr;
            return IntervalType(LowerBoundType(Dyadic(lb),pr),UpperBoundType(Dyadic(ub),pr)); }));
    }

    if constexpr (Constructible<IntervalType,IntervalDomainType> and not Same<IntervalType,IntervalDomainType>) {
        interval_class.def(pybind11::init<IntervalDomainType>());
        if constexpr (Convertible<IntervalDomainType,IntervalType>) {
            pybind11::implicitly_convertible<IntervalDomainType,IntervalType>();
        }
    }
    if constexpr (Constructible<IntervalType,DyadicInterval> and not Same<IntervalType,DyadicInterval>) {
        interval_class.def(pybind11::init<DyadicInterval>());
        interval_class.def(pybind11::init([](Dyadic l, Dyadic u){
            return IntervalType(DyadicInterval(l,u)); }));
        if constexpr (Convertible<DyadicInterval,IntervalType>) {
            pybind11::implicitly_convertible<DyadicInterval,IntervalType>();
        }
    }
    if constexpr (Constructible<IntervalType,RationalInterval> and not Same<IntervalType,RationalInterval>) {
        interval_class.def(pybind11::init<RationalInterval>());
        interval_class.def(pybind11::init([](Rational l, Rational u){
            return IntervalType(RationalInterval(l,u)); }));
        if constexpr (Convertible<RationalInterval,IntervalType>) {
            pybind11::implicitly_convertible<RationalInterval,IntervalType>();
        }
    }
    if constexpr (Constructible<IntervalType,RealInterval> and not Same<IntervalType,RealInterval>) {
        interval_class.def(pybind11::init<RealInterval>());
        interval_class.def(pybind11::init([](Real l, Real u){
            return IntervalType(RealInterval(l,u)); }));
        if constexpr (Convertible<RealInterval,IntervalType>) {
            pybind11::implicitly_convertible<RealInterval,IntervalType>();
        }
    }

    if constexpr (HasPrecisionType<UpperBoundType>) {
        using Precision=PrecisionType<UpperBoundType>;
        if constexpr (Constructible<UpperBoundType,String,Precision>) {
            interval_class.def(pybind11::init([](String ls, String us, Precision pr){
                return IntervalType(LowerBoundType(ls,pr),UpperBoundType(us,pr)); }));
        } else if constexpr (Constructible<UpperBoundType,Decimal,Precision>) {
            interval_class.def(pybind11::init([](String ls, String us, Precision pr){
                return IntervalType(LowerBoundType(Decimal(ls),pr),UpperBoundType(Decimal(us),pr)); }));
        }

        if constexpr (Constructible<IntervalType,FloatBounds<Precision>>) {
            interval_class.def(pybind11::init<FloatBounds<Precision>>());
            if constexpr (Convertible<FloatBounds<Precision>,IntervalType>) {
                pybind11::implicitly_convertible<FloatBounds<Precision>,IntervalType>();
            }
        }
        if constexpr (Constructible<IntervalType,RealInterval,Precision>) {
            interval_class.def(pybind11::init<RealInterval,Precision>());
            interval_class.def(pybind11::init([](Real l, Real u, Precision pr){
                return IntervalType(RealInterval(l,u),pr); }));
        } else if constexpr (Constructible<IntervalType,DyadicInterval,Precision>) {
            interval_class.def(pybind11::init<DyadicInterval,Precision>());
            interval_class.def(pybind11::init([](Dyadic l, Dyadic u, Precision pr){
                return IntervalType(DyadicInterval(l,u),pr); }));
        }
        export_conversions(interval_class);
    }

    export_interval_arithmetic(module,interval_class);

    if constexpr (HasEquality<IVL,IVL>) {
        interval_class.def("__eq__",&__eq__<IVL,IVL,Return<EqualityType<IVL,IVL>>>);
        interval_class.def("__ne__",&__ne__<IVL,IVL,Return<InequalityType<IVL,IVL>>>);
    }

    interval_class.def("lower_bound",&IntervalType::lower_bound);
    interval_class.def("upper_bound",&IntervalType::upper_bound);
    interval_class.def("centre",&IntervalType::centre);
    interval_class.def("midpoint",&IntervalType::midpoint);
    interval_class.def("radius",&IntervalType::radius);
    interval_class.def("width",&IntervalType::width);
    interval_class.def("empty",&IntervalType::is_empty);
    interval_class.def("__str__",&__cstr__<IntervalType>);
    interval_class.def("__repr__",&__repr__<IntervalType>);

    interval_class.def("contains",
        (ContainsType(*)(IntervalType const&,MidpointType const&))&contains);
    module.def("contains",
        (ContainsType(*)(IntervalType const&,MidpointType const&))&contains);

    module.def("centre",&IntervalType::centre);
    module.def("midpoint",&IntervalType::midpoint);
    module.def("radius",&IntervalType::radius);
    module.def("width",&IntervalType::width);

    module.def("disjoint",
        (DisjointType(*)(IntervalType const&,IntervalType const&))&disjoint);
    module.def("subset",
        (SubsetType(*)(Interval<UpperBoundType> const&,Interval<LowerBoundType> const&))&subset);
    module.def("intersection",
        (IntervalType(*)(IntervalType const&,IntervalType const&))&intersection);
    module.def("hull",
        (IntervalType(*)(IntervalType const&,IntervalType const&))&hull);
    module.def("split",
        (Pair<IntervalType,IntervalType>(*)(IntervalType const&))&split);

    if constexpr (Same<IVL,IntervalDomainType>) {
        module.attr("IntervalDomainType")=interval_class;
    } else if constexpr (Same<IVL,IntervalValidatedRangeType>) {
        module.attr("IntervalValidatedRangeType")=interval_class;
    } else if constexpr (Same<IVL,IntervalApproximateRangeType>) {
        module.attr("IntervalApproximateRangeType")=interval_class;
    }
}

Void export_intervals(pybind11::module& module) {
    export_interval<DyadicInterval>(module);
    export_interval<DecimalInterval>(module);
    export_interval<RationalInterval>(module);
    export_interval<RealInterval>(module);

    pybind11::implicitly_convertible<DyadicInterval,DecimalInterval>();
    pybind11::implicitly_convertible<DyadicInterval,RationalInterval>();
    pybind11::implicitly_convertible<DyadicInterval,RealInterval>();
    pybind11::implicitly_convertible<DecimalInterval,RationalInterval>();
    pybind11::implicitly_convertible<DecimalInterval,RealInterval>();
    pybind11::implicitly_convertible<RationalInterval,RealInterval>();

    export_interval<FloatDPExactInterval>(module);
    export_interval<FloatDPLowerInterval>(module);
    export_interval<FloatDPUpperInterval>(module);
    export_interval<FloatDPApproximateInterval>(module);

    pybind11::implicitly_convertible<FloatDPExactInterval,FloatDPUpperInterval>();
    pybind11::implicitly_convertible<FloatDPExactInterval,FloatDPLowerInterval>();

    module.def("cast_singleton",
        (FloatDPBounds(*)(Interval<FloatDPUpperBound> const&))&cast_singleton);
    module.def("cast_singleton",
        (FloatMPBounds(*)(Interval<FloatMPUpperBound> const&))&cast_singleton);
    module.def("cast_exact",
        (IntervalDomainType(*)(IntervalApproximateRangeType const&))&cast_exact_interval);

    template_<Interval> interval_template(module);
    interval_template.instantiate<Dyadic>();
    interval_template.instantiate<Decimal>();
    interval_template.instantiate<Rational>();
    interval_template.instantiate<Real>();
    interval_template.instantiate<FloatDP>();
    interval_template.instantiate<FloatDPUpperBound>();
    interval_template.instantiate<FloatDPLowerBound>();
    interval_template.instantiate<FloatDPApproximation>();
}

Void interval_submodule(pybind11::module& module) {
    export_intervals(module);
}
