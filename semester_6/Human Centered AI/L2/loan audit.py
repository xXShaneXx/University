import csv
import math
import statistics
from collections import defaultdict
import functools


def safe_div(numerator, denominator):
    if denominator == 0:
        return 0.0
    return numerator / denominator


def load_rows():
    candidate_paths = [
        'loan_decisions.csv',
        __file__.rsplit('/', 1)[0] + '/loan_decisions.csv',
    ]

    for path in candidate_paths:
        try:
            with open(path, newline='', encoding='utf-8') as file:
                return list(csv.DictReader(file))
        except FileNotFoundError:
            continue

    raise FileNotFoundError('loan_decisions.csv not found.')


def compute_metrics(rows):
    tp = fp = tn = fn = 0

    for row in rows:
        true_label = int(row['true_label'])
        pred_label = int(row['pred_label'])

        if true_label == 1 and pred_label == 1:
            tp += 1
        elif true_label == 0 and pred_label == 1:
            fp += 1
        elif true_label == 0 and pred_label == 0:
            tn += 1
        else:
            fn += 1

    n = len(rows)
    accuracy = statistics.mean(
        int(int(row['true_label']) == int(row['pred_label'])) for row in rows
    ) if n > 0 else 0.0
    selection_rate = safe_div(tp + fp, n)
    tpr = safe_div(tp, tp + fn)
    fpr = safe_div(fp, fp + tn)

    return {
        'N': n,
        'Accuracy': accuracy,
        'Selection Rate': selection_rate,
        'TPR': tpr,
        'FPR': fpr,
    }

def compute_column_mean(rows, column):
    n = len(rows)
    mean = statistics.mean(float(row[column]) for row in rows) if n > 0 else 0.0

    return mean

def compute_pearson_correlation(rows, col1, col2):
    n = len(rows)
    if n == 0:
        return 0.0

    mean1 = compute_column_mean(rows, col1)
    mean2 = compute_column_mean(rows, col2)

    numerator = sum(
        (float(row[col1]) - mean1) * (float(row[col2]) - mean2)
        for row in rows
    )
    denominator_left = sum((float(row[col1]) - mean1) ** 2 for row in rows)
    denominator_right = sum((float(row[col2]) - mean2) ** 2 for row in rows)
    denominator = math.sqrt(denominator_left * denominator_right)

    return safe_div(numerator, denominator)


def rank_feature_correlations(rows, target_column, features):
    ranked = []
    for feature_column, feature_label in features:
        correlation = compute_pearson_correlation(rows, feature_column, target_column)
        direction = 'positive' if correlation >= 0 else 'negative'
        ranked.append((feature_label, correlation, direction))

    ranked.sort(key=lambda item: abs(item[1]), reverse=True)
    return ranked



def print_metrics(label, metrics):
    print(
        f"{label}: "
        f"N={metrics['N']}, "
        f"Accuracy={metrics['Accuracy']:.3f}, "
        f"Selection Rate={metrics['Selection Rate']:.3f}, "
        f"TPR={metrics['TPR']:.3f}, "
        f"FPR={metrics['FPR']:.3f}"
    )


def main():
    data = load_rows()
    feature_columns = [
        ('income_k', 'income k'),
        ('credit_score', 'credit score'),
        ('debt_ratio', 'debt ratio'),
        ('years_employed', 'years employed'),
    ]

    grouped_rows = defaultdict(list)
    for row in data:
        grouped_rows[row['group']].append(row)

    metrics_a = compute_metrics(grouped_rows['A'])
    metrics_b = compute_metrics(grouped_rows['B'])
    overall_metrics = compute_metrics(data)

    tpr_diff = math.fabs(metrics_a['TPR'] - metrics_b['TPR'])
    fpr_diff = math.fabs(metrics_a['FPR'] - metrics_b['FPR'])
    selection_diff = math.fabs(
        metrics_a['Selection Rate'] - metrics_b['Selection Rate']
    )

    screening_flag = (
        tpr_diff >= 0.10
        or fpr_diff >= 0.10
        or selection_diff >= 0.10
    )

    print('Per-group metrics:')
    print_metrics('Group A', metrics_a)
    print_metrics('Group B', metrics_b)

    print('\nOverall metrics:')
    print_metrics('Overall', overall_metrics)

    print('\nAbsolute differences:')
    print(f"|TPR(A) - TPR(B)| = {tpr_diff:.3f}")
    print(f"|FPR(A) - FPR(B)| = {fpr_diff:.3f}")
    print(f"|SelectionRate(A) - SelectionRate(B)| = {selection_diff:.3f}")

    print('\nFairness screening flag:')
    print('RAISED' if screening_flag else 'NOT RAISED')

    ranked_features = rank_feature_correlations(data, 'model_score', feature_columns)

    print('\nCorrelation-based feature importance proxy:')
    print('feature name | correlation value | direction')
    for feature_name, correlation, direction in ranked_features:
        print(f"{feature_name} | {correlation:.3f} | {direction}")


if __name__ == '__main__':
    main()