package com.example.zadanie3;
import java.io.BufferedReader;
import java.io.InputStreamReader;

public class Launcher
{
    public static String launchExe(String parameters)
    {
        try
        {
            ProcessBuilder builder = new ProcessBuilder("cmd.exe", "/c", "C:\\Users\\Paweł Grzegory\\Desktop\\PWR\\Wip\\Lista 5\\Zadanie3\\src\\main\\resources\\Zadanie2.exe");

            String[] args = parameters.split("\\s+");
            for (String arg : args)
            {
                builder.command().add(arg);
            }

            Process process = builder.start();

            BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
            StringBuilder output = new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null)
            {
                output.append(line).append("\n");
            }

            int exitCode = process.waitFor();
            return "Process exited with code " + exitCode + "\nOutput:\n" + output.toString();
        }
        catch(Exception e)
        {
            return e.getMessage();
        }
    }
}
