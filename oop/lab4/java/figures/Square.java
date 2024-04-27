package figures;

public class Square implements Quadrangle
{
    private double side;

    public Square(double s)
    {
        side = s;
    }

    public double countArea()
    {
        return side * side;
    }

    public double countCircuit()
    {
        return 4 * side;
    }

    public String getName()
    {
        return "Square";
    }
}
