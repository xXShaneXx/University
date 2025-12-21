# 🧠 A Theoretician’s Guide to Experimental Algorithm Design and Debugging  
### _(Summary and Practical Checklist based on David S. Johnson’s “A Theoretician’s Guide to the Experimental Analysis of Algorithms”)_

---

## 🎯 Purpose

This guide summarizes David S. Johnson’s lessons for conducting **scientifically rigorous algorithmic experiments**, focusing on **code optimization, debugging, reproducibility, and experimental design**.  
It translates his ten principles, pitfalls, and pet peeves into a practical programming and algorithm-improvement guide.

---

## ⚙️ 1. Core Philosophy

Experimental algorithmics is a **science**, not a coding contest.  
It seeks **understanding** — not just performance numbers.

- Implement carefully.  
- Measure systematically.  
- Explain results objectively.  
- Ensure reproducibility and comparability.  

> “Implementation is the easy part. The hard part is using that implementation to produce meaningful and valuable research results.” — D.S. Johnson

---

## 🧩 2. Principles of Good Experimental Algorithmics

### **Principle 1 — Perform Newsworthy Experiments**
Work on meaningful problems and algorithms that could matter in practice.  
Avoid trivial or dominated ideas unless the insights are new.

#### 🔥 Pitfalls
1. **Dominated algorithms:** Testing obviously inferior algorithms.  
2. **Wrong questions:** Running full experiments before knowing what to measure.  
3. **Endless experimentation:** Never-ending tuning with no clear goal.  
4. **Losing focus:** Studying random instance behavior instead of algorithms.

#### 💡 Suggestions
- **Think before you compute.**  
- Use **exploratory experiments** to discover good questions.  
- **Iterate:** prototype → analyze → refine → conclude.

---

### **Principle 2 — Tie Your Work to the Literature**
- Study and understand prior results fully.  
- Compare directly to previous experiments.  
- Reproduce old baselines when possible.

#### 🔥 Pet Peeve
- Authors who cite but clearly haven’t read key references.  

---

### **Principle 3 — Use Instance Testbeds That Support General Conclusions**
Mix structured random instances (for scalability) with real-world datasets (for realism).  
Use parameterized generators to control structure and difficulty.

#### 🔥 Pet Peeves
2. **Unstructured random instances:** Unrealistic and misleading.  
3. **Millisecond testbeds:** Problems that finish too quickly to measure meaningfully.  
4. **Already-solved testbeds:** Small or trivial datasets where optima are known.

---

### **Principle 4 — Use Efficient and Effective Experimental Designs**

#### 💡 Suggestions
3. **Variance reduction:** Test all algorithms on the same random instances.  
4. **Bootstrapping:** Efficiently estimate “best-of-k” run behavior.  
5. **Self-documenting programs:** Log machine, compiler, version, parameters, seeds, and date in every run.

---

### **Principle 5 — Use Reasonably Efficient Implementations**
Efficiency matters: poor implementations waste time and distort insight.

#### 🔥 Pet Peeves
5. **“We didn’t have time to optimize”** — not a valid justification.  

#### ⚠️ Pitfall
5. **Over-tuning code:** Don’t obsess over tiny constant-factor improvements.  

#### ✅ Tips
- Profile early; focus on bottlenecks.  
- Choose efficient data structures for core operations.  
- Avoid unnecessary theoretical micro-optimizations.

---

### **Principle 6 — Ensure Reproducibility**
Make experiments repeatable by you and others.

#### 🔥 Pet Peeves
6. **Code doesn’t match the paper.**  
7. **Vague performance standards.**  
8. **Time-based stopping criteria** (e.g., “run for 1 hour”).  
9. **Stopping when reaching known optima.**  
10. **Hand-tuned parameters without description.**  
11. **Single-run studies.**  
12. **Reporting only best results.**

#### ✅ Best Practices
- Define clear stopping conditions (e.g., iterations or operation counts).  
- Report averages, variances, and confidence intervals.  
- Include parameter tuning time in total runtime.  
- Archive and publish code, seeds, and test instances.

---

### **Principle 7 — Ensure Comparability**
Enable future researchers to compare their results with yours.

#### 🔥 Pet Peeves
13. **Uncalibrated machines:** Missing details about hardware or compilers.  
14. **Lost testbeds:** Instances or generators not shared.

#### ⚠️ Pitfall
6. **Lost code or data:** Poor version control or no backups.

#### 💡 Suggestion
6. Use benchmark codes (e.g., DIMACS or LINPACK) for machine calibration and normalization.

---

### **Principle 8 — Report the Full Story**
Be transparent and honest about your results.

#### 🔥 Pet Peeves
15. **False precision:** Excess digits imply false accuracy.  
16. **Unremarked anomalies:** Ignoring surprising outcomes.  
17. **Ex post facto stopping:** Reporting time until “best found.”  
18. **Ignoring total runtime:** Always report complete execution times.

---

### **Principles 9 & 10 — Draw Justified Conclusions & Present Data Informatively**
- Explain *why* algorithms differ in performance.  
- Use visual data summaries (boxplots, scaling plots).  
- Include anomalies and failures — they reveal deeper truths.  
- Support all conclusions with quantitative evidence.

---

## 🧪 3. Debugging and Quality Control

### ✅ Debugging Checklist
- Validate correctness on small, known test cases.  
- Profile code early; fix bottlenecks before large-scale runs.  
- Track all parameter values, seeds, and compiler settings.  
- Examine anomalies — they often reveal subtle bugs.  
- Compare your implementation against trusted baselines.

### ⚠️ Debugging Pitfalls
- Inconsistent or undocumented random seeds.  
- Mixing code versions without tracking.  
- Misinterpreting anomalies as “noise.”  
- Deleting or overwriting original datasets.  

---

## 🚀 4. Experimental Algorithm Workflow

| Step | Goal | Key Practice |
|------|------|--------------|
| 1 | **Plan** | Define meaningful questions; review literature. |
| 2 | **Prototype** | Implement baseline and confirm correctness. |
| 3 | **Explore** | Run small tests; find anomalies and key variables. |
| 4 | **Optimize** | Profile and fix performance bottlenecks. |
| 5 | **Design** | Choose varied and realistic testbeds. |
| 6 | **Measure** | Collect time, memory, and quality metrics. |
| 7 | **Replicate** | Multiple runs; apply variance control. |
| 8 | **Document** | Log all conditions and configurations. |
| 9 | **Analyze** | Explain results, not just report numbers. |
| 10 | **Report** | Publish the full, honest story with supporting data. |

---

## 📘 5. Key Takeaways

- **Think before you compute.**  
- **Efficiency and rigor** are equally essential.  
- **Reproducibility** defines credibility.  
- **Anomalies** are gold — not garbage.  
- **Transparency beats triumph:** show both strengths and weaknesses.  
- Good experimental algorithmics balances **theory’s rigor** with **engineering’s realism**.

---

### 🏁 Final Quote

> “Think before you compute. Then compute carefully, explain fully, and let your code — not your excuses — speak for your algorithm.”  
> — David S. Johnson

---

# 🧩 Algorithm Debugging & Optimization Quick Checklist

A one-page practical summary for experimenters and developers.

---

## 🛠 Implementation
- [ ] Verify correctness with known instances.
- [ ] Use profiling tools to locate bottlenecks early.
- [ ] Use efficient, standard data structures.
- [ ] Avoid untested “speed hacks” that obscure correctness.

## 🧪 Experiment Design
- [ ] Plan goals and metrics before coding.
- [ ] Use the same random instances across algorithms.
- [ ] Calibrate machine performance with benchmarks.
- [ ] Define algorithmic (not time-based) stopping conditions.
- [ ] Use statistically significant sample sizes.

## 🧾 Documentation
- [ ] Output algorithm name, version, date, machine, parameters, and seeds.  
- [ ] Log intermediate data and anomalies.  
- [ ] Archive all code, data, and logs with clear version control.

## 🧠 Analysis
- [ ] Report averages **and** variability.  
- [ ] Explain *why* one algorithm outperforms another.  
- [ ] Include negative and anomalous results.  
- [ ] Avoid overprecision; round to meaningful digits.  

## 🔁 Reproducibility
- [ ] Make source code and test instances publicly available.  
- [ ] Describe tuning or parameter adjustment procedures clearly.  
- [ ] Include runtime calibration data.  
- [ ] Use consistent metrics across papers and implementations.

## 🚨 Common Mistakes to Avoid
- [ ] Dominated or trivial algorithms.  
- [ ] Unstructured random testbeds.  
- [ ] One-run or “best-only” results.  
- [ ] Hand-tuned parameters without rules.  
- [ ] Deleting original data/code.  
- [ ] Ignoring unexplained anomalies.  
- [ ] Reporting non-reproducible timings.

---

✅ **Follow these guidelines to transform experiments into real scientific contributions — not just code runs.**

---
