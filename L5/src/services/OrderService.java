package services;

import database.DatabaseConnection;
import models.User;
import utils.ConsoleUtil;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;

public class OrderService {
    public static void viewOrders() {
        try (Connection conn = DatabaseConnection.getConnection()) {
            String sql = """
            SELECT o.id AS order_id, o.user_id, u.username, u.email
            FROM Orders o
            JOIN Users u ON o.user_id = u.id
        """;
            PreparedStatement stmt = conn.prepareStatement(sql);
            ResultSet rs = stmt.executeQuery();

            System.out.println("\nOrders:");
            while (rs.next()) {
                System.out.printf("Order ID: %d | User ID: %d | Username: %s | Email: %s\n",
                        rs.getInt("order_id"), rs.getInt("user_id"), rs.getString("username"), rs.getString("email"));
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void viewOrdersForUser(User user) {
        try (Connection conn = DatabaseConnection.getConnection()) {
            String sql = """
            SELECT p.name AS product_name, oi.quantity, o.order_date
            FROM Orders o
            JOIN Users u ON o.user_id = u.id
            JOIN OrderItems oi ON o.id = oi.order_id
            JOIN Products p ON oi.product_id = p.id
            WHERE o.user_id = ?
        """;
            PreparedStatement stmt = conn.prepareStatement(sql);
            stmt.setInt(1, user.getId());
            ResultSet rs = stmt.executeQuery();

            System.out.println("\nOrders:");
            while (rs.next()) {
                System.out.printf("Product Name: %s | Quantity: %d | Order Date: %s\n",
                       rs.getString("product_name"), rs.getInt("quantity"), rs.getTimestamp("order_date"));
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void placeOrder(User user) {
        int productId = ConsoleUtil.readInt("Enter product ID to order: ");
        int quantity = ConsoleUtil.readInt("Enter quantity: ");

        try (Connection conn = DatabaseConnection.getConnection()) {
            String sql = "INSERT INTO Orders (user_id) VALUES (?)";
            PreparedStatement stmt = conn.prepareStatement(sql, PreparedStatement.RETURN_GENERATED_KEYS);
            stmt.setInt(1, user.getId());
            stmt.executeUpdate();

            ResultSet rs = stmt.getGeneratedKeys();
            if (rs.next()) {
                int orderId = rs.getInt(1);

                sql = "INSERT INTO OrderItems (order_id, product_id, quantity) VALUES (?, ?, ?)";
                stmt = conn.prepareStatement(sql);
                stmt.setInt(1, orderId);
                stmt.setInt(2, productId);
                stmt.setInt(3, quantity);
                stmt.executeUpdate();

                System.out.println("Order placed successfully.");
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
