package figures;

public class Rectangle implements Quadrangle
{
    private double side1;
    private double side2;

    public Rectangle(double s1, double s2)
    {
        side1 = s1;
        side2 = s2;
    }

    public double countArea()
    {
        return side1 * side2;
    }

    public double countCircuit()
    {
        return 2 * side1 + 2 * side2;
    }

    public String getName()
    {
        return "Rectangle";
    }
}
