# 🐼 Pandas Quick-Reference Tutorial

## 1. Import & Setup
Pandas is conventionally imported with the alias `pd`.
```python
import pandas as pd
import numpy as np
```

Use display options only when you want more rows or columns while exploring.
```python
pd.set_option('display.max_rows', 100)
pd.set_option('display.max_columns', 50)
```

---

## 2. Core Data Structures
Pandas is built on two main data structures: **Series** (1D arrays) and **DataFrames** (2D tables).

### Series (1D Array)
A Series is a single column of data. It holds data of any type and has an index.
```python
# Create from a list
data = [10, 20, 30]
s = pd.Series(data, index=['a', 'b', 'c'])
print(s['a']) # Output 10

# Create from a dictionary (Keys become the index)
dict_data = {"x": 100, "y": 200, "z": 300}
s_dict = pd.Series(dict_data)
```

### DataFrame (2D Table)
A DataFrame is a size-mutable, tabular data structure with labeled rows and columns.
```python
# Create from a dictionary of lists
data = {
    'Name': ['Alice', 'Bob', 'Charlie'],
    'Age': [25, 30, 35],
    'City': ['New York', 'Paris', 'London']
}
df = pd.DataFrame(data)

# Create from a list of dictionaries
data_list = [{'Name': 'Alice', 'Age': 25}, {'Name': 'Bob', 'Age': 30}]
df2 = pd.DataFrame(data_list)
```

### Helpful constructors
```python
pd.DataFrame.from_records(data_list)
pd.DataFrame({'A': [1, 2], 'B': [3, 4]})
pd.Series(['a', 'b', 'c'], name='letters')
```

---

## 3. Data Input & Output (Focus on JSON & CSV)

### Working with CSV Files
```python
# Read CSV
df = pd.read_csv('data.csv')

# Export to CSV (index=False prevents writing the row numbers to the file)
df.to_csv('output.csv', index=False)
```

### Working with JSON (W3Schools focus)
JSON plain text has the same format as Python dictionaries, making it highly compatible with Pandas.

**1. Read JSON from a file or URL:**
```python
df = pd.read_json('data.json')
```

**2. Read JSON directly from a Python Dictionary:**
```python
json_data = {
  "Duration":{ "0":60, "1":60, "2":60 },
  "Pulse":{ "0":110, "1":117, "2":103 }
}
df = pd.DataFrame(json_data)
```

**3. Handling Nested JSON (`json_normalize`):**
Often, APIs return nested JSON (objects within objects). Standard `read_json` leaves nested dictionaries as block text in a single column. You must flatten it:
```python
import json

with open('nested_data.json') as f:
    data = json.load(f)

# Flattens nested structures into distinct columns (e.g., 'user.address.city')
df = pd.json_normalize(data) 
```

### Excel, Parquet, SQL
```python
df = pd.read_excel('data.xlsx', sheet_name='Sheet1')
df.to_excel('output.xlsx', index=False)

df = pd.read_parquet('data.parquet')
df.to_parquet('output.parquet', index=False)

df = pd.read_sql('SELECT * FROM students', con=connection)
df.to_sql('students_copy', con=connection, if_exists='replace', index=False)
```

*Note: Large DataFrames are often truncated when printed. Use `print(df.to_string())` to show everything.*

---

## 4. Inspecting & Viewing Data
Once your data is loaded, use these methods to get a feel for its shape and types.

```python
df.head(5)       # View the first 5 rows
df.tail(3)       # View the last 3 rows
df.info()        # Print Index, Datatype, Non-Null counts, and Memory usage
df.describe()    # Summary statistics (mean, std, min, max, quartiles) for numeric cols
df.shape         # Returns a tuple: (rows, columns)
df.columns       # Returns list of column headers
df.dtypes        # Data types of each column
df.nunique()
```

---

## 5. Selection & Slicing

### Selecting Columns
```python
# Single column (returns a Series)
ages = df['Age'] 
ages = df.Age # Dot notation (only works if there are no spaces in the column name)

# Multiple columns (returns a DataFrame)
subset = df[['Name', 'City']] 
```

### Selecting Rows: `loc` vs `iloc`
* **`.loc[]`** is **label-based** (looks at row/column names).
* **`.iloc[]`** is **integer index-based** (looks at 0-based position).

```python
# Select row by index label
row_0 = df.loc[0] 

# Select multiple rows and specific columns by label
subset = df.loc[0:2, ['Name', 'Age']] 

# Select by integer position (first two rows, first two columns)
subset_iloc = df.iloc[0:2, 0:2] 
```

### Filtering (Boolean Indexing)
```python
# Filter rows based on a condition
adults = df[df['Age'] >= 18]

# Multiple conditions: Use & (AND), | (OR), ~ (NOT). You MUST wrap conditions in parentheses!
filtered = df[(df['Age'] > 25) & (df['City'] == 'Paris')]
filtered = df.query("Age > 25 and City == 'Paris'")
```

### Useful index helpers
```python
df = df.set_index('Employee_ID')
df = df.reset_index()
df = df.sort_index()
```

---

## 6. Data Cleaning & Manipulation

### Handling Missing Data (NaN)
```python
# Check for null values per column
df.isnull().sum()

# Drop any row containing missing values
df_clean = df.dropna()

# Fill missing values with a specific value
df_filled = df.fillna(0)

# Fill missing values dynamically (e.g., with the mean of the column)
df['Age'] = df['Age'].fillna(df['Age'].mean())
```

### Duplicates
```python
df.duplicated().sum()
df = df.drop_duplicates()
df = df.drop_duplicates(subset=['Name', 'City'], keep='first')
```

### Modify columns and rows
```python
# Add a new column
df['Salary'] = [50000, 60000, 70000]

# Rename columns
df['Salary_K'] = df['Salary'] / 1000
df.rename(columns={'Name': 'Full Name', 'Age': 'Years'}, inplace=True)

# Drop a column (axis=1 means columns, axis=0 means rows)
df = df.drop('City', axis=1)

# Drop a row by index
df = df.drop(0, axis=0)
```

### Type conversions
```python
df['Age'] = df['Age'].astype('float64')
df['Age'] = pd.to_numeric(df['Age'], errors='coerce')
df['Date'] = pd.to_datetime(df['Date'], errors='coerce')
df['City'] = df['City'].astype('category')
```

### String operations
```python
df['Name'] = df['Name'].str.upper()
df['Name'] = df['Name'].str.strip()
df['Email_ok'] = df['Email'].str.contains('@', na=False)
df['City'] = df['City'].str.replace('New ', '', regex=False)
df[['First', 'Last']] = df['Full Name'].str.split(' ', n=1, expand=True)
```

### Sorting
```python
# Sort by column(s)
df_sorted = df.sort_values(by=['Age', 'Salary'], ascending=[False, True])
df_sorted = df.sort_values(by='Date')
```

---

## 7. Aggregation & Grouping

```python
# View unique values and their frequencies in a column
df['City'].value_counts()

# Group by a column and calculate the mean for each group
grouped = df.groupby('City').mean(numeric_only=True)

# Group by multiple columns with specific aggregations per column
agg_df = df.groupby('City').agg({
    'Age': ['mean', 'max'],
    'Salary': 'sum'
})
```

### More groupby patterns
```python
summary = df.groupby('City').agg(avg_age=('Age', 'mean'), total_salary=('Salary', 'sum'))
df['age_rank_in_city'] = df.groupby('City')['Age'].rank(method='dense')
df['city_mean_salary'] = df.groupby('City')['Salary'].transform('mean')
big_groups = df.groupby('City').filter(lambda group: len(group) >= 3)
```

### MultiIndex cleanup
```python
agg_df = df.groupby('City').agg({'Age': ['mean', 'max']})
agg_df.columns = ['_'.join(col).strip() for col in agg_df.columns.to_flat_index()]
agg_df = agg_df.reset_index()
```

---

## 8. Reshaping Data

```python
wide = df.pivot(index='Date', columns='City', values='Sales')
long = df.melt(id_vars=['Name', 'City'], value_vars=['Math', 'Physics'], var_name='Subject', value_name='Score')
```

### Pivot table, stack, unstack
```python
table = df.pivot_table(index='City', columns='Product', values='Sales', aggfunc='sum', fill_value=0)

stacked = wide.stack()
unstacked = stacked.unstack()
```

`pivot` requires unique index/column pairs. Use `pivot_table` when duplicates exist or aggregation is needed.

---

## 9. Combining DataFrames

### Concatenation
```python
# Stack vertically (add rows, common columns align)
df_concat = pd.concat([df1, df2], axis=0, ignore_index=True)

# Stack horizontally (add columns side-by-side)
df_concat_cols = pd.concat([df1, df2], axis=1)
```

### Merging (SQL-like Joins)
Used to combine data based on common identifiers/keys.
```python
# Inner join on a specific column
df_merged = pd.merge(df1, df2, on='Employee_ID', how='inner')

# Other 'how' options: 'outer' (Full Outer Join), 'left' (Left Join), 'right' (Right Join)
```

---

## 9. Applying Custom Functions
If built-in vectorized functions aren't enough, `.apply()` allows you to map custom functions across rows or columns.

```python
# Apply a lambda function to a single column
df['Age_In_Months'] = df['Age'].apply(lambda x: x * 12)
df['Age_In_Months'] = df['Age'] * 12 # the same

# Apply a custom function across multiple columns (row-wise mapping)
def categorize_salary(row):
    if row['Salary'] > 60000:
        return 'High'
    return 'Low'

# axis=1 ensures the function is applied row by row
df['Salary_Category'] = df.apply(categorize_salary, axis=1)
```

---

## 12. Plotting Basics
Pandas integrates with Matplotlib through `.plot()`.

```python
df['Age'].plot(kind='hist')
df.plot(x='Date', y='Sales', kind='line')
df.groupby('City')['Salary'].mean().plot(kind='bar')
df[['Age', 'Salary']].plot(kind='box')
```

If needed:
```python
import matplotlib.pyplot as plt
plt.title('Sales by Date')
plt.show()
```

---

## 13. Performance Tips
```python
# Prefer vectorized code
df['Revenue'] = df['Price'] * df['Quantity']

# Reduce memory for repeated text values
df['City'] = df['City'].astype('category')

# Use columns you actually need
df = pd.read_csv('data.csv', usecols=['Name', 'Age'])
```

Avoid heavy row-wise apply when a built-in method exists. `.loc` lookups are fastest when the index is meaningful.

---

## 14. Common Gotchas
```python
# Chained assignment can trigger SettingWithCopyWarning
df.loc[df['Age'] > 18, 'Adult'] = True

# Copy explicitly when you want an independent frame
clean_df = df.copy()

# inplace=True is not always faster and can make debugging harder
df = df.dropna()
```

Watch out for index alignment when combining Series or DataFrames; Pandas matches on index labels, not only row order.

---

## 15. Quick Method Map
```python
# Inspect: head, info, describe, shape
# Select: loc, iloc, query
# Clean: dropna, fillna, drop_duplicates, astype, to_numeric, to_datetime
# Group: groupby, agg, transform, filter, value_counts
# Reshape: pivot, pivot_table, melt, stack, unstack
# Combine: concat, merge, join
# Time: dt, resample, shift, diff, rolling, expanding
# Plot: plot(kind='line'/'bar'/'hist'/'box')
```

---

### Pro Tips
1. `inplace=True` can be convenient, but assigning the result back is usually clearer.
2. `set_index()` is useful when you do repeated lookups by a stable key.
3. String methods live under `.str`, datetime methods under `.dt`, and categorical data can save memory.