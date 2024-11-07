package RowOfPascalT;

import java.util.ArrayList;

public class RowOfPascalTriangle 
{
    ArrayList<Integer> row;

    public RowOfPascalTriangle()
    {}

    public void Initialize(int n)
    {
        row = new ArrayList<Integer>();
        for(int i = 0; i <= n; i++)
        {
            row.add(Count(i, n));
        }
    }

    public int getElement(int index)
    {
        return row.get(index);
    }


    private int Count(int index, int r)
    {
        if (r == 0 || r == 1)
            return 1;

        if (index == 0)
            return 1;

        if (index == r)
            return 1;

        return Count(index - 1, r - 1) + Count(index, r - 1);
    }
}