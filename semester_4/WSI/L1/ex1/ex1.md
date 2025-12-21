# Exercise 1

Współczynnik prawidłowej rozpoznawalności (accuracy):

- Na zbiorze testowym (validation set): 98.06% (0.9806)
- Na zbiorze treningowym: 98.70% (0.9870)

|           | precision | recall | f1-score | support |
|-----------|------------|--------|----------|---------|
| 0         | 0.9719     | 0.9898 | 0.9808   | 980     |
| 1         | 0.9834     | 0.9947 | 0.9890   | 1135    |
| 2         | 0.9815     | 0.9748 | 0.9781   | 1032    |
| 3         | 0.9859     | 0.9683 | 0.9770   | 1010    |
| 4         | 0.9807     | 0.9857 | 0.9832   | 982     |
| 5         | 0.9712     | 0.9843 | 0.9777   | 892     |
| 6         | 0.9813     | 0.9854 | 0.9833   | 958     |
| 7         | 0.9882     | 0.9737 | 0.9809   | 1028    |
| 8         | 0.9744     | 0.9784 | 0.9764   | 974     |
| 9         | 0.9859     | 0.9703 | 0.9780   | 1009    |
| accuracy  |            |        | 0.9806   | 10000   |
| macro avg | 0.9805     | 0.9806 | 0.9805   | 10000   |
| weighted avg | 0.9807  | 0.9806 | 0.9806   | 10000   |

F1-score → Średnia harmoniczna precyzji i czułości (im wyższa, tym lepszy balans modelu).

Macro avg → Średnia dla wszystkich klas (bez ważenia liczbą próbek).

Weighted avg → Średnia ważona liczbą przykładów w każdej klasie.