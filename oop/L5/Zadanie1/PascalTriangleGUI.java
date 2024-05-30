import java.awt.*;
import java.awt.event.*;

import PascalT.PascalTriangle;

public class PascalTriangleGUI extends Frame 
{
    private Label inputL;
    private TextField inputF;
    private Panel pascalP;
    private Button drawB;

    public PascalTriangleGUI() 
    {
        super("Pascal Triangle");
        setLayout(new BorderLayout());
        setBackground(Color.black);

        Panel inputPanel = new Panel();
        inputPanel.setLayout(new FlowLayout(FlowLayout.CENTER));
        inputPanel.setBackground(Color.GRAY);

        Font font = new Font("Arial", Font.PLAIN, 18); 

        inputL = new Label("Enter the height of the Pascal Triangle");
        inputL.setFont(font);

        inputF = new TextField(10);
        inputF.setFont(font);

        pascalP = new Panel(new GridLayout(0, 1));
        pascalP.setFont(font);
        pascalP.setBackground(Color.LIGHT_GRAY);

        ScrollPane scrollPane = new ScrollPane();
        scrollPane.add(pascalP);
        scrollPane.setPreferredSize(new Dimension(400, 300));

        drawB = new Button("Draw the Pascal Triangle");
        drawB.setFont(font);
        drawB.addActionListener(new ActionListener() 
        {
            public void actionPerformed(ActionEvent e) 
            {
                DrawTriangle();
            }
        });

        inputPanel.add(inputL);
        inputPanel.add(inputF);
        inputPanel.add(drawB);

        add(inputPanel, BorderLayout.NORTH);
        add(scrollPane, BorderLayout.CENTER);

        addWindowListener(new WindowAdapter() 
        {
            public void windowClosing(WindowEvent e) 
            {
                dispose();
            }
        });

        setVisible(true);
        pack();
    }

    private void DrawTriangle() 
    {
        int height;
        try 
        {
            height = Integer.parseInt(inputF.getText());
            pascalP.removeAll(); // Remove all components from the panel
            PascalTriangle.Draw(pascalP, height);
            pascalP.revalidate(); // Revalidate the panel to update the layout
            //pascalP.repaint(); // Repaint the panel to reflect the changes
        } 
        catch (NumberFormatException e) 
        {

        } catch (IllegalArgumentException e) 
        {
            
        }
    }

    public static void main(String[] args) 
    {
        PascalTriangleGUI gui = new PascalTriangleGUI();
    }
}