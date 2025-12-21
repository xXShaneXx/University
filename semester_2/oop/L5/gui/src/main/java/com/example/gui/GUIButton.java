package com.example.gui;

import javafx.event.ActionEvent;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import javafx.scene.control.*;
import javafx.scene.layout.VBox;


public class GUIButton extends Button
{
    public GUIButton(String name, TextField textField, VBox vBox)
    {
        super(name);

        setOnAction((ActionEvent event) ->
        {
            vBox.getChildren().clear();
            int height;
            try
            {
                height = Integer.parseInt(textField.getText());
                draw(height, vBox);
            }
            catch (NumberFormatException e)
            {
                vBox.getChildren().add(new Label("Incorrect input"));
            }
            catch (IllegalArgumentException e)
            {
                vBox.getChildren().add(new Label(e.getMessage()));
            }
        });
    }

    private void draw(int height, VBox vBox) throws IllegalArgumentException
    {
        if (height < 1)
            throw new IllegalArgumentException("Number must be greater than 0");

        for (int h = 0; h < height; h++)
        {
            String row = "";
            long prev = 1;

            for (int i = 0; i <= h; i++)
            {
                row += prev + " ";
                prev = prev * (h - i) / (i + 1);
            }
            Label label = new Label(row);

            vBox.getChildren().add(label);
        }
    }
}
