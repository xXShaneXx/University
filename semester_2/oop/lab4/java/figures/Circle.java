package figures;

public class Circle implements Figure
{
    private double  radious;

    public Circle(double r)
    {
        radious = r;
    }

    public double countArea()
    {
        return Math.PI * radious * radious;
    }

    public double countCircuit()
    {
        return 2 * Math.PI * radious;
    }

    public String getName()
    {
        return "Circle";
    }
}
