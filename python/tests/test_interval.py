#!/usr/bin/python3

from pyariadne import *

IntervalDomainTypeAlias = FloatDPExactInterval
IntervalRangeTypeAlias = FloatDPUpperInterval

def test_generics():
    assert Interval[FloatDP] == FloatDPExactInterval
    assert Interval[FloatDPUpperBound] == FloatDPUpperInterval
    assert Interval[FloatDPLowerBound] == FloatDPLowerInterval
    assert Interval[FloatDPApproximation] == FloatDPApproximateInterval

def test_interval():
    dp = DoublePrecision()

    xd = ExactDouble(0)
    w = Dyadic(0)

    wivl = DyadicInterval(-w,w)
    rivl = RealInterval(wivl)

    x = FloatDP(0,dp)
    u = FloatDPUpperBound(0,dp)
    l = FloatDPLowerBound(0,dp)
    a = FloatDPApproximation(0,dp)

    xivl = FloatDPExactInterval({-xd:xd})
    xivl = FloatDPExactInterval(-x,x)
    uivl = FloatDPUpperInterval(-u,u)
    livl = FloatDPLowerInterval(-l,l)
    aivl = FloatDPApproximateInterval(-a,a)

    xivl = FloatDPExactInterval(wivl)
    uivl = FloatDPUpperInterval(rivl)

    xivl.lower_bound()
    xivl.upper_bound()
    xivl.centre()
    xivl.midpoint()
    xivl.radius()
    xivl.width()
    xivl.contains(x)
    xivl.empty()

    contains(xivl,x)
    disjoint(xivl,xivl)
    subset(xivl,xivl)
    intersection(xivl,xivl)
    hull(xivl,xivl)
    (xivl,xivl) = split(xivl)

    assert type(subset(xivl,xivl)) == Boolean
    assert type(subset(livl,uivl)) == ValidatedUpperKleenean
    assert type(subset(uivl,livl)) == ValidatedLowerKleenean
    assert type(subset(aivl,aivl)) == ApproximateKleenean

    assert IntervalDomainType == FloatDPExactInterval
    assert IntervalValidatedRangeType == FloatDPUpperInterval
    assert IntervalApproximateRangeType == FloatDPApproximateInterval

def test_interval_arithmetic_and_repr():
    dp = DoublePrecision()
    l = FloatDPLowerBound(1,dp)
    u = FloatDPUpperBound(2,dp)
    ivl = FloatDPUpperInterval(l,u)

    assert type(ivl + ivl) == FloatDPUpperInterval
    assert type(sqr(ivl)) == FloatDPUpperInterval
    assert "FloatDPUpperInterval" in repr(ivl)

    exact = FloatDPExactInterval(FloatDP(0,dp),FloatDP(1,dp))
    approximate = FloatDPApproximateInterval(exact)
    cast_exact(approximate)
