package services;

import database.DatabaseConnection;
import utils.ConsoleUtil;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;

public class ProductService {
    public static void viewProducts() {
        try (Connection conn = DatabaseConnection.getConnection()) {
            String sql = """
                     SELECT p.id, p.name, p.price, c.name AS category_name
                     FROM Products p
                     LEFT JOIN Categories c ON p.category_id = c.id
                     """;
            PreparedStatement stmt = conn.prepareStatement(sql);
            ResultSet rs = stmt.executeQuery();

            System.out.println("\nProducts:");
            while (rs.next()) {
                System.out.printf("ID: %d | Name: %s | Price: %.2f | Category: %s\n",
                        rs.getInt("id"), rs.getString("name"), rs.getDouble("price"), rs.getString("category_name"));
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }


    public static void addProduct() {
        String name = ConsoleUtil.readLine("Enter product name: ");
        double price = ConsoleUtil.readDouble("Enter product price: ");
        int categoryId = ConsoleUtil.readInt("Enter category ID: ");

        try (Connection conn = DatabaseConnection.getConnection()) {
            String sql = "INSERT INTO Products (name, price, category_id) VALUES (?, ?, ?)";
            PreparedStatement stmt = conn.prepareStatement(sql);
            stmt.setString(1, name);
            stmt.setDouble(2, price);
            stmt.setInt(3, categoryId);
            stmt.executeUpdate();

            System.out.println("Product added successfully.");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }


    public static void updateProduct() {
        int id = ConsoleUtil.readInt("Enter product ID to update: ");
        String name = ConsoleUtil.readLine("Enter new product name: ");
        double price = ConsoleUtil.readInt("Enter new product price: ");
        int categoryId = ConsoleUtil.readInt("Enter new category ID: ");

        try (Connection conn = DatabaseConnection.getConnection()) {
            String sql = "UPDATE Products SET name = ?, price = ?, category_id = ?,  WHERE id = ?";
            PreparedStatement stmt = conn.prepareStatement(sql);
            stmt.setString(1, name);
            stmt.setDouble(2, price);
            stmt.setInt(3, categoryId);
            stmt.setInt(4, id);
            stmt.executeUpdate();

            System.out.println("Product updated successfully.");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void deleteProduct() {
        int id = ConsoleUtil.readInt("Enter product ID to delete: ");

        try (Connection conn = DatabaseConnection.getConnection()) {
            String sql = "DELETE FROM Products WHERE id = ?";
            PreparedStatement stmt = conn.prepareStatement(sql);
            stmt.setInt(1, id);
            stmt.executeUpdate();

            System.out.println("Product deleted successfully.");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void viewCategories() {
        try (Connection conn = DatabaseConnection.getConnection()) {
            String sql = "SELECT id, name FROM Categories";
            PreparedStatement stmt = conn.prepareStatement(sql);
            ResultSet rs = stmt.executeQuery();

            System.out.println("\nCategories:");
            while (rs.next()) {
                System.out.printf("ID: %d | Name: %s\n", rs.getInt("id"), rs.getString("name"));
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void addCategory() {
        String name = ConsoleUtil.readLine("Enter category name: ");

        try (Connection conn = DatabaseConnection.getConnection()) {
            String sql = "INSERT INTO Categories (name) VALUES (?)";
            PreparedStatement stmt = conn.prepareStatement(sql);
            stmt.setString(1, name);
            stmt.executeUpdate();

            System.out.println("Category added successfully.");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void updateCategory() {
        int id = ConsoleUtil.readInt("Enter category ID to update: ");
        String name = ConsoleUtil.readLine("Enter new category name: ");

        try (Connection conn = DatabaseConnection.getConnection()) {
            String sql = "UPDATE Categories SET name = ? WHERE id = ?";
            PreparedStatement stmt = conn.prepareStatement(sql);
            stmt.setString(1, name);
            stmt.setInt(2, id);
            stmt.executeUpdate();

            System.out.println("Category updated successfully.");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void deleteCategory() {
        int id = ConsoleUtil.readInt("Enter category ID to delete: ");

        try (Connection conn = DatabaseConnection.getConnection()) {
            String sql = "DELETE FROM Categories WHERE id = ?";
            PreparedStatement stmt = conn.prepareStatement(sql);
            stmt.setInt(1, id);
            stmt.executeUpdate();

            System.out.println("Category deleted successfully.");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
