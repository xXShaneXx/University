package figures;

public class Pentagon implements Figure
{
    private double side;

    public Pentagon(double s)
    {
        side = s;
    }

    public double countArea()
    {
        return Math.sqrt(25 + 10 * Math.sqrt(5)) * side * side / 4;
    }

    public double countCircuit()
    {
        return 5 * side;
    }

    public String getName()
    {
        return "Pentagon";
    }
}
