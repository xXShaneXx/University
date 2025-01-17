package services;

import database.DatabaseConnection;
import models.User;
import utils.BCryptUtil;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;

public class AuthService {

    public static boolean register(String username, String email, String password) {
        try (Connection conn = DatabaseConnection.getConnection()) {
            // Hashowanie hasła
            String hashedPassword = BCryptUtil.hashPassword(password);

            String sql = "INSERT INTO Users (username, email, password_hash, role_id) VALUES (?, ?, ?, ?)";
            PreparedStatement stmt = conn.prepareStatement(sql);
            stmt.setString(1, username);
            stmt.setString(2, email);
            stmt.setString(3, hashedPassword);
            stmt.setInt(4, 2); // Domyślnie nowi użytkownicy są klientami

            int rowsAffected = stmt.executeUpdate();
            return rowsAffected > 0;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }

    public static User logIn(String username, String password) {
        try (Connection conn = DatabaseConnection.getConnection()) {
            String sql = "SELECT * FROM Users WHERE username = ?";
            PreparedStatement stmt = conn.prepareStatement(sql);
            stmt.setString(1, username);
            ResultSet rs = stmt.executeQuery();

            if (rs.next()) {
                String passwordHash = rs.getString("password_hash");
                if (BCryptUtil.checkPassword(password, passwordHash)) {
                    User user = new User();
                    user.setId(rs.getInt("id"));
                    user.setUsername(rs.getString("username"));
                    user.setEmail(rs.getString("email"));
                    user.setPasswordHash(passwordHash);
                    user.setRoleId(rs.getInt("role_id"));
                    return user;
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }
}
