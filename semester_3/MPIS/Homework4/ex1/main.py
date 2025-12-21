import numpy as np
import scipy.stats as stats
import pandas as pd
from reportlab.lib.pagesizes import letter
from reportlab.platypus import Table, SimpleDocTemplate, TableStyle
from reportlab.lib import colors

n_values = [100, 1000, 10000]
p = 0.5

def markov_inequality(probability, mean, threshold):
    return mean / threshold

def chebyshev_inequality(mean, var, threshold):
    return var / (threshold**2)

data = []

data.append(["n", "Exact P(X > 1.2E(X))", "Markow", "Czebyszew",
             "Exact P(|X - E(X)| > 0.1E(X))", "Markow", "Czebyszew"])

for n in n_values:
    mean = n * p
    var = n * p * (1 - p)


    # (a) P(X > 1.2 * E(X))
    threshold_a = mean * 1.2
    exact_a = round(1 - stats.binom.cdf(threshold_a, n, p), 4)
    markov_a = round(markov_inequality(probability=1, mean=mean, threshold=threshold_a), 4)
    chebyshev_a = round(chebyshev_inequality(mean=mean, var=var, threshold=threshold_a - mean), 4)

    # (b) P(|X - E(X)| > 0.1 * E(X))
    threshold_b = mean * 0.1
    exact_b = round(1 - stats.binom.cdf(mean + threshold_b, n, p) + stats.binom.cdf(mean - threshold_b, n, p), 4)
    markov_b = round(markov_inequality(probability=1, mean=var, threshold=threshold_b), 4)
    chebyshev_b = round(chebyshev_inequality(mean=mean, var=var, threshold=threshold_b), 4)

    data.append([n, exact_a, markov_a, chebyshev_a, exact_b, markov_b, chebyshev_b])

def generate_pdf(data, filename):
    doc = SimpleDocTemplate(filename, pagesize=letter)
    table = Table(data)

    style = TableStyle([
        ('BACKGROUND', (0, 0), (-1, 0), colors.grey),
        ('TEXTCOLOR', (0, 0), (-1, 0), colors.whitesmoke),
        ('ALIGN', (0, 0), (-1, -1), 'CENTER'),
        ('FONTNAME', (0, 0), (-1, 0), 'Helvetica-Bold'),
        ('BOTTOMPADDING', (0, 0), (-1, 0), 8),
        ('BACKGROUND', (0, 1), (-1, -1), colors.beige),
        ('GRID', (0, 0), (-1, -1), 1, colors.black),
    ])

    table.setStyle(style)
    elements = [table]
    doc.build(elements)

generate_pdf(data, "wyniki_nierownosci.pdf")





