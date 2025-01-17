package database;

import java.sql.Connection;
import java.sql.PreparedStatement;

public class CategorySeeder {
    public static void seed() {
        try (Connection conn = DatabaseConnection.getConnection()) {
            String sql = "INSERT INTO Categories (name) VALUES (?)";
            PreparedStatement stmt = conn.prepareStatement(sql);

            String[] categories = {"Electronics", "Clothing", "Home Appliances", "Books", "Toys"};
            for (String category : categories) {
                stmt.setString(1, category);
                stmt.executeUpdate();
            }

            System.out.println("Categories seeded successfully.");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
