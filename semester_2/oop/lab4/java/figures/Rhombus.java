package figures;

public class Rhombus implements Quadrangle
{
    private double side;
    private double angle;

    public Rhombus(double s, double a)
    {
        side = s;
        angle = a;
    }

    public double countArea()
    {
        return  side * side * Math.sin(Math.toRadians(angle));
    }

    public double countCircuit()
    {
        return 4 * side;
    }

    public String getName()
    {
        return "Rhombus";
    }
}