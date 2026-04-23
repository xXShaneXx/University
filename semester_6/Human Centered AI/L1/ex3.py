import pandas as pd



def main():
    # Load the dataset
    df = pd.read_csv('data.csv')

    groups = df['group'].unique()

    results = {}
    # Display the first few rows of the dataset
    print(groups)

    for gr in groups:
        df_gr = df[df['group'] == gr]

        fp = df_gr.query("true_label != pred_label and pred_label == 1").shape[0]
        tp = df_gr.query("true_label == pred_label and pred_label == 1").shape[0]

        tn = df_gr[(df_gr['true_label'] == df_gr['pred_label']) & (df_gr['true_label'] == 0)].shape[0]
        fn = df_gr[(df_gr['true_label'] != df_gr['pred_label']) & (df_gr['true_label'] == 0)].shape[0]

        n = len(df_gr)
        accuracy = (tp + tn) / n if n > 0 else 0
        selection_rate = (tp + fp) / n if n > 0 else 0
        tpr = tp / (tp + fn) if (tp + fn) > 0 else 0
        fpr = fp / (fp + tn) if (fp + tn) > 0 else 0

        results[gr] = {
            'N': n,
            'accuracy': accuracy,
            'selection_rate': selection_rate,
            'tpr': tpr,
            'fpr': fpr
        }

        print(f"Metrics for group {gr}:")
        print(f"  N: {n}")
        print(f"  Accuracy: {accuracy:.2f}")
        print(f"  Selection Rate: {selection_rate:.2f}")
        print(f"  TPR: {tpr:.2f}")
        print(f"  FPR: {fpr:.2f}")
        print("-" * 20)

    # Overall metrics
    fp_total = df.query("true_label != pred_label and pred_label == 1").shape[0]
    tp_total = df.query("true_label == pred_label and pred_label == 1").shape[0]
    tn_total = df[(df['true_label'] == df['pred_label']) & (df['true_label'] == 0)].shape[0]
    fn_total = df[(df['true_label'] != df['pred_label']) & (df['true_label'] == 0)].shape[0]
    n_total = len(df)

    accuracy_total = (tp_total + tn_total) / n_total if n_total > 0 else 0
    selection_rate_total = (tp_total + fp_total) / n_total if n_total > 0 else 0
    tpr_total = tp_total / (tp_total + fn_total) if (tp_total + fn_total) > 0 else 0
    fpr_total = fp_total / (fp_total + tn_total) if (fp_total + tn_total) > 0 else 0

    print("Overall Metrics:")
    print(f"  N: {n_total}")
    print(f"  Accuracy: {accuracy_total:.2f}")
    print(f"  Selection Rate: {selection_rate_total:.2f}")
    print(f"  TPR: {tpr_total:.2f}")
    print(f"  FPR: {fpr_total:.2f}")
    print("-" * 20)

    # ART-style audit summary
    fairness_flag = False
    if len(groups) == 2:
        group_a, group_b = groups[0], groups[1]
        tpr_diff = abs(results[group_a]['tpr'] - results[group_b]['tpr'])
        selection_rate_diff = abs(results[group_a]['selection_rate'] - results[group_b]['selection_rate'])

        if tpr_diff >= 0.10 or selection_rate_diff >= 0.10:
            fairness_flag = True

    print("ART-style Audit Summary:")
    print(f"  Fairness Flag: {'TRIGGERED' if fairness_flag else 'Not Triggered'}")
    if fairness_flag:
        print("  Recommended Next Step: Disparity detected. It is recommended to run a bias test to understand the root cause and define an appeals process for affected individuals, enhancing procedural fairness.")
    else:
        print("  Recommended Next Step: No significant disparity detected. Continue to monitor model performance and set up an incident reporting system to catch future issues.")
    print("-" * 20)

    # Create audit trail log
    with open('audit_log.txt', 'w') as f:
        f.write("id,group,true_label,pred_label,score,decision_reason\n")
        for index, row in df.iterrows():
            decision_reason = f"approved because score>={0.5}" if row['score'] >= 0.5 else f"rejected because score<{0.5}"
            f.write(f"{row['id']},{row['group']},{row['true_label']},{row['pred_label']},{row['score']},{decision_reason}\n")
    print("audit_log.txt created successfully.")



if __name__ == "__main__":
    main()