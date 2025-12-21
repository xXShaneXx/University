# Homework 4 - MPIS

## Overview

This directory contains the Homework 4 assignments for the MPIS course, part of the Semester 3 curriculum.

## Contents

- **Assignment 1**: Tail inequalities for binomial distribution Bin\((n, \frac{1}{2})\)

The goal of this task is to compare bounds obtained using Markov's and Chebyshev's inequalities for the "tails" of the binomial distribution Bin\((n, \frac{1}{2})\) for \(n \in \{100, 1000, 10000...\}\).

Let \(X \sim Bin(n, \frac{1}{2})\). Apply Markov's inequality and Chebyshev's inequality to estimate:

(a) \(P(X > 1 \frac{1}{5} E(X))\),

(b) \(P(|X - E(X)| > \frac{1}{10} E(X))\).

To calculate exact probabilities of these events, use a mathematical package/numerical computation library. In your solution, utilize the fact that the Bin\((n, \frac{1}{2})\) distribution is symmetric.

Present (e.g., in a table) and briefly discuss the obtained results. Which inequality provides more accurate estimates?

- **Assignment 2**: Tail inequalities for binomial distribution Bin\((n, \frac{1}{3})\)

The goal of this task is to analyze the tail probabilities of the binomial distribution Bin\((n, \frac{1}{3})\) using Chernoff bounds for \(n \in \{50, 500, 5000\}\).

Let \(Y \sim Bin(n, \frac{1}{3})\). Apply Chernoff bounds to estimate:

(a) \(P(Y > (1 + \delta) E(Y))\) for \(\delta > 0\),

(b) \(P(Y < (1 - \delta) E(Y))\) for \(\delta > 0\).

Compute the exact probabilities and compare them with the estimates obtained using Chernoff bounds. Discuss the effectiveness of Chernoff bounds in approximating tail probabilities.

- **Assignment 3**: Confidence intervals for sample mean

This assignment involves constructing confidence intervals for the sample mean of a normally distributed random variable with unknown variance.

Let \(Z_1, Z_2, ..., Z_n\) be a random sample from \(N(\mu, \sigma^2)\). For \(n = 30\) and \(n = 100\), construct 95% and 99% confidence intervals for the sample mean \(\bar{Z}\). Use both the t-distribution and the normal approximation to construct the intervals.

Compare the lengths of the confidence intervals obtained using the two methods. Discuss the conditions under which each method is appropriate and the impact of sample size on the interval length.
