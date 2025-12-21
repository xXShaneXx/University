package figures;

public class Hexagon implements Figure
{
    private double side;

    public Hexagon(double s)
    {
        side = s;
    }

    public double countArea()
    {
        return 1.5 * side * side * Math.sqrt(3);
    }

    public double countCircuit()
    {
        return 6 * side;
    }

    public String getName()
    {
        return "Hexagon";
    }
}
