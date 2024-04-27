import RowOfPascalT.*;
public class Test
{
    public static void main(String[] args)
    {
        try
        {
            RowOfPascalTriangle row = new RowOfPascalTriangle();
            row.Initialize(Integer.parseInt(args[0]));

            for(int i = 1; i < args.length; i++)
            {
                try 
                { 
                    int x = Integer.parseInt(args[i]); 
                    System.out.println(x + " - " + row.getElement(x)); 
                } 
                catch(NumberFormatException e)
                {
                    System.out.println(args[i] + " - wrong input");
                }
                catch (IndexOutOfBoundsException e) 
                {
                    System.out.println(args[i] + " - number out of range");
                }
            }
        }
        catch(Exception e)
        {
            System.out.println(args[0] + " - wrogn row number");
        }
    }
}