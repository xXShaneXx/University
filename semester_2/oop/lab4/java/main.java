import java.util.ArrayList;

import figures.*;

public class main 
{
    public static Figure createFigure(char figure, ArrayList<Double> parameters)
    {
        switch (figure)
		{
			case 'o':
				return new Circle(parameters.get(0));

			case 'p':
				return new Pentagon(parameters.get(0));

			case 's':
				return new Hexagon(parameters.get(0));

			case 'c':
				if (parameters.get(4) == 90.0)
				{
					if (parameters.get(0).equals(parameters.get(1))
					&& parameters.get(1).equals(parameters.get(2)))
						return new Square(parameters.get(0));
					else
						return new Rectangle(parameters.get(0), parameters.get(2));
				}
				else
					return new Rhombus(parameters.get(0), parameters.get(4));

			default:
				return null;
		}
    }
    public static void main(String[] args)
    {
		ArrayList<Double> p = new ArrayList<Double>();
		char option = args[0].charAt(0);
		Figure figure;

		switch (option)
		{
			case 'o':
			case 'p':
			case 's':
				p.add(Double.parseDouble(args[1]));
				break;
			case 'c':
				for (int i = 1; i < 6; i++)
				{
					p.add(Double.parseDouble(args[i]));
				}
				break;
		}

		figure = createFigure(option, p);
		System.out.println(figure.getName() + " area:" + figure.countArea() +
		" circuit:" + figure.countCircuit());
    }
}
