package PascalT;

import java.awt.*;

public class PascalTriangle 
{
    public static void Draw(Panel outputP, int height) throws IllegalArgumentException 
    {
        if (height < 1)
            throw new IllegalArgumentException("Number must be greater than 0");
        
        outputP.setLayout(new GridLayout(height, 1));
        
        for (int h = 0; h < height; h++) 
        {
            String row = "";
            long prev = 1;
            Panel rowP = new Panel(new FlowLayout(FlowLayout.CENTER, 0, 0)); 
            
            for (int i = 0; i <= h; i++) 
            { 
                row += prev + " ";
                prev = prev * (h - i) / (i + 1); 
            }

            Label label = new Label(row);
            rowP.add(label);
            outputP.add(rowP);
        }
    }
}
